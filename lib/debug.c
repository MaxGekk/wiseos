/*! @file   debug.c
 *  @brief  Реализация отладочной печати
 *  @author     Max Gekk
 *  @version    0.1
 *  @date   декабрь 2006 г.
 * */

#include    <combuf.h>
#include    <debug.h>

result_t dbg_log(const uint16_t arg_size, const char *const fmt, ...)
{
    const uint16_t size = arg_size + sizeof(fmt);
    uint16_t i;
    combuf_t cb;

    if (IS_ERROR(cb = combuf_create(COMBUF_DEBUG, size)))
        return cb;

    for (i = 0; i < size; i++) {
        combuf_write(cb, i, *((uint8_t *) & fmt + i));
    }

    combuf_send(cb);

    return ENOERR;
}
