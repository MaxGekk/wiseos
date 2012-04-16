#ifndef     _OS_H
#define     _OS_H

/*! @file   os.h 
 *  @brief  Интерфейсы WiseOS
 *  @date   2006-2007 гг.
 *  @version    0.1
 *  @author     Max Gekk
 *  @author     Igor Pankratov
 *
 *  @mainpage Описание WiseOS
 *
 *      WiseOS - операционная система для узлов сенсорных сетей. 
 *  При проектировании WiseOS основными требованиями являются 
 *  достижение энергетической эффективности и создание абстракции 
 *  системных вызовов удобных для реализации сетевых стеков для
 *  сенсорных сетей. 
 *
 *      Разработка ОС ведётся на языке Си. Такой выбор обусловлен
 *  широкой распространённостью языка и удобством разработки 
 *  встроенных систем на нём.
 *          
 * */

#include    <types.h>
#include    <init.h>
#include    <task.h>
#include    <critical.h>

#include    <combuf.h>
#include    <debug.h>

#endif                          /* _OS_H */
