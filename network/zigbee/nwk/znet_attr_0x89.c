//  vim:encoding=utf-8
/*! @file  znet_attr_0x89.c
 *  @author     Max Gekk
 *  @date       ноябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <znet_reset.h>

/* По умолчанию стоимость пути не константа и должна вычисляться ( например, по
 * по индикатору качества связи - LQI ) */
#define     DEFAULT_REPORT_CONSTANT_CONST   FALSE

/* Атрибут, определяющий является ли стоимость пути до любого соседнего узла
 * константой или нет. */
bool_t  nwkReportConstantConst = DEFAULT_REPORT_CONSTANT_CONST;

/* Сброс атрибута в значение по умолчанию */
void    znet_reset_a0x89()
{
    nwkReportConstantConst = DEFAULT_REPORT_CONSTANT_CONST;
}

REG_RESET( znet_reset_a0x89 );

