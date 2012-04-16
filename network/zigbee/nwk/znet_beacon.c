//  vim:encoding=utf-8
/*! @file  znet_beacon.c
 *  @brief Доступ к полям кадра-маяка
 *  @author     Max Gekk
 *  @date       октябрь 2007 г.
 *  @version    1   
 *      Модуль предназначен для доступа к полям кадра-маяка.        
 *  */

#include    <ztypes.h>
#include    <zigconf.h>
#include    <zmac_beacon.h>
#include    <znet_beacon.h>

/* Функция установки битов разрешения присоединения 
 * Параметры:
 * rtr_cap - 0 - сбросить флаг ROUTER CAPABILITY в 0, 1 установить в 1, иначе ничего не делать.
 * end_cap - 0 - сбросить флаг END DEVICE CAPABILITY в 0, 1 установить в 1, иначе ничего не делать.
 * */
static void    bcn_cap_set( uint8_t rtr_cap, uint8_t end_cap )
{
    uint8_t  bcn_payload[ Z_BCN_PAYLOAD_SIZE ];
    bcn_payload_get( bcn_payload );

    if( rtr_cap < 2 )   SET_ROUT_CAPACITY(bcn_payload, rtr_cap );
    if( end_cap < 2 )   SET_END_CAPACITY(bcn_payload, end_cap );

    bcn_payload_set( bcn_payload );

    return;
}

/* Сброс в кадре-маяке битов разрешения подключения роутеров и конечных устройств.*/
void    bcn_all_cap_off() 
{
    bcn_cap_set( 0,0 );
}

/* Сброс в кадре-маяке битов разрешения подключения роутеров либо конечных устройств.*/
void     bcn_cap_off( uint8_t dev_type )
{
    bcn_cap_set( dev_type == FFD? 0: 2 , dev_type == RFD? 0: 2 );
}

/* Установка в кадре-маяке битов разрешения подключения роутеров либо конечных устройств.*/
void    bcn_cap_on( uint8_t dev_type )
{
    bcn_cap_set( dev_type == FFD? 1: 2 , dev_type == RFD? 1: 2 );
}




