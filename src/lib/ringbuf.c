#include "ringbuf.h"

uint8_t rb_pindex = 0, rb_cindex = 0;
RB_TYPE *ringbuf[RB_SIZE];

void ringbuf_init()
{
    memset(ringbuf, 0, RB_SIZE * sizeof(RB_TYPE));
}

Ringbuf_Err_t ringbuf_produce(void *item)
{
    if (rb_pindex - rb_cindex >= RB_SIZE) return RINGBUF_ERR;

    ringbuf[rb_pindex % RB_SIZE] = item;
    rb_pindex++;

    if (rb_pindex == UINT8_MAX) {
        rb_pindex -= RB_SIZE;
        rb_cindex -= RB_SIZE;
    }

    return RINGBUF_OK;
}

Ringbuf_Err_t ringbuf_consume()
{
    if (rb_cindex >= rb_pindex) return RINGBUF_ERR;

    rb_cindex++;
    return RINGBUF_OK;
}

void ringbuf_access_item(void **item)
{
    (*item) = ringbuf[rb_cindex % RB_SIZE];
}
