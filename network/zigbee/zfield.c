//  vim:encoding=utf-8
/*! @file  zfield.c
 *  @brief Реализация доступа к полям фрейма
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <debug.h>
#include    <zfield.h>

/* Получение указателя на поле с заданным номером */
void*   zfield_find( field_t* fp, uint16_t num )
{
    ASSERT( (field_t *)0 == fp, "Error: field pointer is NULL");
    ASSERT( (field_info_t *)0 == fp->info_func, "Error: field is not initialized");

    /* Если во фрейме не удалось найти поле с заданным номером, то возвращаем 0 и выходим */
    if( 0 == (fp->ptr = fp->info_func( fp->frame, num, &(fp->len) )) )
        return (void *)0;

    /* Если по каким-либо причинам нельзя возвратить указатель непосредственно на поле 
     * во фрейме ( например, на узле не сетевой порядок байт ), то возвращаем указатель
     * на промежуточный буфер. */
    if( (ZNET_VAR_LEN != fp->len)&&( 1 < fp->len )
     &&( !IS_TARGET_LSB
#if Z_WORD_ALIGN == 2
         ||( (unsigned)(fp->ptr) & 1)
#endif 
       ) ) {
        fp->is_buf = 1;
        return (void *)(fp->buf);
    }

    fp->is_buf = 0;
    return fp->ptr;
}

/* Коопирование с переупорядочиванием принеобходимости */
void    reorder( void *from, uint8_t  from_offset,
        void *to,  uint8_t to_offset, 
        uint8_t len )
{
    int  i;
    if( IS_TARGET_LSB ) {
        for( i=0; i<len; i++ )
            OCTET( to, to_offset+i ) = OCTET( from, from_offset+i);
    } else {
        for( i=0; i<len; i++ )
            OCTET( to, to_offset+i ) = OCTET( from, from_offset+len-i-1 );
    }
}


/* Распаковка поля в промежуточный буфер, если это необходимо. */
result_t    zfield_unpack( field_t* fp )
{
    ASSERT( (field_t *)0 == fp, "Error: field pointer is NULL");

    /* До вызова этой функции должна быть вызвана функция zfield_find,
     * которая определяет нужно ли буферизовать поле или нет */
    if( fp->is_buf ) {
        /* Поле должно быть распаковано в промежуточный буфер */
        reorder( fp->ptr, 0, fp->buf, 0, fp->len );
    }
    return ENOERR;
}

/* Упаковка поля во фрейм */
result_t    zfield_pack( field_t* fp )
{
    ASSERT( (field_t *)0 == fp, "Error: field pointer is NULL");

    /* До вызова данной функции должна быть вызвана функция zfield_find,
     * определяющая необходимость буферизации поля. */
    if( fp->is_buf ) {
        /* Берём новое значение поля из промежуточного буфера */
        reorder( fp->buf, 0, fp->ptr, 0, fp->len );
    }
    return ENOERR;
}


