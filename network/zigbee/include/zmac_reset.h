//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_RESET_H
#define     _ZIGBEE_MAC_RESET_H
/*! @file  zmac_reset.h
 *  @brief Сброс MAC-уровня стандарта IEEE 802.15.4
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup ZIGBEE_MAC_RESET Сброс MAC-уровня
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 *      Сбрасывается MAC-уровень и останавливается физический уровень
 * */

/* Имя стекового вызова для сброса MAC-уровня */
#define     ZCALL_MLME_RESET    64

/* Аргументы стекового вызова ZCALL_MLME_RESET */
typedef struct {
    bool_t  set_default_pib;    /*!< IN: TRUE - установить все атрибуты в значение по-умолчанию */
    zstatus_t   status;         /*!< OUT: статус завершения процедуры сброса */
} zargs_mlme_reset_t;

/*! @} */
#endif      /* _ZIGBEE_MAC_RESET_H */

