
/* Test program for the chunk store -- link to chunk.o and compile
 * with DEBUG defined
 */

#include <stdio.h>
#include "chunk.h"

#define DBG(m)  fprintf(stderr,"---%s\n",m)

int main()
{
  
  DBG("Initializing the chunk store...");
  chk_init();
  DBG("Initialized. Now dumping it out");

  chk_dump(stderr);
  /*  -------------------------------------------------------------  */
  DBG("Inserting chunk 1");
  fprintf(stderr," chk_insert(0x100+0x100) -> %d\n", 
      chk_insert((vaddr_t)0x100,(size_t)0x100,(seg_t*)0x100));


  /*  -------------------------------------------------------------  */
  DBG("Inserting chunk 2");
  fprintf(stderr," chk_insert(0x150+0x150) -> %d\n", 
      chk_insert((vaddr_t)0x150,(size_t)0x150,(seg_t*)0x150));


  /*  -------------------------------------------------------------  */
  DBG("Inserting chunk 3");
  fprintf(stderr," chk_insert(0x125+0x50) -> %d\n", 
      chk_insert((vaddr_t)0x125,(size_t)0x50,(seg_t*)0x125));


  chk_dump(stderr);

  /*  -----------------------------------------------------------  */
  DBG("Inserting chunk 4");
  fprintf(stderr," chk_insert(0x350+0x100) -> %d\n", 
      chk_insert((vaddr_t)0x350,(size_t)0x100,(seg_t*)0x350));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Inserting chunk 5");
  fprintf(stderr," chk_insert(0x250+0x150) -> %d\n", 
      chk_insert((vaddr_t)0x250,(size_t)0x150,(seg_t*)0x250));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Inserting chunk 6");
  fprintf(stderr," chk_insert(0x300+0x50) -> %d\n", 
      chk_insert((vaddr_t)0x300,(size_t)0x50,(seg_t*)0x300));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Inserting chunk 7");
  fprintf(stderr," chk_insert(0x50+0x130) -> %d\n", 
      chk_insert((vaddr_t)0x50,(size_t)0x130,(seg_t*)0x50));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */

  DBG("Inserting chunk 8");
  fprintf(stderr," chk_insert(0x50+0x130) -> %d\n", 
      chk_insert((vaddr_t)0x50,(size_t)0x130,(seg_t*)0x55));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 8");
  fprintf(stderr," chk_delete(@0x50) -> %d\n", 
      chk_delete((vaddr_t)0x50));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 7");
  fprintf(stderr," chk_delete(@0x50) -> %d\n", 
      chk_delete((vaddr_t)0x50));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 6");
  fprintf(stderr," chk_delete(@0x300) -> %d\n", 
      chk_delete((vaddr_t)0x300));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 5");
  fprintf(stderr," chk_delete(@0x250) -> %d\n", 
      chk_delete((vaddr_t)0x250));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 4");
  fprintf(stderr," chk_delete(@0x350) -> %d\n", 
      chk_delete((vaddr_t)0x350));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 3");
  fprintf(stderr," chk_delete(@0x125) -> %d\n", 
      chk_delete((vaddr_t)0x125));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 2");
  fprintf(stderr," chk_delete(@0x150) -> %d\n", 
      chk_delete((vaddr_t)0x150));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
  DBG("Deleting chunk 1");
  fprintf(stderr," chk_delete(@0x100) -> %d\n", 
      chk_delete((vaddr_t)0x100));


  chk_dump(stderr);
  /*  -----------------------------------------------------------  */
  
DBG("done");
  return 0;
}
