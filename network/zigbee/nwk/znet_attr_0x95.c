//  vim:encoding=utf-8
/*! @file  znet_attr_0x95.c
 *  @brief  Атрибут - время хранения транзакций
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* Значение атрибута по умолчанию, измеряемое в количестве суперфреймов */
#define     DEFAULT_PERSIS_TIME   0x01f4;

/* Атрибут, фактически, определяет число суперфреймов устройства, в течении
 * которых фреймы для непрямой передачи дочерним узлам должны хранится на узле */
uint16_t    nwkTransactionPersistenceTime = DEFAULT_PERSIS_TIME;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x95()
{
    nwkTransactionPersistenceTime = DEFAULT_PERSIS_TIME;
}

REG_RESET( znet_reset_a0x95 );


