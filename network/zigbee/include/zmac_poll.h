//  vim:encoding=utf-8
#ifndef     _ZIGBEE_MAC_POLL_H
#define     _ZIGBEE_MAC_POLL_H
/*! @file  zmac_poll.h
 *  @brief Опрос координатора
 *  @author     Max Gekk
 *  @date       сентябрь 2007 г.
 *  @version    1           
 *  */

#include    <ztypes.h>
#include    <zneighbor.h>

/*! @defgroup ZIGBEE_MAC_POLL Отпрос координатора
 *  @ingroup  ZIGBEE_MAC
 *  @{ 
 * */

/* Имя стекового вызова для опроса координатора */
#define     ZCALL_MLME_POLL    0

/* Аргументы стекового вызова ZCALL_MLME_POLL */
typedef struct {
    neighbor_t  *coord_ptr;
    zstatus_t   status;
} zargs_mlme_poll_t;


/*! @} */
#endif      /* _ZIGBEE_MAC_RESET_H */

