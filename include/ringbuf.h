#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <string.h>

#ifdef TESTING
    #define RB_SIZE 2
#else 
    #define RB_SIZE 32
#endif

#if __SIZEOF_POINTER__ == 4
    #define RB_TYPE uint32_t
#else
    #define RB_TYPE uint32_t
#endif

typedef enum {
    RINGBUF_OK = 0,
    RINGBUF_ERR = 1,
} Ringbuf_Err_t;

/** 
 * pindex = last not produced or first to produce
 * cindex = last not consumed or first to consume
 */
extern uint8_t rb_pindex, rb_cindex;

void ringbuf_init();
Ringbuf_Err_t ringbuf_produce(void *item);
Ringbuf_Err_t ringbuf_consume();
void ringbuf_access_item(void **item);

#endif /** RINGBUF_H */
