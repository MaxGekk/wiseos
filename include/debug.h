#ifndef _DEBUG_H
#define _DEBUG_H

/*! @file  debug.h
 *  @brief Описание интерфейса debug
 *  @author     Max Gekk
 *  @author     Igor Pankratov
 *  @date       декабрь 2006
 *  @version    0.1     

 *  @defgroup debug Отладочная печать
 *  @ingroup  library
 *  @{ 
 *  @addtogroup debug
 *
 *      Отладочная печать включается определением макроконстанты DBGLOG.
 *  Отладочная печать осуществляется посредством форматируемого вывода.
 *  Строка форматирования состоит из:
 *      - символов, непосредственно печатаемых,
 *      - спицификаторов формата, которые определяют способ отображения аргументов.
 *        Спецификтор начинается со знака процента.
 *
 *  Допустимы следующие спецификаторы формата:
 *      - %%hhx - шестнадцатеричное без знака длиной 1 байт
 *      - %%x - шестнадцатеричное без знака длиной 2 байта
 *      - %%lx - шестнадцатеричное без знака длиной 4 байта
 *      - %%llx - шестнадцатеричное без знака длиной 8 байт
 *      - %%hhd - десятичное целое число со знаком длиной 1 байт
 *      - %%d - десятичное целое число со знаком длиной 2 байта
 *      - %%ld - десятичное целое число со знаком длиной 4 байта
 *      - %%lld - десятичное целое число со знаком длиной 8 байт
 * */

#if defined(DBGLOG)

#ifndef DBGLOG_LOCATION
/*! @def DBGLOG_LOCATION
 *  @brief Показывать ли номер строки и имя файла при отладочной печати
 *  */
#define DBGLOG_LOCATION
#endif

#include    <types.h>

result_t dbg_log(const uint16_t arg_size, const char *const fmt, ...);

#define     DEBUG_STRING(s) ({static const char __attribute__((__section__(".dbgstr"))) __is[] = s; __is; })

#if defined(DBGLOG_LOCATION)
#define     STRINGIFY(x) #x
#define     TOSTRING(x) STRINGIFY(x)
#define     AT __FILE__ ":" TOSTRING(__LINE__) " -> "
#else
#define     AT
#endif

#define     SIZEOF(a)   ((sizeof(a)<sizeof(int))?sizeof(int):sizeof(a))
#define     DBG_STR(fmt)    DEBUG_STRING( AT fmt )

#define     DBG(fmt)        dbg_log(0,DBG_STR(fmt))
#define     DBG1(fmt,a1)    dbg_log(SIZEOF(a1),DBG_STR(fmt), a1 )
#define     DBG2(fmt,a1,a2) dbg_log(SIZEOF(a1)+SIZEOF(a2), DBG_STR(fmt), a1,a2)
#define     DBG3(fmt,a1,a2,a3)      dbg_log(SIZEOF(a1)+SIZEOF(a2)+SIZEOF(a3), DBG_STR(fmt), a1,a2,a3)
#define     DBG4(fmt,a1,a2,a3,a4)   dbg_log(SIZEOF(a1)+SIZEOF(a2)+SIZEOF(a3)+SIZEOF(a4), DBG_STR(fmt), a1,a2,a3,a4)

#define     ASSERT(c,fmt)   if(c) {DBG(fmt);}
#define     ASSERT1(c,fmt,a1)   if(c) {DBG1(fmt,a1);}
#define     ASSERT2(c,fmt,a1,a2)   if(c) {DBG2(fmt,a1,a2);}
#define     ASSERT3(c,fmt,a1,a2,a3)   if(c) {DBG3(fmt,a1,a2,a3);}
#define     ASSERT4(c,fmt,a1,a2,a3,a4)   if(c) {DBG4(fmt,a1,a2,a3,a4);}

#else

#define     DBG_STR(s)
#define     DBG(fmt)
#define     DBG1(fmt,a1)
#define     DBG2(fmt,a1,a2)
#define     DBG3(fmt,a1,a2,a3)
#define     DBG4(fmt,a1,a2,a3,a4)

#define     ASSERT(c,fmt)   ((void)(0))
#define     ASSERT1(c,fmt,a1)   ((void)(0))
#define     ASSERT2(c,fmt,a1,a2)    ((void)(0))
#define     ASSERT3(c,fmt,a1,a2,a3)     ((void)(0))
#define     ASSERT4(c,fmt,a1,a2,a3,a4)      ((void)(0))

#endif                          /* DEBUG_LOG */

/*! @def DBG(fmt)
 *  @brief Отладочная печать строки @a fmt
 *  @param fmt - печатаемая строка
 * */

/*! @def DBG1(fmt,a1)
 *  @brief Форматированный вывод отладочной информации с одним аргументом
 *  @param fmt - управляющая строка
 *  @param a1 -  первый печатаемый аргумент
 * */

/*! @def DBG2(fmt,a1,a2)
 *  @brief Форматированный вывод отладочной информации с двумя аргументами
 *  @param fmt - управляющая строка
 *  @param a1 - первый печатаемый аргумент
 *  @param a2 - второй печатаемый аргумент
 * */

/*! @def DBG3(fmt,a1,a2,a3)
 *  @brief Форматированный вывод отладочной информации с тремя аргументами
 *  @param fmt - управляющая строка
 *  @param a1 - первый печатаемый аргумент
 *  @param a2 - второй печатаемый аргумент
 *  @param a3 - третий печатаемый аргумент
 * */

/*! @def DBG4(fmt,a1,a2,a3,a4)
 *  @brief Форматированный вывод отладочной информации с четырмя аргументами
 *  @param fmt - управляющая строка
 *  @param a1 - первый печатаемый аргумент
 *  @param a2 - второй печатаемый аргумент
 *  @param a3 - третий печатаемый аргумент
 *  @param a4 - четвёртый печатаемый аргумент
 * */

/*! @def ASSERT(c,fmt) 
 *  @brief Проверка утверждения. Если утверждение @a c ложно, то печатается строка fmt.
 *  @param c - проверяемое условие
 *  @param fmt - строка, выводимая на печать если условие @a c ложно.
 * */

/*! @def ASSERT1(c,fmt,a1) 
 *  @brief Проверка утверждения. Если утверждение @a c ложно, то производится форматируемый 
 *  вывод строки @a fmt с аргументом @a a1.
 *  @param c - проверяемое условие.
 *  @param fmt - строка, выводимая на печать если условие @a c ложно.
 *  @param a1 - первый аргумент.
 * */

/*! @def ASSERT2(c,fmt,a1,a2) 
 *  @brief Проверка утверждения. Если утверждение @a c ложно, то производится форматируемый 
 *  вывод строки @a fmt с аргументами @a a1, @a a2.
 *  @param c - проверяемое условие.
 *  @param fmt - строка, выводимая на печать если условие @a c ложно.
 *  @param a1 - первый аргумент.
 *  @param a2 - второй аргумент.
 * */

/*! @def ASSERT3(c,fmt,a1,a2,a3) 
 *  @brief Проверка утверждения. Если утверждение @a c ложно, то производится форматируемый 
 *  вывод строки @a fmt с аргументами @a a1, @a a2, @a a3.
 *  @param c - проверяемое условие.
 *  @param fmt - строка, выводимая на печать если условие @a c ложно.
 *  @param a1 - первый аргумент.
 *  @param a2 - второй аргумент.
 *  @param a3 - третий аргумент.
 * */

/*! @def ASSERT4(c,fmt,a1,a2,a3, a4) 
 *  @brief Проверка утверждения. Если утверждение @a c ложно, то производится форматируемый 
 *  вывод строки @a fmt с аргументами @a a1, @a a2, @a a3, @a a4.
 *  @param c - проверяемое условие.
 *  @param fmt - строка, выводимая на печать если условие @a c ложно.
 *  @param a1 - первый аргумент.
 *  @param a2 - второй аргумент.
 *  @param a3 - третий аргумент.
 *  @param a4 - четвёртый аргумент.
 * */

/*! @} */
#endif                          /* _DEBUG_H */
