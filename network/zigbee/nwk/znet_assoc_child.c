//  vim:encoding=utf-8
/*! @file  znet_assoc_child.c
 *  @brief Присоединение дочернего узла через ассоциацию
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1
 *
 *      Реализация процедуры присоединения со стороны дочернего узла
 *  с использованием механизмов канальной ассоциации. Модуль предоставляет
 *  реализацию примитивов NLME-JOIN.request ( см. 3.3.6.1 ) и NLME-JOIN.confirm
 *  ( см. 3.3.6.3 ). Функциональное описание можно найти в разделе 3.7.1.3.1.1.        
 *  */

#include    <debug.h>
#include    <ztypes.h>
#include    <zigconf.h>
#include    <zcall.h>
#include    <zneighbor.h>
#include    <znet_join.h>
#include    <znet_attr.h>

ZCALL_USES( ZCALL_MLME_ASSOC_CHILD );

/* Вычисление стоимости пути до соседа на основе индикатора качества связи (LQI). 
 * Стоимость пути - это целое число из диапазона от 0 до 7 включительно. Описание
 * стоимости пути см.  в разделе 3.7.3.1.*/
uint8_t     link_cost( neighbor_t   *nbr_ptr )
{
    if( nwkReportConstantCost == TRUE ) 
        return 1;
    return (255-nbr_ptr->lqi)>>5; /* Отображение [0;255]->[7..0]*/
}

/* Поиск очередного потенциального родителя, к которому можно присоединиться */
neighbor_t* potential_parent( neighbor_t  *nbr_ptr, e_addr_t  *panid_ptr )
{
    neighbor_t  *parent_ptr = AFTER_TABLE;

    /* Перебираем все занятые записи в таблице соседей */
    for( nbr_ptr=nbr_next(nbr_ptr); IN_TABLE(nbr_ptr); nbr_ptr=nbr_next(nbr_ptr) ) {
        /* Если сосед не может быть потенциальным родителем или не разрешает к нему
         * присоединяться, то продолжаем поиск. */
        if( (nbr_ptr->potential_parent==0)||(nbr_ptr->permit_joining==0) )
            continue;
        /* Отбрасываем из узлы из других сетей */
        if( nbr_ptr->e_panid != *panid_ptr ) continue;
        /* Выбираем только те узлы, стоимость пути до которых не превышает максимального порога */
        if( Z_MAX_LINK_COST < link_cost(nbr_ptr) ) continue;
        /* Ранее уже был найден потенциальный родитель */
        if( IN_TABLE(parent_ptr) ) {
            /* Если глубина недавно найденного узла меньше, чем у потенциального родителя,
             * то этого узел становится нашим потенциальным родителем. */
            if( nbr_ptr->depth < parent_ptr->depth )
                parent_ptr = nbr_ptr;
        } else {
            parent_ptr = nbr_ptr; /* Первый узел, удовлетворяющий всем условиям, становится потенциальным родителем */
            /* Смотрим на глубину только при распределённой раздаче адресов. См. стр. 334. */
            if( nwkUseTreeAddrAlloc == FALSE )
                break;
        }
    }
    return parent_ptr;
}

/* Обработка уведомления о завершении процедуры канальной ассоциации */
void    znet_assoc_done( zcall_t    *mac_assoc )
{
    zcall_t     *zc = mac_assoc->assoc_call;
    neighbor_t  *nbr_ptr = ZARG( mac_assoc, zargs_cassoc_t, coord);

    ASSERT( ((neighbor_t *)0 == nbr_ptr)||!IN_TABLE(nbr_ptr), "Error: invalid neighbor pointer");

    if( ZARG(mac_assoc, zargs_cassoc_t, status) == SUCCESS ) {
        /* Процедура ассоциации завершилась успешно */
        nbr_ptr->relationship = NWK_PARENT; /* Оформляем отношения */
        nwkShortAddress = ZARG(mac_assoc, zargs_cassoc_t, assoc_addr );
        nwkExtendedPANID = nbr_ptr->e_panid;    /* Берём фамилию родителя ( длинный идентификатор сети родителя )*/
        ZARG(zc, zargs_ajoin_t, status) = SUCCESS;
        goto ajoin_done;
    }
    /* Не удалось присоединиться. Пробуем присоединиться к другому узлу. */
    nbr_ptr->potential_parent = 0;
    nbr_ptr = potential_parent(nbr_ptr, &ZARG(zc, zargs_ajoin_t, panid));
    if( !IN_TABLE(nbr_ptr) ) {
        /* Других потенциальных родителей нет */
        ZARG(zc, zargs_ajoin_t, status) = NWK_NOT_PERMITTED;
        goto ajoin_done;
    }
    /* Пробуем ещё раз ассоциироваться, но уже с другим узлом.*/
    ZARG(mac_assoc, zargs_cassoc_t, coord) = nbr_ptr;
    if( IS_OK(zcall_invoke(mac_assoc)) )
        return;
    ZARG(zc, zargs_ajoin_t, status) = NWK_INVALID_REQUEST;
ajoin_done:
    zcall_del( mac_assoc );
    zcall_return(zc);
    return;
}

/* Обработка запроса на присоединение со стороны вышележащего уровня */
void    znet_ajoin_child( zcall_t   *zc )
{
    zcall_t   *mac_assoc;
    neighbor_t  *nbr_ptr;

    /* Если мы не присоединены или нам не удалось создать новый вызов для канальной 
     * ассоциации, то возвращаем ошибку. */
    if( nwkExtraAttr.is_joined || ( (zcall_t *)0 == ( mac_assoc = zcall_new() ) ) ) {
        ZARG(zc, zargs_ajoin_t, status) = NWK_INVALID_REQUEST;
        zcall_return(zc);
        return;
    }
    /* Ищем первого потенциального родителя */
    nbr_ptr = potential_parent(0, &ZARG(zc, zargs_ajoin_t, panid));
    if( !IN_TABLE(nbr_ptr) ) {
        /* Потенциальных родителей в округе нет */
        ZARG(zc, zargs_ajoin_t, status) = NWK_NOT_PERMITTED;
        zcall_del( mac_assoc );
        zcall_return(zc);
        return;
    }

    /* Инициализируем стековый вызов для канальной ассоциации */
    ZCALL_INIT( mac_assoc, ZCALL_MLME_ASSOC_CHILD, znet_assoc_done );
    mac_assoc->assoc_call = zc;
    ZARG(mac_assoc, zargs_cassoc_t, coord) = nbr_ptr;

    /* Подробности заполнения поля с возможностями узла приведены в таблице 3.17 */
    nwkCapabilityInformation.alt_coord = 0;
    if( ZARG(zc,zargs_ajoin_t,join_as_router)==1 )
        nwkCapabilityInformation.dev_type = 1; /* Полнофункциональное устройство */
    else 
        nwkCapabilityInformation.dev_type = 0; /* Устройство с урезанной функциональностью */   
    nwkCapabilityInformation.pow_src = ZARG(zc,zargs_ajoin_t,power_src);
    nwkCapabilityInformation.rx_on_when_idle = ZARG(zc,zargs_ajoin_t,rx_on_when_idle);
    nwkCapabilityInformation.sec = ZARG(zc,zargs_ajoin_t,mac_security);
    nwkCapabilityInformation.alloc_addr = 1;

    ZARG(mac_assoc, zargs_cassoc_t, cap_info) = nwkCapabilityInformation;

    if( IS_OK(zcall_invoke(mac_assoc)) )
        return;
    zcall_del( mac_assoc );
    ZARG(zc, zargs_ajoin_t, status) = NWK_INVALID_REQUEST;
    zcall_return(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_NLME_AJOIN, znet_ajoin_child );

