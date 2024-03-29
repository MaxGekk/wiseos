//  vim:encoding=utf-8
#ifndef     _ZIGBEE_NETWORK_RELATIVE_H
#define     _ZIGBEE_NETWORK_RELATIVE_H
/*! @file  znet_relative.h
 *  @brief Доступ к записям родственных узлов в таблице соседей
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <zneighbor.h>

/*! @defgroup ZIGBEE_NETWORK_PARENT Доступ к записи родителя
 *  @ingroup  ZIGBEE_NETWORK_NEIGHBOR
 *  @{ 
 * */

/*! @fn neighbor_t*     get_parent();
 *  @return Функция возвращается указатель на запись родителя в таблице соседей.
 *  Если таковая не была найдена, то возвращается указатель на AFTER_TABLE ( указатель
 *  на область памяти за таблицей соседей). Проверить то, что
 *  возращаемое значение указывает на записи таблицы можно с помощью макроса @a IN_TABLE.
 * */
neighbor_t*     get_parent();

/*!  @}  */

/*! @defgroup ZIGBEE_NETWORK_CHILD Доступ к дочерним записям
 *  @ingroup  ZIGBEE_NETWORK_NEIGHBOR
 *  @{ 
 *      Интерфейс позоволяет осуществлять операции связанные с
 *  дочерними узлами.
 * */

/*! @fn neighbor_t*     child_next( neighbor_t    *nbr_ptr );
 *  @brief  Функция возвращает указатель на следующую занятую дочернюю запись в таблице соседей.
 *  Дочерней считается запись, содержащая сведения о реальном ребёнке и бывшем ребёнке, за которым
 *  зарезервирован короткий адрес.
 *  @param  nbr_ptr - указатель на запись в таблице соседей или 0 если функция должна найти
 *  первую занятую запись. Начиная со следующей записи будет осуществляться поиск детей в таблице соседей.
 *  @return Указатель на следующую дочернюю запись в таблице соседей. Если никаких записей не было
 *  найдено, то функция возвращает указатель на область за концом таблицы соседей. Проверить то, что
 *  возращаемое значение указывает на записи таблицы можно с помощью макроса @a IN_TABLE.
 * */
neighbor_t*     child_next( neighbor_t    *nbr_ptr );

/*! @fn uint16_t    child_count();
 *  @return Текущее число дочерних записей в таблице соседей
 * */
uint16_t    child_count();

/*! @fn neighbor_t*     child_by_addr( s_addr_t  addr );
 *  @brief Поиск дочерней записи в таблице соседей по адресу
 *  @param addr - короткий адрес искомого дочернего узла
 *  @return Указатель на дочерний узел с адресом addr. Если такой узел не был найден, то 
 *  возвращается указатель на область за концом таблицы соседей.
 * */
neighbor_t*     child_by_addr( s_addr_t  addr );

/*! @fn neighbor_t*     child_by_eaddr( e_addr_t  addr );
 *  @brief Поиск дочерней записи в таблице соседей по длинному адресу
 *  @param addr - длинный адрес искомого дочернего узла
 *  @return Указатель на дочерний узел с адресом addr. Если такой узел не был найден, то 
 *  возвращается указатель на область за концом таблицы соседей.
 * */
neighbor_t*     child_by_eaddr( e_addr_t  addr );

/*!  @}  */
#endif  /*  _ZIGBEE_NETWORK_RELATIVE_H */

