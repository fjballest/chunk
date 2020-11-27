#ifndef _CHUNK_H
#define _CHUNK_H 1

#include "seg.h"                /* to define seg_t */

/* Information kept per chunk */
typedef struct chunk_st {
   seg_t    *seg;               /* owner responsible for this chunk */
   vaddr_t   vstart;            /* starting virtual address */
   
   int      num_hidden;         /* number of hidden chunks */
   struct chunk_st *chk_hidden; /* ptr to the first hidden chunk */
   struct chunk_st *chk_nxt;    /* ptr to the next hidden chunk  */
} chunk_t;

#define alloc_chunk()   ((chunk_t*)malloc(sizeof(chunk_t)))
#define free_chunk(c)   free(c)

/* Initialize the chunk store
 */
void chk_init(void);
/* Insert a new segment SEG starting at VSTART with length SIZE 
 * and return error status. 
 */
int chk_insert(vm_offset_t vstart, vm_size_t size, seg_t *seg);

/* Erase a previously defined segment starting at VSTART
 * return error status 
 */
int chk_delete(vm_offset_t vstart);
/* Lookup VADDR and return the segment where it lies.
 */
seg_t *chk_lookup(mv_offset_t vaddr);
#ifdef DEBUG
/* Dump the chunk store to file FP --for debug purposes
 */
void chk_dump(FILE *fp);
#endif
#endif 
