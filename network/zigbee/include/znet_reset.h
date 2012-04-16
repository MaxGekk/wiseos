//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_RESET_H
#define     _ZIGBEE_NETWORK_RESET_H
/*! @file  znet_reset.h
 *  @brief Сброс сетевого уровня ZigBee
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_NETWORK_RESET Сброс сетевого уровня
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *      Сбрасывается как сетевой уровень, так и нижележаший канальный.
 * */

/* Имя стекового вызова для сброса сетевого уровня */
#define     ZCALL_NLME_RESET     0

/* Аргументы стекового вызова ZCALL_NLME_RESET */
typedef struct {
    zstatus_t   status;     /*!< Статус завершения сброса сетевого уровня.
                             *   Аргумент, значение которого возвращается. Допустимы следующие значения:
                             *      - SUCCESS - успешный сброс как сетевого, так и нижележащего уровня
                             *      - MAC_DISABLE_TRX_FAILURE - не удалось выключить радиочасть */
} zargs_nlme_reset_t;

/* Тип функции сброса  */
typedef     void (* net_reset_ft )();

/* Регистрация функции сброса */
#define  REG_RESET( fn ) \
    static net_reset_ft __reset_##fn __attribute__ ((unused,__section__ (".reset.net"))) = fn;

/*! @} */
#endif      /* _ZIGBEE_NETWORK_RESET_H */

