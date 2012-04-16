//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_ORPHAN_H
#define     _ZIGBEE_MAC_ORPHAN_H
/*! @file  zmac_orphan.h
 *  @brief Интерфейс извещения о найденном узле
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>

/*! @defgroup   ZIGBEE_MAC_ORPHAN_PARENT  Ответ потерянному узлу
 *  @ingroup    ZIGBEE_MAC
 *  @{
 * */

/*! @def  ZCALL_MAC_ORPHAN_PARENT
 *  @brief Имя стекового вызова-ответа потерянному узлу
 *  */
#define     ZCALL_MAC_ORPHAN_PARENT 112

/*! @typedef    zargs_porphan_t
 *  @brief  Аргументы вызова ZCALL_MAC_ORPHAN_PARENT
 * */
typedef     struct {
    e_addr_t    dev_addr;   /*!<  Вх. Длинный адрес потерянного узла */
    s_addr_t    saddr;      /*!<  Вх. Короткий адрес потерянного узла. Если это не наш дочерний
                                  узел, то аргумент равен 0xFFFF */
    zstatus_t   status;     /*!<  Вых. Статус завершения процедуры ответа потерянному узлу */
} zargs_porphan_t;
/*! @}  */

/*! @defgroup   ZIGBEE_MAC_ORPHAN_INDICATION  Извещение о потерянном узле
 *  @ingroup    ZIGBEE_MAC
 *  @{
 * */

/*! @def  ZCALL_MAC_ORPHAN_INDICATION
 *  @brief Имя стекового вызова для извещения о потерянном узле  
 *  */
#define     ZCALL_MAC_ORPHAN_INDICATION 113

/*! @typedef    zargs_iorphan_t
 *  @brief  Аргументы вызова ZCALL_MAC_ORPHAN_INDICATION
 *  @param dev_addr - длинный адрес потерянного узла
 * */
typedef     zargs_porphan_t     zargs_iorphan_t;
/*! @}  */

#endif  /*  _ZIGBEE_MAC_ORPHAN_H   */

