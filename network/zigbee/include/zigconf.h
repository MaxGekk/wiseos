//  vim:encoding=utf-8
#ifndef     _ZIGBEE_CONFIG_H
#define     _ZIGBEE_CONFIG_H

/* Размер таблицы соседей ( в записях ) */
#define     Z_NEIGHBOR_TABLE_SIZE 10

/* Максимальное число выделенных пакетов */
#define     Z_MAX_PACKET 4

/* Максимальный размер аргументов стекового вызова */
#define     Z_CALL_MAX_ARGS_SIZE    16

/* Размер пула стековых вызовов */
#define     Z_CALL_POOL_SIZE    4

/* Максимальное число групп, в которые может входить узел */
#define     Z_TOTAL_GROUP   2

/* Максимально допустимый уровень энергии на канале */
#define     Z_MAX_CHANNEL_ENERGY    5

/* Размер полезной нагрузки кадра-маяка */
#define     Z_BCN_PAYLOAD_SIZE 	14

/* Слова должны быть выровнены по чётным адресам */
#define     Z_WORD_ALIGN    2

/* Стоимость пути до соседа не должна превышать этого значения.
 * См. спицификацию ZigBee 2006 раздел 3.7.1.3.2.1 */
#define     Z_MAX_LINK_COST 3

#endif  /*  _ZIGBEE_CONFIG_H  */

