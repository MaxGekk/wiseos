//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_ASSOC_H
#define     _ZIGBEE_MAC_ASSOC_H
/*! @file  zmac_assoc.h
 *  @brief Интерфейс канальной ассоциации
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zneighbor.h>

/*! @defgroup   ZIGBEE_MAC_ASSOC_CHILD   Ассоциация на стороне дочернего узла
 *  @ingroup    ZIGBEE_MAC
 *  @{
 * */

/* Имя стекового вызова для канальной ассоциации */
#define     ZCALL_MLME_ASSOC_CHILD   100

/* Аргументы вызова ZCALL_MLME_ASSOC_CHILD */
typedef     struct {
    neighbor_t  *coord;     /*!< Вх. Указатель на запись потенциального координатора в таблице соседей */
    cap_info_t  cap_info;   /*!< Вх. Информация о возможностях узла */
    s_addr_t    assoc_addr; /*!< Вых. Адрес, выделенный узлу после ассоциации с координатором */
    zstatus_t   status;     /*!< Вых. Статус завершения процедуры ассоциации.*/
} zargs_cassoc_t;

/*! @}  */

/* Аргументы стекового вызова ассоциации на стороне родителя */
typedef struct {
    e_addr_t    dev_addr;
    s_addr_t    assoc_addr;
    cap_info_t  cap_info;
    zstatus_t   status;
} zargs_passoc_t;

/*! @defgroup   ZIGBEE_MAC_ASSOC_INDICATION   Запрос ассоциации на стороне родительского узла
 *  @ingroup    ZIGBEE_MAC
 *  @{
 * */

/* Имя стекового вызова для извещения сетевого уровня о запросе ассоциации  */
#define     ZCALL_MLME_ASSOC_INDICATION   101

/*! @typedef zargs_iassoc_t
 *  @brief Аргументы стекового вызова ZCALL_MLME_ASSOC_INDICATION
 *  @param dev_addr - Вх. Длинный адрес узла, запросившего ассоциацию
 *  @param cap_info - Вх. Возможности этого узла
 *  */
typedef     zargs_passoc_t  zargs_iassoc_t;

/*! @}  */

/*! @defgroup   ZIGBEE_MAC_ASSOC_PARENT   Ассоциация на стороне родительского узла (ответ)
 *  @ingroup    ZIGBEE_MAC
 *  @{
 * */

/* Имя стекового вызова для канальной ассоциации на стороне родителя */
#define     ZCALL_MLME_ASSOC_PARENT   102

/*! @typedef    zargs_prassoc_t
 *  @brief Аргументы вызова ZCALL_MLME_ASSOC_PARENT 
 *  @param dev_addr - Вх. Длинный адрес узла, запросившего ассоциацию
 *  @param assoc_addr - Вх. Короткий адрес, выделенный узлу
 *  @param status - Вх.  Ответ-статус на запрос ассоциации. 
                    Вых. Статус завершения процедуры ассоциации.
 *  */
typedef     zargs_passoc_t  zargs_prassoc_t;

/*! @}  */

#endif  /*  _ZIGBEE_MAC_ASSOC_H   */

