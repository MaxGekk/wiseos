//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_GROUP_H
#define     _ZIGBEE_NETWORK_GROUP_H
/*! @file  znet_group.h
 *  @brief Операции с группами
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup   ZIGBEE_NETWORK_GROUP Операции с группами узла
 *  @ingroup    ZIGBEE_NETWORK
 *  @{ */

/*! @fn result_t    zgroup_add( uint16_t    group_id );
 *  @brief Добавление нового идентификатора группы в список групп
 *  данного узла
 *  @param  group_id - идентификатор, добавляемой группы
 *  @return В случае успешного добавления группы возвращается ENOERR, иначе
 *      - ENOMEM - невозможно добавить группу из-за нехватки памяти
 * */
result_t    zgroup_add( uint16_t    group_id );

/*! @fn result_t    zgroup_del( uint16_t    group_id );
 *  @brief Удалить заданный идентификатор группы из списка групп
 *  @param group_id - идентификатор удаляемой группы
 *  @return В случае успешного удаления возвращается ENOERR, иначе
 *      - ENOTFOUND - идентификатор группы не обнаружен в списке групп
 * */
result_t    zgroup_del( uint16_t    group_id );

/*! @fn result_t    zgroup_find( uint16_t   group_id );
 *  @brief Поиск идентификатора группы в списке групп
 *  @param group_id - идентификатор искомой группы
 *  @return Если группа найдена, то возвращается ENOERR, иначе
 *      -   ENOTFOUND - группа не найдена
 * */
result_t    zgroup_find( uint16_t   group_id );

/*  @}  */
#endif  /*  _ZIGBEE_NETWORK_GROUP_H   */

