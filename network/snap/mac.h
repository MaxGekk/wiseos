#ifndef     _MAC_H
#define     _MAC_H
/*! @file mac.h
 *  @brief Сервисы подуровня MAC стека SNAP
 *  @author     Max Gekk
 *  @author     Serge Pyptev 
 *  @date       январь 2007 г.
 *  @version    1.1
 
 *  @defgroup SNAP_MAC   Подуровень MAC
 *  @ingroup  SNAP_DL
 *  @{
 *      Подуровень MAC осуществляет управление транзакциями обмена данными и 
 *  служебной информацией между устройствами, поддерживающими стек SNAP. 
 *  Подуровень MAC отличается от вышележащих уровней тем, что все  действия должны 
 *  выполняться в более жёстких временных рамках. То есть подуровень MAC инкапсулирует 
 *  такие последовательности операций, временные интервалы между которыми менее 10 мс.
 *  MAC-подуровень параллельно выполняет две основные процедуры:
 *      - обмен данными с соседними узлами;
 *      - знакомство с соседними узлами.
 *
 *      Интенсивность выполнения каждой из процедур контролируется вышележащим уровнем стека SNAP, 
 *  а также определяется механизмами управления энергопо-треблением на самом подуровне MAC. 
 * */

#include    <types.h>

/*! @typedef    dl_result_t
 *  @brief Тип результата вызова функции
 * */
typedef enum {
    DLE_NOERR   =   0   /*!< ошибки нет */
} dl_result_t;

/*! @typedef    dl_status_t
 *  @brief  Тип статуса завершения операции
 *  */
typedef enum {
    DLS_SUCCESS  = 0     /*!< успешное завершение операции */
} dl_status_t;

typedef     uint32_t    dl_node_id_t;           /*!< Тип уникального идентификатора узла */
typedef     uint16_t    dl_size_t;              /*!< Тип, задающий размер памяти */          
typedef     uint16_t    dl_channel_t;           /*!< Тип, определяющий номер канала */
typedef     uint32_t    dl_duration_t;          /*!< Тип временных интервалов */
typedef     uint32_t    dl_time_t;              /*!< Тип локального времени */
typedef     uint32_t    dl_session_param_t;     /*!< Тип параметра сеанса */

/*! @defgroup SNAP_MAC_CONTROL   Сервис общего управления
 *  @ingroup SNAP_MAC
 *  @{
 *      Данный сервис позволяет вышележащему уровню производить общее 
 *  управление и контроль над MAC-подуровнем.
 * */

/*! @fn dl_result_t     mac_start( const dl_node_id_t   node_id,
 *  const dl_channel_t    meet_channel, const dl_duration_t   meet_duration,
 *  void    *const in_mem_ptr,  const dl_size_t in_mem_size )
 *      Вызов функции приводит к запуску и инициализации подуровня.
 *
 *  @param node_id – идентификатор узла;
 *  @param meet_channel – номер канала для знакомства с соседними узлами;
 *  @param meet_duration – продолжительность интервала знакомств;
 *  @param in_mem_ptr – указатель на область памяти для буферов ввода, 
 *  используемых для передачи данных от подуровня MAC вышележащему уровню;
 *  @param in_mem_size – размер области памяти для буферов ввода.
 * */
dl_result_t     mac_start( const dl_node_id_t   node_id, 
        const dl_channel_t    meet_channel, const dl_duration_t   meet_duration,
        void    *const in_mem_ptr,  const dl_size_t in_mem_size );

/*! @fn void    mac_start_done( dl_status_t     status )
 *      Функция вызывается подуровнем MAC. С помощью данного примитива подуровень 
 *
 *  извещает об окончании процедуры запуска, инициированной функцией mac_start(). 
 *  После запуска подуровня атрибуты local_time и epoch принимают нулевые значения.
 *  @param status – статус завершения запуска подуровня.
 * */
void    mac_start_done( dl_status_t     status );

/*! @fn dl_result_t     mac_reset()
 *      Сброс текущего состояния подуровня MAC к состоянию, в котором был подуровень 
 *
 *  после последнего вызова функции mac_start(). 
 *  */
dl_result_t     mac_reset();

/*! @fn void    mac_reset_done( dl_status_t     status )
 *      Завершение процедуры сброса подуровня, инициированной функцией mac_reset().
 *
 *  @param status – статус завершения процедуры сброса подуровня.
 *  */
void    mac_reset_done( dl_status_t     status );

/*! @fn dl_result_t     mac_stop();
 *      Останов подуровня MAC.
 * */
dl_result_t     mac_stop();

/*! @fn void    mac_stop_done( dl_status_t      status )
 *      Завершение процедуры останова подурровня MAC.
 * */
void    mac_stop_done( dl_status_t      status );
/*! @} */

/*! @defgroup SNAP_MAC_BUFFER   Сервис буферов ввода-вывода
 *  @ingroup SNAP_MAC
 *  @{
 *      С помощью данного сервиса подуровень MAC и вышележащий уровень 
 *  осуществляют обмен данными. Через рассматриваемый сервис данные 
 *  извлекаются и передаются подуровню MAC. Буферы ввода-вывода используются 
 *  при выполнении действий определяемых через сервисы знакомств, передачи и 
 *  приёма данных. Существует два типа буферов: (1) буферы ввода используются
 *  для передачи данных вышележащему уровню от MAC-подуровня, 
 *  (2) буферы вывода используются для передачи данных от вышележащего уровня
 *  подуровню MAC. 
 * */

typedef     int16_t         dl_buffer_t;   /*!< Тип дескриптора буфера ввода-вывода. */
typedef     dl_buffer_t    dl_obuf_t;     /*!< Тип дескриптора буфера вывода. */
typedef     dl_buffer_t    dl_ibuf_t;     /*!< Тип дескриптора буфера ввода. */

/*! @fn dl_result_t     mac_new_obuf( const dl_size_t data_size, void *const data_ptr, dl_obuf_t  *const obuf_ptr )
 *      Создание нового буфера вывода.
 *  @param data_size – размер данных, которые будет содержать создаваемый буфер вывода;
 *  @param data_ptr – указатель на область памяти с отправляемыми данными;
 *  @param obuf_ptr - указатель на область памяти, в которую будет скопирован дескриптор нового
 *  буфера вывода.
 * */
dl_result_t     mac_new_obuf( const dl_size_t data_size, void *const data_ptr, dl_obuf_t  *const obuf_ptr );

/*! @fn dl_result_t     mac_ibuf_size( const dl_ibuf_t ibuf, dl_size_t *const size_ptr )
 *      Функция возвращает размер данных, которые содержит буфер ввода.
 *  @param ibuf - дескриптор буфера ввода;
 *  @param size_ptr - указатель на размер буфера ввода.
 * */
dl_result_t     mac_ibuf_size( const dl_ibuf_t ibuf, dl_size_t *const size_ptr );

/*! @fn dl_result_t     mac_ibuf_data( const dl_ibuf_t ibuf, void  **const data_pptr )
 *      С помощью данной функции вышележащий уровень может получить доступ к данным, 
 *  которые содержит буфер ввода. 
 *
 *  @param ibuf - дескриптор буфера ввода.
 *  @param data_pptr - указатель на область памяти, в которую будет в свою очередь скопирован
 *  указатель на данные буфера ввода.
 * */
dl_result_t     mac_ibuf_data( const dl_ibuf_t ibuf, void  **const data_pptr );

/*! @fn dl_result_t     mac_destroy_ibuf( const dl_ibuf_t  ibuf )
 *      Разрушение буфера ввода, при этом освобождается память занятая буфером.
 *  @param ibuf - дескриптор буфера ввода.
 * */
dl_result_t     mac_destroy_ibuf( const dl_ibuf_t  ibuf );

/*! @} */

/*! @defgroup SNAP_MAC_ATTRIBUTE   Сервис доступа к атрибутам
 *  @ingroup SNAP_MAC
 *  @{
 *      Данный сервис позволяет установить и считать значения атрибутов подуровня MAC. 
 * */

/*! @fn dl_result_t     mac_set_time( const dl_time_t   time )
 *      Установка локального времени. В качестве параметра функция принимает текущее 
 *  значение времени. После вызова функции подуровень MAC устанавливает своё локальное 
 *  время в заданное значение и увеличивает его на единицу c шагом не более 1/30000 секунды.
 *  @param time – новое значение времени.
 * */
dl_result_t     mac_set_time( const dl_time_t   time );

/*! @fn dl_result_t     mac_get_time( dl_time_t     *const time_ptr )
 *      Функция возвращает локальное время подуровня MAC.
 *  @param time_ptr - указатель на локальное время подуровня MAC.
 * */
dl_result_t     mac_get_time( dl_time_t     *const time_ptr );

/*! @fn dl_result_t     mac_set_epoch( const dl_duration_t  epoch )
 *      Данная функция позволяет задать новое значение эпохи.
 *  @param epoch – значение эпохи.
 * */
dl_result_t     mac_set_epoch( const dl_duration_t  epoch );

/*! @fn dl_result_t     mac_get_epoch( dl_duration_t    *const epoch_ptr )
 *      Данная функция возвращает текущее значение эпохи.
 *  @param epoch_ptr - указатель на текущее значение эпохи.
 * */
dl_result_t     mac_get_epoch( dl_duration_t    *const epoch_ptr );
/*! @} */

/*! @defgroup SNAP_MAC_MEET   Сервис знакомства с соседями
 *  @ingroup SNAP_MAC
 *  @{
 *      Данный сервис предназначен для осуществления процедуры знакомства
 *  с соседними узлами.
 * */

/*! @fn dl_result_t     mac_meet( const dl_time_t   shot_time, const dl_channel_t shot_channel, const dl_obuf_t  obuf )
 *      Данная функция позволяет задать параметры процедуры знакомств. Сеанс знакомства 
 *  начинает по некоторому сигналу синхронизации. Такой сигнал может быть сформирован 
 *  MAC-подуровнями узлов или получен другими средствами. Вышележащий уровень должен 
 *  настроить MAC-подуровень на сеанс знакомства, который начнётся при получении следующего 
 *  сигнала синхронизации. Для этого подуровню должно быть передано время и номер канала того 
 *  сеанса передачи данных, который начнётся предположительно после завершения сеанса знакомства. 
 *  Продолжительность времени до сеанса передачи данных подуровень должен вычислить самостоятельно. 
 *  Также должны быть переданы данные о знакомствах узла. Идентификатор узла, номер канала для сеанса 
 *  знакомства  и длительность процедуры знакомства могут быть получены из соответствующих атрибутов подуровня.
 *
 *  @param shot_time – момент времени одного из будущих сеансов передачи данных;
 *  @param shot_channel – номер канала сеанса передачи;
 *  @param obuf – дескриптор буфера вывода, содержащий данные о знакомствах узла.
 * */
dl_result_t     mac_meet( const dl_time_t   shot_time, const dl_channel_t shot_channel, const dl_obuf_t  obuf );

/*! @fn dl_result_t     mac_cancel_meet( dl_obuf_t *const obuf_ptr );
 *      Отмена ранее заданного с помощью функции mac_meet() сеанса знакомства с соседними узлами.
 *  @param obuf_ptr - указатель на дескриптор буфера вывода, переданный в соответствующий вызов mac_meet().
 * */
dl_result_t     mac_cancel_meet( dl_obuf_t *const obuf_ptr );

/*! @fn dl_result_t     mac_change_meet( const dl_time_t    new_shot_time, const dl_channel_t   new_shot_channel );
 *      Изменение некоторых параметров сеанса знакомства, ранее заданных с помощью функции mac_meet(). 
 *  Данная функция позволяет изменить те параметры, которые в следствии внешних по отношению к подуровню MAC 
 *  факторов могут потерять актуальность к моменту начала сеанса знакомства.
 *
 *  @param new_shot_time – время начала одного из сеансов передачи данных, который вероятнее всего начнётся 
 *  после окончания сеанса знакомства.
 *  @param new_shot_channel – номер канала, на котором начнётся сеанс передачи данных. Время начала этого 
 *  сеанса определяется параметром new_shot_time.
 * */
dl_result_t     mac_change_meet( const dl_time_t    new_shot_time, const dl_channel_t   new_shot_channel );

/*! @fn void    mac_meet_notify_enable();
 *      Разрешение подуровню MAC  асинхронно извещать вышележащий уровень о событиях связанных с 
 *  процедурой знакомства.
 * */
dl_result_t     mac_meet_notify_enable();

/*! @fn void    mac_meet_notify_disable();
 *      Запрещение подуровню MAC асинхронно извещать вышележащий уровень о событиях связанных с 
 *  процедурой знакомства.
 * */
dl_result_t     mac_meet_notify_disable();

/*! @fn void    mac_meet_begin();
 *      Асинхронное извещение вышележащего уровня о начале сеанса знакомства.
 * */
void    mac_meet_begin();

/*! @fn void    mac_meet_done()
 *      Асинхронное извещение вышележащего уровня о завершении сеанса знакомства.
 * */
void    mac_meet_done();

/*! @fn dl_result_t     mac_meet_info( dl_status_t  *const meet_status_ptr, dl_ibuf_t  *const ibuf_ptr );
 *      Получение информации о завершённом сеансе знакомства с соседними узлами. Предполагается, что 
 *  на MAC-подуровне существует очередь FIFO, элементы которой содержат информацию о завершённых сеансах 
 *  знакомств. При завершении очередного сеанса информация о нём помещается в конец очереди. При вызове 
 *  данного примитива данные извлекаются из начала очереди. Из очереди извлечённые данные удаляются.
 *
 *  @param meet_status_ptr - указатель на статус завершения процедуры знакомства;
 *  @param ibuf_ptr - указатель на дескриптор буфера ввода. Буфер содержит  информацию о параметрах 
 *  ближайших сеансов связи как можно большего числа узлов, участвующих в знакомстве, а также данные 
 *  об их знакомствах. Буфер должен содержать данные и информацию в том порядке, в котором они получены от соседних узлов.
 * */
dl_result_t     mac_meet_info( dl_status_t  *const meet_status_ptr, dl_ibuf_t  *const ibuf_ptr );

/*! @} */

/*! @defgroup SNAP_MAC_TX   Сервис отправки данных
 *  @ingroup SNAP_MAC
 *  @{
 *      Сервис предназначен для  передачи данных соседним знакомым узлам.
 * */

typedef     uint16_t    dl_shot_t;          /*!< Тип дескриптора сеанса передачи данных */

/*! @fn dl_result_t     mac_fire_shot( const dl_time_t  time, const dl_channel_t    channel,
        const dl_session_param_t  session_param, const dl_node_id_t    trap_id,
        const dl_obuf_t    obuf, dl_shot_t    *const shot_ptr )

 *  Определение параметров следующего сеанса связи узла. Данная функция вызывается вышележащим 
 *  уровнем. При вызове подуровню MAC указывается в какой момент времени и на каком канале должен 
 *  начаться сеанс связи. Также передаётся параметр сеанса, буфер, содержащий передаваемые данные
 *  и идентификатор узла, которому предназначены эти данные. Значение эпохи и идентификатор самого
 *  узла, должны быть взяты из атрибутов MAC-подуровня.
 *
 *  @param time – момент времени в локальных часах подуровня MAC, начиная с которого должен 
 *  начаться сеанс передачи данных;
 *  @param channel – номер канала, на котором должен начаться сеанс передачи данных;
 *  @param session_param – параметр сеанса;
 *  @param trap_id – идентификатор узла, которому предназначены данные;
 *  @param obuf – дескриптор буфера вывода, содержащий передаваемые данные;
 *  @param shot_ptr - указатель на дескриптор сеанса передачи данных.
 * */
dl_result_t     mac_fire_shot( const dl_time_t  time, const dl_channel_t    channel,
        const dl_session_param_t  session_param, const dl_node_id_t    trap_id,
        const dl_obuf_t    obuf, dl_shot_t    *const shot_ptr );

/*! @fn dl_result_t     mac_cancel_shot( const dl_shot_t  shot, dl_obuf_t   *const obuf_ptr )
 *      Отмена ранее заданного с помощью примитива mac_fire_shot() сеанса передачи данных.
 *  @param shot – дескриптор сеанса передачи данных;
 *  @param obuf_ptr – указатель на дескриптор буфера вывода, переданный в функцию mac_fire_shot().
 * */
dl_result_t     mac_cancel_shot( const dl_shot_t  shot, dl_obuf_t   *const obuf_ptr );

/*! @fn dl_result_t     mac_change_shot( const dl_shot_t  shot, const dl_node_id_t  new_trap_id,
        const dl_obuf_t new_obuf, dl_obuf_t *const old_obuf_ptr )

 *  Изменение некоторых параметров сеанса передачи данных,  ранее заданных с помощью mac_fire_shot(). 
 *  Данная функция позволяет изменить те параметры, которые в следствии внешних по отношению к подуровню 
 *  MAC факторов могут потерять актуальность к моменту начала сеанса передачи данных.
 *
 *  @param shot – дескриптор сеанса передачи данных;
 *  @param new_trap_id – новый идентификатор узла, которому предназначены данные;
 *  @param new_obuf – дескриптор нового буфера вывода;
 *  @param old_obuf_ptr – указатель на дескриптор старого буфера вывода.
 * */
dl_result_t     mac_change_shot( const dl_shot_t  shot, const dl_node_id_t  new_trap_id,
        const dl_obuf_t new_obuf, dl_obuf_t *const old_obuf_ptr );

/*! @fn void    mac_shot_notify_enable();
 *      Разрешение подуровню MAC  асинхронно извещать вышележащий уровень о событиях связанных с 
 *  сеансом передачи данных.
 * */
dl_result_t     mac_shot_notify_enable();

/*! @fn void    mac_shot_notify_disable();
 *      Запрещение подуровню MAC асинхронно извещать вышележащий уровень о событиях связанных с 
 *  сеансом передачи данных.
 * */
dl_result_t     mac_shot_notify_disable();

/*! @fn void    mac_shot_begin();
 *      Асинхронное извещение вышележащего уровня о начале сеанса передачи данных.
 * */
void    mac_shot_begin();

/*! @fn void    mac_shot_done()
 *      Асинхронное извещение вышележащего уровня о завершении сеанса передачи данных.
 * */
void    mac_shot_done();

/*! @fn dl_result_t mac_shot_info( dl_shot_t    *const shot_ptr, dl_status_t    *const status_ptr,
        dl_node_id_t    *const trap_id_ptr, dl_obuf_t   *const obuf_ptr )

 *  Получение информации о завершённом сеансе передачи данных. Предполагается, что на MAC-подуровне 
 *  существует очередь FIFO, элементы которой содержат информацию о завершённых сеансах передачи данных. 
 *  При завершении очередного сеанса информация о нём помещается в конец очереди. 
 *  При вызове данного примитива данные извлекаются из начала очереди. 
 *  Из очереди извлечённые данные удаляются. 
 *
 *  @param shot_ptr - указатель на дескриптор сеанса передачи данных;
 *  @param status_ptr - указатель на статус завершения сеанса передачи данных;
 *  @param trap_id_ptr - указатель на идентификатор узла, которому должны были быть переданы данные;
 *  @param obuf_ptr - указатель на дескриптор буфера вывода.
 * */
dl_result_t mac_shot_info( dl_shot_t    *const shot_ptr, dl_status_t    *const status_ptr,
        dl_node_id_t    *const trap_id_ptr, dl_obuf_t   *const obuf_ptr );

/*! @} */

/*! @defgroup SNAP_MAC_RX   Сервис приёма данных
 *  @ingroup SNAP_MAC
 *  @{
 *      С помощью сервиса приёма данных вышележащий уровень может принимать данные от соседних 
 *  знакомых узлов.
 * */

typedef     uint16_t    dl_catcher_t; /*!< Тип дескриптора сеанса приёма данных */

/*! @fn dl_result_t     mac_catch_shot( const dl_time_t time, const dl_channel_t channel, dl_catcher_t  *const catcher_ptr )
 *      Определение параметров следующего сеанса приёма данных. Данная функция вызывается вышележащим уровнем. 
 *  При вызове подуровню MAC указывается в какой момент времени по локальным часам и на каком канале 
 *  начинать приём данных. 
 *
 *  @param time – момент времени в локальных часах подуровня MAC, начиная с которого 
 *  должен начаться приём данных;
 *  @param channel - номер канала, на котором должен начаться приём данных;
 *  @param catcher_ptr - указатель на область памяти, в которую будет записан дескриптор сеанса
 *  приёма данных.
 * */
dl_result_t     mac_catch_shot( const dl_time_t time, const dl_channel_t channel, 
        dl_catcher_t  *const catcher_ptr );


/*! @fn dl_result_t     mac_cancel_catch( const dl_catcher_t  catcher )
 *  Отмена ранее заданного с помощью функции mac_catch_shot() сеанса приёма данных.
 *  @param catcher - дескриптор сеанса приёма данных.
 * */
dl_result_t     mac_cancel_catch( const dl_catcher_t  catcher ); 

/*! @fn void    mac_catch_notify_enable();
 *      Разрешение подуровню MAC  асинхронно извещать вышележащий уровень о событиях связанных с 
 *  сеансом приёма данных.
 * */
dl_result_t     mac_catch_notify_enable();

/*! @fn void    mac_catch_notify_disable();
 *      Запрещение подуровню MAC асинхронно извещать вышележащий уровень о событиях связанных с 
 *  сеансом приёма данных.
 * */
dl_result_t     mac_catch_notify_disable();

/*! @fn void    mac_catch_begin();
 *      Асинхронное извещение вышележащего уровня о начале сеанса приёма данных.
 * */
void    mac_catch_begin();

/*! @fn void    mac_catch_done()
 *      Асинхронное извещение вышележащего уровня о завершении сеанса приёма данных.
 * */
void    mac_catch_done();

/*! @fn dl_result_t     mac_catch_info( dl_catcher_t    *const catcher_ptr, dl_status_t *const status_ptr,
        dl_node_id_t    *const shot_id_ptr, dl_time_t   *const shot_time_ptr, 
        dl_session_param_t  *const session_param_ptr, dl_duration_t *const epoch_ptr,
        dl_ibuf_t *const ibuf_ptr )

 *      Получение информации о завершённом сеансе приёма данных. Предполагается, что на MAC-подуровне 
 *  существует очередь FIFO, элементы которой содержат информацию о завершённых сеансах приёма данных. 
 *  При завершении очередного сеанса информация о нём помещается в конец очереди. При вызове данной 
 *  функции данные извлекаются из начала очереди. Из очереди извлечённые данные удаляются.
 
 *  @param catcher_ptr - указатель на дескриптор сеанса приёма данных;
 *  @param status_ptr - указатель на статус завершения сеанса приёма данных;
 *  @param shot_id_ptr - указатель на идентификатор передающего узла;
 *  @param shot_time_ptr - указатель на момент времени начала сеанса приёма данных;
 *  @param session_param_ptr - указатель на параметр сеанса передающего узла;
 *  @param epoch_ptr - указатель на эпоху передающего узла;
 *  @param ibuf_ptr - указатель на дескриптор буфера ввода, содержащий принятые данные. 
 * */
dl_result_t     mac_catch_info( dl_catcher_t    *const catcher_ptr, dl_status_t *const status_ptr,
        dl_node_id_t    *const shot_id_ptr, dl_time_t   *const shot_time_ptr, 
        dl_session_param_t  *const session_param_ptr, dl_duration_t *const epoch_ptr,
        dl_ibuf_t *const ibuf_ptr );

/*! @} */

/*! @} */
#endif      /* _MAC_H */

