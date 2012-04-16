//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_JOIN_H
#define     _ZIGBEE_NETWORK_JOIN_H
/*! @file  znet_join.h
 *  @brief Интерфейсы присоединения
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zmac_scan.h>
#include    <zmac_assoc.h>

/*! @defgroup ZIGBEE_NETWORK_PERMIT_JOIN Разрешение присоединения
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *  */

/* Имя стекового вызова, разрешающего присоединение дочерних узлов */
#define     ZCALL_NLME_PERMIT_JOIN  0

/* Аргументы стекового вызова ZCALL_NLME_PERMIT_JOIN */
typedef     struct {
    uint8_t     duration;   /*!< Вх. Интервал времени ( в секундах ), в течении которого дочерние узлы могут присоединяться.
                             *   Значения 0x00 и 0xff означают соответственно запрет и разрешение присоединения */
    zstatus_t   status;     /*!< Вых. При успешном выполнении вызова возвращается SUCCESS, иначе NWK_INVALID_REQUEST */
} zargs_pjoin_t;
/*! @}  */

/*! @defgroup ZIGBEE_NETWORK_DIRECT_JOIN Прямое присоединение
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *      Прямое присоедиение не предполагает передачу пакетов по радио-эфиру,
 *  а осуществляется непосредственным внесением информации об узле в таблицу
 *  соседей.
 * */

/* Имя стекового вызова для прямого присоединения */
#define     ZCALL_NLME_DIRECT_JOIN  1

/* Аргументы стекового вызова ZCALL_NLME_DIRECT_JOIN */
typedef     struct {
    e_addr_t    dev_addr;   /*!< Вх. Длинный адрес присоединяемого узла */
    cap_info_t  cap_info;   /*!< Вх. Информация о возможностях узла */
    zstatus_t   status;     /*!< Вых. Статус завершения операции прямого присоединения узла.
                                 Допустимые значения:
                                    - SUCCESS - узел успешно присоединён,
                                    - NWK_ALREADY_PRESENT - информация об узле уже есть в таблице соседей,
                                    - NWK_NEIGHBOR_TABLE_FULL - в таблице соседей нет места 
                             */
} zargs_djoin_t;
/*! @}  */

/*! @defgroup   ZIGBEE_NETWORK_AJOIN   Присоединение посредством ассоциации
 *  @ingroup    ZIGBEE_NETWORK
 *  @{
 * */

/* Имя стекового вызова присоединения посредством ассоциации на стороне дочернего узла */
#define     ZCALL_NLME_AJOIN    2

/* Аргументы стекового вызова ZCALL_NLME_AJOIN */
typedef     struct {
    e_panid_t   panid;      /*!< Вх. Расширенный PANID сети, к которой нужно подключиться */
    unsigned    join_as_router  :1;     /*!< Вх. Подключиться в качестве маршрутизатора:1-да, 0-нет */
    unsigned    power_src       :1;     /*!< Вх. Источник питания узла: 1-электросеть, 0-что-то другое*/
    unsigned    rx_on_when_idle :1;     /*!< Вх. Переходить ли в приём если ничем не занят: 1-да, 0-нет*/
    unsigned    mac_security    :1;     /*!< Вх. Включить шифрование на канальном уровне:1-да,0-нет */
    unsigned    have_net_key    :1;     /*!< Вых. */
    unsigned                    :3;
    s_addr_t    s_addr;     /*!< Вых. Короткий адрес, выделенный узлу */
    s_panid_t   s_panid;    /*!< Вых. PANID сети, к которой присоединился узел */
    zstatus_t   status;     /*!< Вых. Статус завершения операции присоединения посредством канальной ассоциации.
                                 Допустимы значения: 
                                    - SUCCESS - узел успешно присоединился
                                    - NWK_INVALID_REQUEST, NWK_NOT_PERMITTED, NWK_NO_NETWORKS,
                                    - статус завершения канального вызова  */
} zargs_ajoin_t;
/*! @}  */

/*! @defgroup   ZIGBEE_NETWORK_JOIN_INDICATION   Извещение о присоединение дочернего узла
 *  @ingroup    ZIGBEE_NETWORK
 *  @{
 * */

/* Имя стекового вызова извещающего о присоединении дочернего узла */
#define     ZCALL_NLME_JOIN_INDICATION  3

/*! @typedef zargs_ijoin_t
 *  @brief  Аргументы стекового вызова ZCALL_NLME_JOIN_INDICATION 
 *  @param  dev_addr - Длинный адрес присоединившегося узла
 *  @param  assoc_addr - Короткий адрес присоединившегося узла
 *  @param  cap_info - Информация о возможностях узла
 *  */
typedef     zargs_passoc_t  zargs_ijoin_t;

/*! @}  */

/*! @defgroup   ZIGBEE_NETWORK_OJOIN   Присоединение к потерянному родителю
 *  @ingroup    ZIGBEE_NETWORK
 *  @{
 * */

/* Имя стекового вызова для присоединения к потерянному узлу */
#define     ZCALL_NLME_OJOIN    4

/* Аргументы стекового вызова ZCALL_NLME_OJOIN */
/*! @typedef zargs_ojoin_t
 *  @brief  Аргументы стекового вызова ZCALL_NLME_OJOIN
 *  @param channels Вх. Битовая маская каналов, на которых нужно искать родителя. 
 *      27 бит (b0,b1,...,b26) соответсвуют каналам. Если соответсвующий
 *      бит установлен в 1, то канал должен быть просканирован, иначе 0 
 *      Вых. Битовая маска просканированных каналов 
 *  @param duration - Вх. Аргумент, определяющий продолжительность сканирования каждого
 *      канала. Продолжительность aBaseSuperframeOrder*(2^duration+1) 
 *  @param status - Вых. Статус завершения операции присоединения к потерянному родителю.
 *  */
typedef     zargs_oscan_t   zargs_ojoin_t;

/*! @}  */

#endif  /*  _ZIGBEE_NETWORK_JOIN_H   */

