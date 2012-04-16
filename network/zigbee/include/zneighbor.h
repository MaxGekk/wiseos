//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_NEIGHBOR_H
#define     _ZIGBEE_NETWORK_NEIGHBOR_H
/*! @file  zneighbor.h
 *  @brief Интерфейс таблицы соседей
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zigconf.h>

/*! @defgroup ZIGBEE_NETWORK_NEIGHBOR Таблица соседей
 *  @ingroup  ZIGBEE_NETWORK
 *  @{ 
 *      Таблица соседей содержит информацию о соседних узлах.
 * */

/* Максимальное число записей в таблице соседей */
#if !defined( Z_NEIGHBOR_TABLE_SIZE )
#define Z_NEIGHBOR_TABLE_SIZE 10
#endif

/* Отношения между соседними узлами */
#define     NWK_PARENT          0x00    /*!< Родитель */
#define     NWK_CHILD           0x01    /*!< Ребёнок */
#define     NWK_SIBLING         0x02    /*!< Брат */
#define     NWK_STRANGER        0x03    /*!< Чужой */
#define     NWK_PREVIOUS_CHILD  0x04    /*!< Бывший ребёнок */


/*! @typedef    neighbor_t
 *      Тип записи в таблице соседей
 * */
typedef struct {
    unsigned    busy                :1;     /*!< Флаг занятости записи в таблице: 1 - занято */
    unsigned    rx_on_when_idle     :1;     /*!< Включён ли приём при простое в CAP-e */
    unsigned    potential_parent    :1;     /*!< Если 1, то сосед может быть родителем, иначе нет */
    unsigned    permit_joining      :1;     /*!< Разрешена ли ассоциация */
    unsigned    dev_type            :2;     /*!< Тип соседа: 0 - коорд., 1 - маршрутизатор, 2 - конечное устройство */
    unsigned    batt_life_ext       :1;     /*!< Работает устройство от батареи или нет */
    unsigned    reserved            :1;
    unsigned    relationship        :3;     /*!< Отношение между данным устройством и соседом */
    unsigned    channel             :5;     /*!< Номер канала соседа */
    unsigned    beacon_order        :4;     
    unsigned    sf_order            :4;
    unsigned    depth               :4;     /*!< Глубина соседа в сети */
    unsigned    final_cap_slot      :4;     /*!< Номер последнего CAP-слота. Нумерация начинается с 0. */
    s_addr_t    net_addr;                   /*!< короткий ( 16-битный ) адрес*/
    s_panid_t   s_panid;
    e_addr_t    ext_addr;                   /*!< длинный ( 64-битный ) адрес */
    e_panid_t   e_panid;                    /*!< 64-битный уникальный идентификатор сети */
    uint32_t    beacon_timestamp;           /*!< Время приёма последнего бикона от соседа */
    uint32_t    beacon_offset;              /*!< Смещение бикона соседа относительно его родителя */
    uint8_t     lqi;                        /*!< Качество канала связи */
    uint8_t     tx_fail;                    /*!< Число неудачных попыток отправки */
} neighbor_t;

extern neighbor_t  nwkNeighborTable[ Z_NEIGHBOR_TABLE_SIZE ];

/*! @def AFTER_TABLE
 *  @brief Указатель на область памяти за таблицой соседей.
 * */
#define AFTER_TABLE   &nwkNeighborTable[ Z_NEIGHBOR_TABLE_SIZE ]

/*! @def IN_TABLE
 *  @brief Проверка того, что указатель указывает на запись в таблице соседей.
 * */
#define IN_TABLE( nbr_ptr ) ( nbr_ptr < AFTER_TABLE )

/*! @fn neighbor_t*     nbr_next( neighbor_t    *nbr_ptr );
 *  @brief  Функция возвращает указатель на следующую занятую запись в таблице соседей.
 *  @param  nbr_ptr - указатель на запись в таблице соседей или 0 если функция должна найти
 *  первую занятую запись.
 *  @return Указатель на следующую занятую запись в таблице соседей. Если никаких записей не было
 *  найдено, то функция возвращает указатель на область за концом таблицы соседей. Проверить то, что
 *  возращаемое значение указывает на записи таблицы можно с помощью макроса @a IN_TABLE.
 * */
neighbor_t*     nbr_next( neighbor_t    *nbr_ptr );

/*! @fn neighbor_t*     nbr_place_by_saddr( const s_panid_t s_panid, const s_addr_t  s_addr )
 *  @brief Размещение в таблице соседей записи с заданным коротким адресом и номером сети
 *  @param s_panid - идентификатор сети
 *  @param s_addr - короткий адрес
 *  @return Если найдена хоть одна запись с таким же коротким адресом и идентификатором сети, то возвращается указатель 
 *  на одну из таким записей ( остальные удаляются ). В противном случае возращается указатель на 
 *  свободную запись в таблице. Если таковой нет, то AFTER_TABLE. Выделенное место под запись 
 *  помечается как занятое.
 * */
neighbor_t*     nbr_place_by_saddr( const s_panid_t s_panid, const s_addr_t  s_addr );

/*! @fn neighbor_t*     nbr_place_by_eaddr( const e_addr_t  *const  e_addr_ptr )
 *  @brief Размещение в таблице соседей записи с заданным длинным адресом
 *  @param e_addr_ptr - указатель на длинный адрес
 *  @return Если найдена хоть одна запись с таким же длинным адресом, то возвращается указатель 
 *  на одну из таким записей ( остальные удаляются ). В противном случае возращается указатель на 
 *  свободную запись в таблице. Если таковой нет, то AFTER_TABLE. Выделенное место под запись 
 *  помечается как занятое.
 * */
neighbor_t*     nbr_place_by_eaddr( const e_addr_t  *const  e_addr_ptr );

/*! @fn  void    nbr_clean();
 *  @brief  Функция освобождает все записи в таблице соседей.
 * */
void    nbr_clean();

/*!  @}  */
#endif  /*  _ZIGBEE_NETWORK_NEIGHBOR_H */

