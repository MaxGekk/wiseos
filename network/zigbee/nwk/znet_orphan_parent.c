//  vim:encoding=utf-8
/*! @file  znet_orphan_parent.c
 *  @brief Реализация присоединения потерянного узла
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1  
 *
 *      Данный модуль предоставляет реализацию восстановления потерянной связи 
 *  с одним из дочерних узлов. Подробности описаны в разделах 3.3.6, 3.7.1.3.3.3.
 *
 *  */

#include    <ztypes.h>
#include    <zcall.h>
#include    <zmac_orphan.h>
#include    <znet_relative.h>
#include    <znet_join.h>

ZCALL_USES( ZCALL_MAC_ORPHAN_PARENT );
ZCALL_USES( ZCALL_NLME_JOIN_INDICATION );

/* Окончание процедуры восстановления связи с узлом */
void    znet_orphan_done( zcall_t   *zc )
{
    zargs_porphan_t *parg = (zargs_porphan_t *)(zc->args);

    if( (parg->status == SUCCESS)&&(parg->saddr != 0xFFFF) )  {
        /* Ответ успешно отправлен нашему дочернему узлу */
        neighbor_t  *child_ptr;
        /* Повторно ищём его запись в таблице соседей.
         * TODO: Нужно соптимизировать и передавать указатель на запись в таблице соседей.
         *       Хотя в этом случае могут произойти всякие неприятные вещи. Например, после
         *       ответа узлу его запись удаляется и приходит уведомление об успешной отправке ему ответа. */
        child_ptr = child_by_eaddr( parg->dev_addr );
        if( IN_TABLE(child_ptr) ){
            /* Запись узла обнаружена в таблице соседей */
            zargs_ijoin_t *iarg = (zargs_ijoin_t *)(zc->args);
            /* Восстанавливаем отношение с узлом */
            child_ptr->relationship = NWK_CHILD;
            /* Формируем уведомление для вышележащего уровня */
            iarg->dev_addr = child_ptr->ext_addr, parg->saddr = child_ptr->net_addr;
            iarg->cap_info.alt_coord = 0;
            iarg->cap_info.dev_type = (child_ptr->dev_type == ZIGBEE_ENDDEV)?RFD:FFD;
            iarg->cap_info.pow_src = child_ptr->batt_life_ext;
            iarg->cap_info.rx_on_when_idle = child_ptr->rx_on_when_idle;
            iarg->cap_info.sec = 0;
            iarg->cap_info.alloc_addr = 1;

            ZCALL_INIT( zc, ZCALL_NLME_JOIN_INDICATION, 0);
            if( IS_OK(zcall_invoke(zc) ) )
                return;
        }
    }
    zcall_del(zc);
    return;
}

/* Принят запрос на восстановление связи */
void    znet_orphan_indication( zcall_t     *zc )
{
    zargs_iorphan_t *iarg = (zargs_iorphan_t *)(zc->args);
    zargs_porphan_t *parg = (zargs_porphan_t *)(zc->args);
    neighbor_t  *nbr_ptr;

    /* Проверяем, является ли узел, запросивший восстановление связи, нашим 
     * настоящим или бывшим ребёнком. */
    nbr_ptr = child_by_eaddr( iarg->dev_addr );
    if( IN_TABLE(nbr_ptr) ) {
        /* Узел является нашим ребёнком. Тут возможны 3 варианта:
         *  1. Узел является нашим настоящим ребёнком. В этом случае возвращаем его короткий адрес.
         *  2. Узел является нашим бывшим ребёнком, но ему выделен короткий адрес. В этом случае
         *     возвращаем узлу его короткий адрес и восстанавливаем отношения. 
         *  3. Узел является нашим бывшим ребёнком, но ему не выделен адрес. В этом случае в
         *     таблице соседей записан короткий адрес этого узла равный 0xFFFF. Этот адрес и
         *     возвращаем узлу. Что означает отказ в восстановлении связи. Пусть использует
         *     сетевое присоединение. */
        parg->saddr = nbr_ptr->net_addr;
    } else {
        /* Узел нам чужой. Поэтому никаких связей с ним нет и восстанавливать нечего. */
        parg->saddr = 0xFFFF;
    } 
    /* Отвечаем узла на запрос */   
    ZCALL_INIT( zc, ZCALL_MAC_ORPHAN_PARENT, znet_orphan_done );
    if( IS_OK(zcall_invoke(zc)) )
        return;
    zcall_del(zc);
    return;
}

ZCALL_PROVIDE( ZCALL_MAC_ORPHAN_INDICATION, znet_orphan_indication);

