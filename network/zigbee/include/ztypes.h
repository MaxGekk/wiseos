//  vim:encoding=utf-8
#ifndef     _ZIGBEE_TYPES_H
#define     _ZIGBEE_TYPES_H
/*! @file  ztypes.h
 *  @brief Общие типы данных для всех уровней стека ZigBee
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <types.h>

/*! @defgroup ZIGBEE_TYPES Типы данных стека ZigBee
 *  @ingroup  ZIGBEE
 *  @{ 
 *      Типы данных, относящиеся ко всем уровням.
 * */

#define     IS_TARGET_LSB   ({ const uint8_t n[2] = {1,0}; ((*(uint16_t*)n) == 1); })

/* Режим адреса */
typedef     enum {
    ADDR_NOT_PRESENT    =   0x00,
    ADDR_RESERVED       =   0x01,
    SHORT_ADDR          =   0x02,
    EXT_ADDR            =   0x03
} addr_mode_t;

typedef     uint64_t    e_addr_t;   /*!< Расширенный адрес */
typedef     uint64_t    e_panid_t;  /*!< Расширенный идентификатор сети */

typedef     uint16_t    s_addr_t;   /*!< Короткий (сетевой) адрес*/
typedef     uint16_t    s_panid_t;  /*!< Идентификатор сети */
typedef     s_panid_t   panid_t;

typedef     enum {
    SUCCESS                 =   0x00,   /*!< Запрос был успешно выполнен */ 
    MAC_PAN_AT_CAPACITY     =   0x01,
    MAC_PAN_ACCESS_DENIED   =   0x02,
    NWK_INVALID_PARAMETER   =   0xc1,   /*!< Некорректный или выходящий за допустимый диапазон параметр был
                                         передан в примитив вышележащим уровнем */
    NWK_INVALID_REQUEST     =   0xc2,   /*!< Вышележащий уровнем произвёл вызов, который не может быть выполнен
                                         сетевым уровнем в текущем состоянии */
    NWK_NOT_PERMITTED       =   0xc3,   /*!< Вызов NLME-JOIN.request не разрешён */
    NWK_STARTUP_FAILURE     =   0xc4,   /*!< Вызов NLME-NETWORK-FORMATION.request не привёл к старту сети */
    NWK_ALREADY_PRESENT     =   0xc5,   /*!< Данные об узле, добавляемые посредством NLME-DIRECT-JOIN.request,
                                         уже присутствуют в таблице соседей */
    NWK_SYNC_FAILURE        =   0xc6,   /*!< Синхронизация завершилась неудачно */
    NWK_NEIGHBOR_TABLE_FULL =   0xc7,   /*!< В таблице соседей нет места для нового узла */
    NWK_UNKNOWN_DEVICE      =   0xc8,   /*!< Вызов NLME-LEAVE.request завершился неудачно, потому что 
                                         адрес устройства не найден в таблице соседей */
    NWK_UNSUPPORTED_ATTRIBUTE   =   0xc9,   /*!< В примитив NLME-GET.request или NLME-SET.request 
                                         был передан неизвестный идентификатор атрибута */
    NWK_NO_NETWORKS         =   0xca,   /*!< NLME-JOIN.request был вызван в окружении, где нет сетей */
    NWK_LEAVE_UNCONFIRMED   =   0xcb,   /*!< Подтверждение выхода из сети завершилось неудачно */
    NWK_MAX_FRM_CNTR        =   0xcc,   /*!< Шифрование завершилось неудачно, поскольку счётчик фреймов достиг 
                                         *   максимального значения */
    NWK_NO_KEY              =   0xcd,   /*!< Не найден ключ шифрования */
    NWK_BAD_CCM_OUTPUT      =   0xce,   /*!< Ошибочный вывод CCM */
    NWK_NO_ROUTING_CAPACITY =   0xcf,   /*!< Попытка обнаружить маршрут провалилась, так как недостаточно места 
                                         в таблице мпршрутизации или таблице поиска маршрутов */
    NWK_ROUTE_DISCOVERY_FAILED  =   0xd0,   /*!< Попытка обнаружить маршрут завершилась неудачно. */
    NWK_ROUTE_ERROR         =   0xd1,   /*!< Вызов NLDE-DATA.request завершился неудачно из-за того, что не был
                                         найден маршрут к узла назначения */
    MWK_BT_TABLE_FULL       =   0xd2,   /*!< Нет места в BTT */
    NWK_FRAME_NOT_BUFFERED  =   0xd3,   /*!< Фрейм был отброшен, так как маршрут в процессе установления */

    MAC_COUNTER_ERROR       =   0xdb,   /*!< Предположительно неправильный счётчик у принятого фрейма */
    MAC_IMPROPER_KEY_TYPE   =   0xdc,
    MAC_IMPROPER_SECURITY_LEVEL =   0xdd,
    MAC_UNSUPPORTED_LEGACY  =   0xde,
    MAC_UNSUPPORTED_SECURITY=   0xdf,    
    MAC_BEACON_LOSS         =   0xe0,	/*!< The beacon was lost following a synchronization request. */

    MAC_CHANNEL_ACCESS_FAILURE  =   0xe1,   /*!< Невозможна передача из-за активности на канале */
    MAC_DENIED              =   0xe2,   /*!< Запрос гарантированного тайм-слота был отвергнуть PAN координатором. */
    MAC_DISABLE_TRX_FAILURE     =   0xe3,   /*!< Попытка выключить радиочасть завершилась неудачно  */
    MAC_FAILED_SECURITY_CHECK   =   0xe4,   /*!< The received frame induces a failed security check according to the security suite. */
    MAC_FRAME_TOO_LONG      =   0xe5, 	/*!< После шифрования фрейм стал слишком длинным */

    MAC_INVALID_GTS         =   0xe6,	/*!< The requested GTS transmission failed because the specified GTS either did not have a transmit
                                         * GTS direction or was not defined. */
    MAC_INVALID_HANDLE      =   0xe7,   /*!< A request to purge an MSDU from the transaction queue was made using an MSDU handle that was not
                                         * found in the transaction table. */
    MAC_INVALID_PARAMETER   =   0xe8, 	/*!< Значение параметра выходит за допустимый диапазон. */
    MAC_NO_ACK              =   0xe9,   /*!< После aMaxFrameRetries попыток не было получено фрейма-подтверждения. */
    MAC_NO_BEACON           =   0xea,	/*!< В процессе сканирования не было найдено ни одного кадра-маяка. */
    MAC_NO_DATA             =   0xeb,	/*!< No response date were available following a request. */
    MAC_NO_SHORT_ADDRESS    =   0xec,	/*!< The operation failed because a short address was not allocated. */
    MAC_OUT_OF_CAP          =   0xed,	/*!< A receiver enable request was unsuccessful because it could not be completed within the CAP. */
    MAC_PAN_ID_CONFLICT     =   0xee,	/*!< A PAN identifier conflict has been detected and communicated to the PAN coordinator. */
    MAC_REALIGNMENT         =   0xef,	/*!< A coordinator realignment command has been received. */
    MAC_TRANSACTION_EXPIRED =   0xf0,   /*!< The transaction has expired and its information discarded. */
    MAC_TRANSACTION_OVERFLOW=   0xf1,	/*!< There is no capacity to store the transaction. */
    MAC_TX_ACTIVE           =   0xf2,	/*!< The transceiver was in the transmitter enabled state when the receiver was requested to be enabled. */
    MAC_UNAVAILABLE_KEY     =   0xf3,	/*!< The appropriate key is not available in the ACL. */
    MAC_UNSUPPORTED_ATTRIBUTE=  0xf4,	/*!< A SET/GET request was issued with the identifier of a PIB attribute that is not supported. */
    MAC_INVALID_ADDRESS     =   0xf5,
    MAC_ON_TIME_TOO_LONG    =   0xf6,
    MAC_PAST_TIME           =   0xf7,
    MAC_TRACKING_OFF        =   0xf8,
    MAC_INVALID_INDEX       =   0xf9,
    MAC_LIMIT_REACHED       =   0xfa,
    MAC_READ_ONLY           =   0xfb,
    MAC_SCAN_IN_PROGRESS    =   0xfc,
    MAC_SUPERFRAME_OVERLAP  =   0xfd,
} zstatus_t;

/* Информация о возможностях узла */
typedef struct {
   unsigned alt_coord       :1;     /*!< 1 - если узел может быть pan-координатором */
   unsigned dev_type        :1;     /*!< Тип узла: 1 - FFD, 0 - RFD */
   unsigned pow_src         :1;     /*!< Источник питания: 1 - от розетки */
   unsigned rx_on_when_idle     :1;     /*!< 1 - узел находится в режиме приёма когда ничего не делает */
   unsigned                 :2;
   unsigned sec             :1;     /*!< 1 - включена криптографическая защита на mac-уровне */
   unsigned alloc_addr      :1;     /*!< 1 - координатор может выделять адреса */
}  cap_info_t;

/* Типы ZigBee-узлов */
typedef  unsigned   znode_type_t;
#define     ZIGBEE_COORD    0x00    /*!< ZigBee-координатор */
#define     ZIGBEE_ROUTER   0x01    /*!< ZigBee-маршрутизатор */
#define     ZIGBEE_ENDDEV   0x02    /*!< конечное ZigBee-устройство */
#define     ZIGBEE_INVDEV   0x03    /*!< Некорректный тип узла */

#define     FFD     1
#define     RFD     0

/* Тип специфицирующий суперфрейм */
typedef     struct {
    unsigned    beacon_order    :4; /*!<  Поле, определяющее длину суперфрейма */
    unsigned    sf_order        :4; /*!<  Поле, определяющее длину активной части суперфрейма */
    unsigned    final_cap_slot  :4; /*!<  Номер последнего слота, используемого под CAP */
    unsigned    batt_life_ext   :1; /*!<  Расширенный режим работы от батареи */
    unsigned                    :1;
    unsigned    pan_coord       :1; /*!<  Является ли узел PAN-координатором */
    unsigned    assoc_permit    :1; /*!<  Разрешает ли узел подключаться к себе */
} sf_spec_t;

/*  @}  */
#endif  /*  _ZIGBEE_TYPES_H */

