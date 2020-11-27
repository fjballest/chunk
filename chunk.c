#include <assert.h>             /* for assertions */
#ifdef DEBUG
#include <stdio.h>      /* for fprintf */
#endif
#include "chunk.h"

#define CHK_MAX_SURF_ENT        255 /* Max # of surface entries */
   chunk_t chk_null_chunk_at_zero = { NULL, 
                                      (vaddr_t)0,
                                      NULL, NULL, 0};
   chunk_t chk_null_chunk_at_mmax = { NULL, 
                                      (vaddr_t)-1,
                                      NULL, NULL, 0};

static chunk_t *chk_surface[CHK_MAX_SURF_ENT]; /* The surface array */
static int      chk_surf_nents;                /* # of entries in surf.arry.*/

void chk_init(void)
{
   chk_surf_nents=0;
   chk_surface[chk_surf_nents++] = &chk_null_chunk_at_zero;
   chk_surface[chk_surf_nents++] = &chk_null_chunk_at_mmax;
}
int chk_insert(vaddr_t vstart, size_t size, seg_t *seg)
{
   
   vaddr_t vend = vstart + size;
   int sstart;            /* starting index for chunk in surface array*/
   int send;              /* ending index for chunk in surface array*/
   int  hstart;             /* first hidden chunk (or >hend) */
   int  hend;               /* last hidden chunk (or <hstart) */
    chunk_t *new_start;     /* points for the new chunk  */
    chunk_t *new_end;

   
   sstart = chk_surf_lookup(vstart);
   
   send = chk_surf_lookup(vend);
   
   hstart =  (vstart > chk_surface[sstart]->vstart)  
          ?  sstart+1           /* partly hidden: will hide the next */
          :  sstart;            /* hidden: this is the first hidden chunk*/
   hend = send;
   
    new_start = alloc_chunk();  /* create the two new points */
    new_end   = alloc_chunk();
   
    
    new_start->seg = seg;
    new_start->vstart = vstart;
    new_start->chk_hidden = new_start->chk_nxt = NULL;
    new_start->num_hidden = 0;
    new_end->seg = chk_surface[send]->seg;
    new_end->vstart = vstart + size;
    new_end->chk_hidden = new_end->chk_nxt = NULL;
    new_end->num_hidden = 0;
    
    if (hstart <= hend){                /* there're hidden chunks */
       int aux = hstart;
       new_start->num_hidden = hend - hstart + 1;
       new_start->chk_hidden = chk_surface[hstart];
       while( aux < hend ){
          chk_surface[aux]->chk_nxt = chk_surface[++aux];
       }
    }
    
    if ( new_start->num_hidden != 2 ){     /* we have to shift */
       memmove((void*)(chk_surface+hend + 1 + 2 - new_start->num_hidden), 
              (void*)(chk_surface+hend + 1), 
              sizeof(chk_surface[0]) * (chk_surf_nents - hend -1));
    }
    
    chk_surface[hstart] = new_start;
    chk_surface[hstart+1] = new_end;
    chk_surf_nents += 2 - new_start->num_hidden;
}/*chk_insert*/
/* Lookup VADDR and return the surface entry where it lies.
 */
int chk_surf_lookup(vaddr_t vaddr)
{
   int start = 0;
   int end   = chk_surf_nents-1;
   int aux;                     /* possibly here? */

   assert(start < end);         /* only works on non-empty surfaces */

   while (start < end) {
      aux = (start + end) / 2;
      if (vaddr == chk_surface[aux]->vstart) /* found */
         return aux;
      else if (vaddr > chk_surface[aux]->vstart)
         start = aux+1;
      else if (vaddr < chk_surface[aux]->vstart)
         end = aux-1;
   } 

   
   assert(( start && 
            chk_surface[start-1]->vstart   <= vaddr &&
            chk_surface[start]->vstart     > vaddr) ||
          ( chk_surface[start]->vstart   <= vaddr &&
            chk_surface[start+1]->vstart > vaddr));
   
   if (chk_surface[start]->vstart <= vaddr)
     return(start);
   else
     return(start - 1);
}/*chk_surf_lookup*/
/* Lookup VADDR and return the segment where it lies.
 */
seg_t *chk_lookup(vaddr_t vaddr)
{
   return(chk_surface[chk_surf_lookup(vaddr)]->seg);
}

/* Erase a previously defined segment starting at VSTART
 * return error status 
 */
int chk_delete(vaddr_t vstart)
{
    
    int      istart;            /* index for start chunk */
    chunk_t *cstart;            /* start chunk */
    chunk_t *aux;

    
    istart = chk_surf_lookup(vstart);
    cstart = chk_surface[istart];
    
    free_chunk(chk_surface[istart+1]); /* won't need it */

    if (cstart->num_hidden != 2){
       
       memmove((void*)(chk_surface + istart + cstart->num_hidden), 
              (void*)(chk_surface + istart + 2), 
              sizeof(chk_surface[0]) * (chk_surf_nents - istart +2));
       chk_surf_nents += cstart->num_hidden - 2;
    }
    
    aux=cstart;
    cstart=cstart->chk_hidden;
    free_chunk(aux);
    while(cstart!=NULL){
       chk_surface[istart] = cstart;
       cstart=cstart->chk_nxt;
       chk_surface[istart++]->chk_nxt = NULL;
    }
}/*chk_delete*/
#ifdef DEBUG
static void tabto(FILE *fp, int numtabs)
{
  while (numtabs--)
    fprintf(fp,"\t");
}
static void dump_one_chunk_tabbing(FILE *fp, int numtabs, chunk_t *c)
{
   assert(c!=NULL);

   fprintf(fp,"chunk = {\n");
   tabto(fp,numtabs); fprintf(fp,"  vstart=0x%x\n",c->vstart);
   tabto(fp,numtabs); fprintf(fp,"  seg at 0x%x\n",c->seg);
   tabto(fp,numtabs); fprintf(fp,"  next hidden at 0x%x\n",c->chk_nxt);
   tabto(fp,numtabs); fprintf(fp,"  hidden %d chunks ",c->num_hidden);

   if (c->num_hidden){    
     chunk_t *aux;
     int      hidden_count = 0;
     assert(c->chk_hidden != NULL);
     fprintf(fp," = {\n");
     aux = c->chk_hidden;
     while(aux != NULL){
        hidden_count++;
        tabto(fp,numtabs+1); dump_one_chunk_tabbing(fp,numtabs+1,aux);
        aux = aux->chk_nxt;
     }
     assert(hidden_count == c->num_hidden);
     tabto(fp,numtabs); fprintf(fp,"  }\n");
   }
   else {
     assert(c->chk_hidden == NULL);
     fprintf(fp,"\n");
   }
   tabto(fp,numtabs); fprintf(fp,"}\n");
}
/* Dump the chunk store to file FP --for debug purposes
 */
void chk_dump(FILE *fp){

  int ind,j;

  assert(chk_surf_nents >=2 && chk_surf_nents <= CHK_MAX_SURF_ENT);
  fprintf(fp,"chunk_t is %d bytes long\n\n",sizeof(chunk_t));
  fprintf(fp,"chunk store {\n\tnents=%d",chk_surf_nents);
  for (ind=0; ind < chk_surf_nents; ind++ ){
      assert(chk_surface[ind] != NULL);
      fprintf(fp,"\n\t[%2d] at 0x%x: ",ind, chk_surface[ind]);
      dump_one_chunk_tabbing(fp,1,chk_surface[ind]);
  }
  fprintf(fp,"} chunk store\n\n");

}
#endif
