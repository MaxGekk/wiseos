#include    <critical.h>
#include    <init.h>
#include    <task.h>
#include    <uart.h>
#include    <combuf.h>

#if defined(DBGLOG)
#undef	DBGLOG
#endif

#include    <debug.h>

/* размер хранилища буферов */
#define     COMBUF_STORE_SIZE   256

/* тип, максимальное значение которого >= COMBUF_STORE_SIZE */
typedef uint16_t cb_store_t;

/* суммирование по модулю COMBUF_STORE_SIZE */
/* Если COMBUF_STORE_SIZE степень 2, то для
 * оптимизации можно использовать &(COMBUF_STORE_SIZE-1) ,
 * иначе  % COMBUF_STORE_SIZE */
//#define     SUM( a1, a2 )   ( ( a1 + a2 ) % COMBUF_STORE_SIZE )
#define     SUM( a1, a2 )   ( ( a1 + a2 ) & (COMBUF_STORE_SIZE-1) )

/* хранилище буферов */
static uint8_t combuf_store[COMBUF_STORE_SIZE];

/* число занятых элементов хранилища ( байтов ) */
static cb_store_t busy;

/* Тип экземпляры, которого задают смещение в хранилище относительно его начала.
 * Тип должен быть целым. Максимальное значение не меньше COMBUF_STORE_SIZE */
typedef int16_t cb_offset_t;

/* следующий элемент после первого в очереди */
static cb_offset_t front;
/* последний элемент очереди. ( можно не защищать в критических секциях,
 * так обращение к переменное происходит только из задач ) */
static cb_offset_t back;

/* размер заголовка буфера */
#define     COMBUF_HEAD_SIZE    3
/* размер служебной информации в конце буфера ( контрольная сумма ) */
#define     COMBUF_END_SIZE     1

/* маркер начала буфер. */
#define     BEGIN_MARKER        0xeb
/* маркеры, используемые при стаффинге */
#define     ESC_MARKER          0xda
#define     XOR_MARKER          0x19

#define     CHECK_COMBUF( combuf )         ( ( 0 <= combuf )&&( BEGIN_MARKER == combuf_store[ combuf ] ) )
#define     OFFSET( combuf, offset )    SUM( combuf, offset )
#define     ITEM( combuf, offset )      combuf_store[ OFFSET( combuf, offset ) ]

#define     BEGIN( combuf )      ITEM( combuf, 0 )
#define     SIZE( combuf )       ITEM( combuf, 1 )
#define     CONTROL( combuf )    ITEM( combuf, 2 )
#define     CSUM( combuf )       ITEM( combuf, ( SIZE( combuf )-1) )

#define     PAYLOAD_SIZE( combuf )  ( SIZE( combuf ) - COMBUF_HEAD_SIZE - COMBUF_END_SIZE )
#define     PAYLOAD_ITEM( combuf, offset )  ITEM( combuf, COMBUF_HEAD_SIZE + offset )

#define     TYPE_MASK       0x0f
#define     CREATE_MASK     0x10
#define     INPUT_MASK      0x20
#define     READY_MASK      0x40

#define     DESTROY( combuf )    CONTROL( combuf ) &= ~CREATE_MASK
#define     IS_DESTROYED( combuf )  ( 0x00 == ( CONTROL( combuf ) & CREATE_MASK ) )

#define     IS_BACK( combuf )    ( back == combuf )

combuf_t combuf_create(const uint8_t control, const combuf_size_t payload_size)
{
    const cb_store_t cb_size =
        COMBUF_HEAD_SIZE + payload_size + COMBUF_END_SIZE;
    cb_store_t _busy = cb_size;
    result_t res = ENOERR;
    cb_offset_t _front;

    /* критическая секция нужна, потому что combuf_create может быть
     * вызвана как из прерывания, так и из задачи. */
    critical_enter();
    _busy += busy;
    if (_busy <= COMBUF_STORE_SIZE) {
        _front = front;
        front = SUM(front, cb_size);
        busy = _busy;
    } else
        res = ENOMEM;
    critical_exit();

    if (ENOERR != res)
        return res;

    /* Можно не вносить в критическую секцию, поскольку доступ к этим
     * полям осуществляется пользователем по описателю буфера, который
     * ему ещё не возвращён. */
    BEGIN(_front) = BEGIN_MARKER;
    SIZE(_front) = cb_size;
    CONTROL(_front) = control | CREATE_MASK;

    return (combuf_t) _front;
}

/* Уничтожение буфера. Функция вызывается только из внутренних задач.*/
static void combuf_destroy(const combuf_t combuf)
{
    ASSERT1(CHECK_COMBUF(combuf), "invalid combuf = %u", combuf);

    /* Пометить буфер для удаления */
    DESTROY(combuf);

    /* Если буфер последний в очереди, то удаляем его и
     * следующие буферы в очереди ( если они помечены для удаления ) */
    if (IS_BACK(combuf)) {
        cb_store_t _busy;
        cb_store_t cb_size;

        do {
            cb_size = SIZE(back);
            back = SUM(back, cb_size);

            critical_enter();
            busy -= cb_size;
            _busy = busy;
            critical_exit();

        } while ((0 < _busy) && IS_DESTROYED(back));

    }
}

result_t combuf_write(const combuf_t combuf,
                      const combuf_pos_t pos, const uint8_t data)
{
    ASSERT1(CHECK_COMBUF(combuf), "invalid combuf = %d", combuf);
    ASSERT2(pos < PAYLOAD_SIZE(combuf),
            "invalid pos = %hhu ( combuf = %d)", pos, combuf);

    PAYLOAD_ITEM(combuf, pos) = data;

    return ENOERR;
}

uint8_t combuf_read(const combuf_t combuf, const combuf_pos_t pos)
{
    ASSERT1(CHECK_COMBUF(combuf), "invalid combuf = %d", combuf);
    ASSERT2(pos < PAYLOAD_SIZE(combuf),
            "invalid pos = %hhu ( combuf = %d)", pos, combuf);

    return PAYLOAD_ITEM(combuf, pos);
}

/* функция подсчёта контрольной суммы */
static uint8_t calc_csum(const combuf_t combuf)
{
    uint8_t crc = 0;
    cb_offset_t i = 0;
    cb_store_t cb_size;
    ASSERT1(CHECK_COMBUF(combuf), "invalid combuf = %d", combuf);

    cb_size = SIZE(combuf) - COMBUF_END_SIZE;
    for (; i < cb_size; i++)
        crc += ITEM(combuf, i);

    return crc;
}

/* функция поиска буфера по полю контроля буфера.
 * Функция вызывается только из задач данного модуля. */
static combuf_t combuf_search(const uint8_t mask, const uint8_t value)
{
    cb_store_t cb_size;
    cb_offset_t _front;
    uint8_t cb_ctrl;
    combuf_t cb_iter = back;

    while (cb_iter != front) {

        /* не защищаем CONTROL( cb_iter ), так как предполагаем,
         * что вычитывание атомарно */
        if (value == (CONTROL(cb_iter) & mask))
            return cb_iter;

        /* поле SIZE( cb_iter ) можно не вычитывать в крит. секции, так как
         * это поле не изменяется никем. Изменяется только в combuf_create, но
         * это не страшно:
         * 1. если combuf_create выз. из задачи, то всё хорошо. 
         *      front и поле size будут согласованы.
         * 2. если из прерывания, то тоже согласованы.  */
        cb_iter = SUM(cb_iter, SIZE(cb_iter));
    }

    return ENOTFOUND;
}

static combuf_t out_combuf;
static cb_offset_t out_offset = -2;
static uint8_t out_xor_char = 0;

void task_combuf_send()
{
    switch (out_offset) {
    case -1:
        combuf_destroy(out_combuf);
        ++out_offset;
    case 0:
        out_combuf =
            combuf_search(READY_MASK | INPUT_MASK | CREATE_MASK,
                          READY_MASK | CREATE_MASK);
        if (out_combuf < 0)
            break;

        /* подсчитываем контрольную сумму. Поле можно не защищать,
         * поскольку доступ только в этом месте. */
        CSUM(out_combuf) = calc_csum(out_combuf);

        out_xor_char = 0;
        ++out_offset;
        uart_tx(BEGIN_MARKER);
        return;
    }
    out_offset = -2;
}

TASK(task_combuf_send, 1);

/* Функция вызывается из прерывания и не может быть прервана. */
void uart_tx_done()
{
    if (out_offset < SIZE(out_combuf)) {
        uint8_t data;
        data = ITEM(out_combuf, out_offset);
        data ^= out_xor_char;
        if ((BEGIN_MARKER == data) || (ESC_MARKER == data)) {
            out_xor_char = XOR_MARKER;
            uart_tx(ESC_MARKER);
            return;
        }
        out_xor_char = 0;
        ++out_offset;
        uart_tx(data);
    } else {
        out_offset = -1;
        POST_TASK(task_combuf_send);
    }
}

result_t combuf_send(const combuf_t combuf)
{
    cb_offset_t _out_offset;
    ASSERT1(CHECK_COMBUF(combuf), "invalid combuf = %d", combuf);

    critical_enter();
    CONTROL(combuf) |= READY_MASK;
    _out_offset = out_offset;
    if (-2 == out_offset)
        out_offset = 0;
    critical_exit();

    if (-2 == _out_offset)
        POST_TASK(task_combuf_send);

    return ENOERR;
}

extern combuf_recv_t __combuf_recv_start;
extern combuf_recv_t __combuf_recv_end;

void task_combuf_recv()
{
    combuf_t cb_iter;
    while (0 <= (cb_iter = combuf_search(READY_MASK | INPUT_MASK | CREATE_MASK,
                                         READY_MASK | INPUT_MASK |
                                         CREATE_MASK))) {
        /* проверяем контрольную сумму */
        if (CSUM(cb_iter) == calc_csum(cb_iter)) {
            /* уведомляем о приёме буфера */
            combuf_recv_t *combuf_recv = &__combuf_recv_start;
            for (; combuf_recv < &__combuf_recv_end; combuf_recv++)
                (*combuf_recv) (cb_iter, PAYLOAD_SIZE(cb_iter));
        }
        combuf_destroy(cb_iter);
    }
}

TASK(task_combuf_recv, 1);

void uart_rx_done(uint8_t data)
{
    static combuf_t in_combuf;
    static cb_offset_t in_offset = 0;
    static uint8_t in_xor_char = 0;

    if (0 == in_offset) {
        if (BEGIN_MARKER != data)
            return;
        ++in_offset;
        in_xor_char = 0;
        return;
    }
    if (ESC_MARKER == data) {
        in_xor_char = XOR_MARKER;
        return;
    }
    data ^= in_xor_char;
    in_xor_char = 0;
    if (1 == in_offset) {
        in_combuf =
            combuf_create(0, (data - COMBUF_HEAD_SIZE - COMBUF_END_SIZE));
        if (in_combuf < 0) {
            in_offset = 0;
            return;
        }
    }
    ITEM(in_combuf, in_offset) = data;
    if (++in_offset == SIZE(in_combuf)) {
        CONTROL(in_combuf) |= READY_MASK;
        POST_TASK(task_combuf_recv);
        in_offset = 0;
    }
}

void init_combuf()
{
    uart_start();
}

INIT(init_combuf);
