#ifndef     _DATALINK_H
#define     _DATALINK_H
/*! @file datalink.h
 *  @brief Сервисы канального уровня стека SNAP
 *  @author     Max Gekk
 *  @author     Serge Pyptev 
 *  @date       январь 2007 г.
 *  @version    1.1
 
 *  @defgroup SNAP      Стек SNAP
 *  @defgroup SNAP_DL   Канальный уровень
 *  @ingroup  SNAP
 *  @{
 * */

#include    "mac.h"

/*! @typedef dl_neighbor_t 
 *  @brief Тип дескриптора соседнего узла
 * */
typedef     int16_t    dl_neighbor_t; 

#define     FIRST_NEIGHBOR     -1      /*!< Первый дескриптор в списке дескрипторов соседних узлов */
#define     LAST_NEIGHBOR      -2      /*!< Последний дескриптор в списке дескрипторов соседних узлов */

/*! @defgroup SNAP_DL_CONTROL   Сервис общего управления
 *  @ingroup SNAP_DL
 *  @{
 *      Данный сервис позволяет сетевому уровню производить общее управление 
 *  и контроль над канальным уровнем.
 * */

/*! @fn  dl_result_t     dl_start( const dl_node_id_t node_id,  const dl_size_t out_mem_size, const dl_size_t in_mem_size,
        const dl_size_t net_info_size, const dl_size_t packet_meta_size, const uint8_t tx_attempt_number );    
 *  Запуск и инициализация канального уровня.
 *
 *  @param node_id - уникальный идентификатор данного узла
 *  @param out_mem_size - размер области памяти, которую нужно зарезервировать для отправляемых пакетов
 *  @param in_mem_size - размер области памяти, которую нужно зарезервировать для принимаемых пакетов
 *  @param net_info_size – размер области памяти для сервисной информации сетевого уровня, связанной с соседним узлом
 *  @param packet_meta_size - размер области памяти для сервисной информации сетевого уровня, связанной с пакетом
 *  @param tx_attempt_number – число повторных попыток отправки пакета в случае неудачи первой попытки
 *  @result  
 *      - #DLE_NOERR
 * */
dl_result_t     dl_start( const dl_node_id_t node_id, 
        const dl_size_t out_mem_size, const dl_size_t in_mem_size,
        const dl_size_t net_info_size, const dl_size_t packet_meta_size,
        const uint8_t tx_attempt_number );    

/*! @fn void dl_start_done( dl_status_t status )
 *      Функция вызывается канальным уровнем. С помощью данной ф-ии 
 *  канальный уровень извещает об окончании процедуры запуска, инициированной dl_start().
 *  @param status - статус завершения запуска канального уровня. 
 * */
void dl_start_done( dl_status_t status );

/*! @fn dl_result_t dl_reset()
 *  Сброс текущего состояния канального уровня к состоянию, 
 *  в котором он был после последнего вызова примитива dl_start().
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t dl_reset();     

/*! @fn void dl_reset_done( dl_status_t status )
 *  Завершение процедуры сброса канального уровня, инициированной dl_reset().
 *  Функция вызывается канальным уровнем.
 *  @param status – статус завершения процедуры сброса подуровня.
 * */
void dl_reset_done( dl_status_t status );

/*! @fn dl_reset_t  dl_stop()
 *  Окончание работы канального уровня.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t  dl_stop();

/*! @fn void dl_stop_done( dl_status_t status )
 *  Функция вызывается канальным уровнем для извещения вышележащего уровня
 *  об окончании работы канального уровня.
 *  @param status – статус завершения процедуры окончания работы канального уровня. 
 * */
void dl_stop_done( dl_status_t status );

/*! @} */

/*! @defgroup SNAP_DL_NEIGHBOR  Сервис информирования о соседях
 *  @ingroup SNAP_DL
 *  @{
 *      Сервис информирования о соседях позволяет сетевому уровню получить 
 *  информацию о ближайших соседних узлах, то есть тех узлах, с которыми может быть 
 *  установлена непосредственная связь.
 * */

/*! @typedef    dl_evict_reason_t
 *  @brief Причина исключения узла из списка знакомых узлов. 
 * */
typedef enum {
    DLR_NET_DESIRE = 0      /*!<    По желанию сетевого уровня*/
} dl_evict_reason_t;

/*! @fn dl_result_t     dl_neighbor_search_enable()
 *  Вызов данной функции разрешает канальному уровню осуществлять поиск соседних узлов.
 *  @result  
 *      - #DLE_NOERR
 * */
dl_result_t     dl_neighbor_search_enable();

/*! @fn dl_result_t     dl_neighbor_search_disable()
 *  Вызов данной функции запрещает канальному уровню осуществлять поиск соседних узлов.
 *  @result  
 *      - #DLE_NOERR
 * */
dl_result_t     dl_neighbor_search_disable();

/*! @fn void dl_neighbor_found( const dl_neighbor_t neighbor )
 *  Данная функция вызывается канальным уровнем для уведомления о том, 
 *  что найден новый соседний узел и с этим узлом установлена связь.
 *  @param neighbor – дескриптор нового соседнего знакомого узла. 
 *  По этому дескриптору сетевой уровень может получить различную информацию о соседнем
 *  узле с помощью примитивов рассматриваемого сервиса.
 * */
void dl_neighbor_found( const dl_neighbor_t neighbor );

/*! @fn dl_result_t     dl_neighbor_id( const dl_neighbor_t neighbor, dl_node_id_t    *const node_id_ptr )
 *  Функция позволяет получить уникальный идентификатор соседнего узла.
 *  @param neighbor – дескриптор соседнего узла,
 *  @param node_id_ptr - указатель на область памяти, в которую, в случае успешного вызова, будет скопирован
 *      идентификатор знакомого узла.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_neighbor_id( const dl_neighbor_t neighbor, dl_node_id_t    *const node_id_ptr );

/*! @fn dl_result_t     dl_neighbor_net_info( const dl_neighbor_t neighbor, void  **const net_info_pptr )
 *  С помощью данной функции сетевой уровень может получить доступ к сервисным данным 
 *  о знакомом узле. Структура и семантика этих данных определяются сетевым уровнем.
 *  @param neighbor – дескриптор соседнего узла,
 *  @param net_info_pptr - указатель на область памяти, в которую, в случае успешного вызова, будет
 *  скопирован указатель на область памяти с сервисными данными сетевого уровня.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_neighbor_net_info( const dl_neighbor_t neighbor, void  **const net_info_pptr );

/*! @fn dl_result_t     dl_evict_neighbor( const dl_neighbor_t neighbor )
 *  Вызов функции позволяет сетевому уровню исключить соседний узел из списка знакомых узлов.
 *  @param neighbor – дескриптор соседнего узла,
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_evict_neighbor( const dl_neighbor_t neighbor );

/*! @fn void    dl_neighbor_evicted( dl_neighbor_t neighbor, dl_evict_reason_t reason )
 *  Функция вызывается канальным уровнем для извещения вышележащего уровня о том, 
 *  что после обработки этого вызова определённый соседний узел будет исключён из списка 
 *  знакомых узлов и с ним будет прекращена связь.
 *  @param neighbor – дескриптор соседнего узла,
 *  @param reason - причина исключения соседнего узла из списка знакомых узлов. 
 * */
void    dl_neighbor_evicted( dl_neighbor_t neighbor, dl_evict_reason_t reason );

/*! @fn dl_result_t     dl_next_neighbor( const dl_neighbor_t  neighbor, dl_neighbor_t  *const next_neighbor_ptr )
 *  Данная функция позволяет проводить перебор всех знакомых соседних узлов. 
 *  По переданному дескриптору знакомого узла примитив возвращает следующий дескриптор соседнего 
 *  узла из списка знакомых узлов. Упорядоченность узлов определяется канальным уровнем. 
 *  Существует два специальных дескриптора #FIRST_NEIGHBOR и #LAST_NEIGHBOR, которые 
 *  означают начало и конец списка знакомых узлов соответсвенно. При обнаружении нового соседнего узла 
 *  канальный уровень помещает его дескриптор в конец списка знакомых узлов.
 *  @param  neighbor - дескриптор соседнего узла,
 *  @param  next_neighbor_ptr - дескриптор следующего соседнего узла.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_next_neighbor( const dl_neighbor_t  neighbor, dl_neighbor_t  *const next_neighbor_ptr );

/*! @} */

/*! @defgroup SNAP_DL_CHANNEL  Сервис каналов связи
 *  @ingroup SNAP_DL
 *  @{
 *      С точки зрения сетевого уровня на канальном уровне между соседними узлами 
 *  существуют виртуальные каналы связи. Канал имеет две точки доступа ( channel 
 *  access point – CAP ). У узла может быть только один канал связи с соседним узлом. 
 *  Поэтому канал и точка доступа к этому каналу однозначно идентифицируются дескриптором 
 *  соседнего узла.  Каждый из узлов может поддерживать свою точку доступа в одном 
 *  из следующих состояний:
 *      - Активное состояние. По каналу узел передаёт и принимает данные.
 *      - Состояние передачи. Узел осуществляет только передачу пакетов.
 *      - Состояние приёма. Узел только принимает пакеты.
 *      - Состояние слежения. Не осуществляется ни приём, ни передача пакетов, 
 *          но возможен переход в один из вышеперечисленных режимов.
 *      - Пассивное состояние. Канал не поддерживается.
 *  Управлять и получать информацию о канале можно через рассматриваемый сервис.
 * */

/*! @typedef    dl_cap_state_t
 *  @brief Состояния точки доступа к каналу.
 * */
typedef     enum {
    DLC_PASSIVE = 0,    /*!< Пассивное состояние */
    DLC_ACTIVE  = 1,    /*!< Активное состояние */
    DLC_TX      = 2,    /*!< Состояние передачи */
    DLC_RX      = 3,    /*!< Состояние приёма   */
    DLC_TRACE   = 4     /*!< Состояние слежения */
} dl_cap_state_t;

/*! @fn dl_result_t     dl_set_cap_state( const dl_neighbor_t   neighbor, const dl_cap_state_t state )
 *  Изменение состояния точки доступа к каналу.
 *  @param  neighbor - дескриптор соседнего узла, с которым существует канал связи.
 *  @param  state - новое состояние точки доступа к каналу.
 *  @result 
 *      - #DLE_NOERR
 * */
dl_result_t     dl_set_cap_state( const dl_neighbor_t   neighbor, const dl_cap_state_t state );

/*! @fn dl_result_t     dl_get_cap_state( const dl_neighbor_t   neighbor, dl_cap_state_t    *const state_ptr )
 *  Функция позволяет узнать текущее состояние точки доступа к каналу на данном узле.
 *  @param  neighbor – дескриптор соседнего узла, с которым существует канал связи,
 *  @param  state_ptr - указатель на область памяти, в которую, в случае успешного вызова,
 *  будет скопировано состояние точки доступа к каналу.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_get_cap_state( const dl_neighbor_t   neighbor, dl_cap_state_t    *const state_ptr );

/*! @fn void     dl_cap_changed( const dl_neighbor_t    neighbor );
 *  Примитив вызывается канальным уровнем для уведомления вышележащего уровня об 
 *  изменении состояния точки доступа к каналу.
 *  @param neighbor - дескриптор соседнего узла.
 * */
void     dl_cap_changed( const dl_neighbor_t    neighbor );

/*! dl_result_t     dl_channel_quality( const dl_neighbor_t neighbor, dl_quality_t *const quality_ptr )
 *  Данная функция позволяет сетевому уровню узнать качество канала связи с 
 *  соседним знакомым узлом.
 *  @param neighbor - дескриптор соседнего узла,
 *  @param quality_ptr - указатель на область памяти, в которую будет скопировано состояние
 *  точки доступа к каналу связи.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_channel_quality( const dl_neighbor_t neighbor, dl_quality_t *const quality_ptr );

/*! @} */

/*! @defgroup SNAP_DL_PACKET  Сервис операций с пакетами
 *  @ingroup SNAP_DL
 *  @{
 *      Данный сервис позволяет сетевому и канальному уровню обмениваться данными 
 *  переменной длины, которые были получены или будут переданы соседним узлам.
 * */

/*! @typedef dl_packet_t
 *  @brief  Тип дескриптора пакета
 *  */
typedef int16_t    dl_packet_t;

#define     FIRST_PACKET    -1      /*!< Первый пакет в очереди отложенных пакетов */
#define     LAST_PACKET     -2      /*!< Последний пакет в очереди отложенных пакетов */

/*! @fn dl_result_t     dl_new_packet( const dl_size_t  payload_size, dl_packet_t packet )
 *  С помощью данной функции сетевой уровень резервирует память необходимого 
 *  размера на канальном уровне для пакета.
 *  @param  payload_size - размер полезной нагрузки пакета,
 *  @param  packet_ptr - указатель на область памяти, в которую в случае успешного вызова 
 *  будет скопирован дескриптор созданного пакета. По этому дескриптору сетевой 
 *  уровень может получить размер и доступ к области памяти пакета с помощью 
 *  примитивов рассматриваемого сервиса.
 *  @result
 *      - #DLE_NOERR
 *  */
dl_result_t     dl_new_packet( const dl_size_t  payload_size, dl_packet_t *const packet_ptr );     

/*! @fn dl_result_t     dl_destroy_packet( const dl_packet_t    packet )
 *  Уничтожить пакет, ранее созданный с помощью функции new_packet().
 *  @param  packet - дескриптор пакета.
 *  @result 
 *      - #DLE_NOERR
 * */
dl_result_t     dl_destroy_packet( const dl_packet_t    packet );

/*! @fn dl_result_t     dl_packet_size( const dl_packet_t packet, dl_size_t *const payload_size_ptr )
 *  Посредством данной функции сетевой уровень может узнать размер полезной нагрузки пакета.
 *  @param  packet - дескриптор пакета,
 *  @param  payload_size_ptr - указатель на область памяти, в которую, в случае успешного вызова данной
 *  функции, будет скопирован размер полезной нагрузки пакета.
 *  @result
 *      - DLE_NOERR
 * */
dl_result_t     dl_packet_size( const dl_packet_t packet, dl_size_t *const payload_size_ptr );

/*! @fn dl_result_t     dl_packet_payload( const dl_packet_t packet, void **const payload_pptr )
 *  Функция позволяет получить доступ к памяти с полезной нагрузкой пакета.
 *  @param  packet - дескриптор пакета.
 *  @param  payload_pptr - указатель на область памяти, в которую в свою очередь будет
 *  скопирован указатель на полезную нагрузку пакета.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_packet_payload( const dl_packet_t packet, void **const payload_pptr );

/*! @fn dl_result_t     dl_packet_meta_info( const dl_packet_t packet, void **const meta_info_pptr )
 *  Данный примитив предоставляет сетевому уровню доступ к мета-информации пакета. 
 *  Эта информация не передаётся вместе с пакетом соседним узлам, а используется сетевым уровнем 
 *  как сервисная по его усмотрению. Объём мета-информации одинаков для всех пакетов и задаётся при 
 *  запуске канального уровня.
 *  @param  packet - дескриптор пакета.
 *  @param  meta_info_pptr - указатель на область памяти, в которую в свою очередь будет
 *  скопирован указатель на мета-информацию пакета.
 *  @result
 *      - #DLE_NOERR
 * */
dl_result_t     dl_packet_meta_info( const dl_packet_t packet, void **const meta_info_pptr );

/*! @fn dl_result_t     dl_next_packet( const dl_packet_t   packet, dl_packet_t *const next_packet_ptr );
 *  Данная функция позволяет проводить перебор всех отложенных пакетов. 
 *  По переданному дескриптору отложенного пакета примитив возвращает следующий 
 *  дескриптор пакета из списка отложенных пакетов. Упорядоченность пакетов определяется 
 *  канальным уровнем. Существует два специальных дескриптора #FIRST_PACKET и #LAST_PACKET, 
 *  которые соответственно обозначают начало и конец списка отложенных пакетов. 
 *  @param  packet - дескриптор пакета.
 *  @param  next_packet_ptr - указатель на область памяти с дескриптором следующего пакета.
 * */
dl_result_t     dl_next_packet( const dl_packet_t   packet, dl_packet_t *const next_packet_ptr );

/*! @} */

/*! @defgroup SNAP_DL_TRANSMIT  Сервис передачи пакетов
 *  @ingroup SNAP_DL
 *  @{
 *      Данный сервис предназначен для передачи пакетов знакомым соседним узлам. 
 * */

/*! @typedef    dl_tx_status_t
 *  @brief  Статус завершения процедуры отправки пакета.
 * */
typedef enum {
    DLT_SUCCESS = 0
} dl_tx_status_t;

/*! @typedef    dl_tx_order_t
 *  @brief Порядок синхронизации с соседним узлом.
 * */
typedef enum {
    DLO_PRE_NEIGHBOR    = 0,    /*!< передача перед соседним узлом */
    DLO_AFTER_NEIGHBOR  = 1     /*!< передача после соседнего узла */
} dl_tx_order_t;

/*! @fn dl_result_t     dl_transmit( const dl_neighbor_t neighbor, const dl_packet_t  packet );
 *  Отправка соседнему узлу с дескриптором neighbor пакет с дескриптором packet.
 *  Вызов функции приводит к тому, что указанный пакет перемещается в очередь отправляемых 
 *  пакетов. Очередь отправляемых пакетов является очередью FIFO, поэтому канальный уровень 
 *  отправляет пакеты в том порядке в котором они находятся в очереди. В случае неуспешной 
 *  отправки пакета канальный уровень делает ещё несколько повторных повторных попыток. Число 
 *  этих попыток задаётся при запуске уровня. В случае успешной отправки или заданного количества 
 *  неуспешных отправок пакет удаляется из очереди отправляемых пакетов и становится отложенным.
 *  @param neighbor - дескриптор соседнего узла,
 *  @param packet - дескриптор отправляемого пакета.
 * */
dl_result_t     dl_transmit( const dl_neighbor_t neighbor, const dl_packet_t  packet );

/*! @fn void    dl_transmit_done( dl_neighbor_t neighbor, dl_packet_t packet, dl_tx_status_t status )
 *  Функция вызывается канальным уровнем для уведомления от том, что завершена процедура 
 *  отправки пакета. Перед вызовом канальный уровень удаляет пакет из очереди отправляемых и 
 *  пакет становится отложенным. 
 *  @param neighbor - дескриптор знакомого узла, которому должен был быть отправлен пакет,
 *  @param packet -  дескриптор пакета,
 *  @param status - статус завершения процедуры отправки.
 * */
void    dl_transmit_done( dl_neighbor_t neighbor, dl_packet_t packet, dl_tx_status_t status );  

/*! @fn dl_result_t dl_sync_transmission( const dl_neighbor_t neighbor, const dl_tx_order_t order )
 *  Синхронизовать передачу пакетов с определённым знакомым узлом.
 *  @param neighbor - дескриптор соседнего узла,
 *  @param order - порядок, в котором следуют передачи пакетов данного узла и 
 *  указанного знакомого узла. Первым передаёт данный узел, а потом знакомый узел, либо наоборот.
 * */
dl_result_t dl_sync_transmission( const dl_neighbor_t neighbor, const dl_tx_order_t order );

/*! @} */

/*! @defgroup SNAP_DL_RECEIVE  Сервис приёма пакетов
 *  @ingroup SNAP_DL
 *  @{
 *      Данный сервис позволяет сетевому уровню принимать пакеты от знакомых узлов, а также 
 *  управлять приёмом. Пакеты от знакомых узлов помещаются в очередь FIFO  принятых пакетов в 
 *  ожидании извещения о их приёме.
 * */

/*! @fn void    dl_receive( dl_neighbor_t   neighbor, dl_packet_t   packet );
 *  функция вызывается канальным уровнем для уведомления вышележащего уровня о том, 
 *  что принят пакет от знакомого узла.  Перед вызовом примитива пакет удаляется из очереди 
 *  принятых пакетов и становится отложенным.
 *  @param neighbor – дескриптор соседнего узла, от которого принят пакет,
 *  @param packet – дескриптор принятого пакета.
 * */
void    dl_receive( dl_neighbor_t   neighbor, dl_packet_t   packet );

/*! @} */

/*! @} */
#endif      /* _DATALINK_H */

