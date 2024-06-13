#include <stdio.h>
#include <stdlib.h>
#include <microkit.h>
#include <sddf/util/fence.h>
#include <sddf/util/util.h>

/* TODO: add debug print functions */

void ffiTHREAD_MEMORY_RELEASE(unsigned char *c, long clen, unsigned char *a, long alen) {
    _Static_assert(clen == 1 && alen == 1);
    THREAD_MEMORY_RELEASE();
}

void ffiASSERT(unsigned char *c, long clen, unsigned char *a, long alen) {
    assert(clen);
}

void ffimicrokit_notify(unsigned char *c, long clen, unsigned char *a, long alen) {
    microkit_notify(clen);
}

void ffimicrokit_irq_ack_delayed(unsigned char *c, long clen, unsigned char *a, long alen) {
    microkit_irq_ack_delayed(clen);
}

/* hardware related ffi (temporarily) housed here */
/* TODO: add detailed calling conventions below: */
/* c to contain ptr to tx/rx_ring to mutate; clen = 0 iff rx else 1 */
void ffiset_hw_regs(unsigned char *c, long clen, unsigned char *a, long alen) {
    /* *c contains register value to set, *a contains address of register */
    uint32_t val = *(uintptr_t *)c;
    *(uint32_t *)a = val;
}

void ffiget_hw_head(unsigned char *c, long clen, unsigned char *a, long alen) {
    /* c to contain ptr to tx/rx_ring to access */
    target_ring = (hw_ring_t *)c;
    /* Store result to a */
    *(unsigned int *)a = target_ring->head;
}

void ffiget_hw_tail(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    *(unsigned int *)a = target_ring->tail;
}

void ffiset_hw_head(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    c->head = (c->head + 1) % (clen ? TX_COUNT : RX_COUNT);
}

void ffiset_hw_tail(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    c->tail = (c->tail + 1) % (clen ? TX_COUNT : RX_COUNT);
}

/* Bad argument passing convention; fix this */
void ffiset_hw_meta(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    net_buff_desc_t buffer = *(net_buff_desc_t *)a;
    rx_ring->descr_mdata[rx_ring->tail] = buffer;
}

void ffihw_ring_full(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    *(uint64_t *)a = !((target_ring->tail - target_ring->head + 1) % (clen ? TX_COUNT : RX_COUNT));
}

void ffihw_ring_empty(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;
    *(uint64_t *)a = !((target_ring->tail - target_ring->head) % (clen ? TX_COUNT : RX_COUNT));
}

/* Needs redesign */
void ffiset_hw_ring_slot(unsigned char *c, long clen, unsigned char *a, long alen) {
    target_ring = (hw_ring_t *)c;

}
