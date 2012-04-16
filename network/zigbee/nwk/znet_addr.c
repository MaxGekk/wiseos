//  vim:encoding=utf-8
/*! @file  znet_addr.c
 *  @brief Выделение адресов
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *
 *      Выделение адресов дочерним узлам. Механизм раздачи адресов описан
 *  в разделе 3.7.1.5 спецификации ZigBee 2006.
 *
 *  */

#include    <math.h>
#include    <ztypes.h>
#include    <zneighbor.h>
#include    <zmac_attr.h>
#include    <znet_attr.h>
#include    <znet_relative.h>
#include    <znet_addr.h>

/* Выделение уникального ( среди всех детей этого узла ) адреса для заданного типа устройства 
 * Тип устройства может быть FFD - полнофункциональное и RFD - конечное.
 * */
s_addr_t    znet_addr_alloc( uint8_t dev_type )
{
    if( nwkUseTreeAddrAlloc == TRUE ) {
        /* Распределённая (иерархическая) раздача адресов */
        const uint16_t depth = nwkExtraAttr.depth+1;    /* Глубина дочерних узлов */    
        const uint16_t t = nwkMaxDepth - depth - 1;
        uint16_t cskip;
        if( nwkMaxDepth < depth )
            return 0xffff;  /* Превышение максимальной глубины */
        /* Вычисление Cskip по формуле на странице 345 спец. ZigBee 2006 */
        if( nwkMaxRouters == 1 )
            cskip = 1 + ((uint16_t)nwkMaxChildren)*t;
        else {
            cskip = (int16_t)(1 + nwkMaxChildren - nwkMaxRouters 
                    - nwkMaxChildren*power(nwkMaxRouters,t))/(int16_t)(1 - nwkMaxRouters);
        }
        if( cskip == 0 ) {
            /* Нет адресов которые можно выделить */
            macAssociationPermit = FALSE;   /* см. стр. 345 строки 16-20 */
            return 0xffff;
        }
        /* Поиск адреса, не занятого ни одним из детей */
        if( dev_type == FFD ) { /* Полнофункциональное устройство */
            uint16_t i;
            s_addr_t raddr = 1 + nwkShortAddress;   /* Адрес первого возможного дочернего роутера */
            /* Проверяем последовательно все возможные адреса роутеров - не выделены ли они уже */
            for( i=0; i<nwkMaxRouters; i++ ) {
                if( !IN_TABLE( child_by_addr(raddr)))
                    return raddr;   /* Адрес не выделен. Возвращаем его */
                raddr += cskip;
            }
        } else {    /* Устройство с ограниченной функциональностью */
            uint16_t i;
            s_addr_t eaddr = 1 + nwkShortAddress + cskip*nwkMaxRouters; /* Адрес первого возможного 
                *   дочернего конечного узла */
            /* Перебираем все возможные адреса дочерних конечных узлов */
            for( i=0; i<(nwkMaxChildren-nwkMaxRouters); i++ ) {
                if( !IN_TABLE( child_by_addr(eaddr)))
                    return eaddr;   /* Адрес никому не выделен. Возвращаем его. */
                eaddr += 1;
            }
        }
        return 0xffff;
    } else {
        /* Адреса выделяются под контролем вышележащего уровня */
        if( 0 < nwkAvailableAddress ) {
            s_addr_t    new_addr = nwkNextAddress;
            --nwkAvailableAddress;
            nwkNextAddress += nwkAddressIncrement;
            return new_addr;
        }
        macAssociationPermit = FALSE;
        return 0xffff;
    }
}

