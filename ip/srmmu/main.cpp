//!PDC Cache test file

#include "page_descriptor_cache.H"


int main(void)
{
  page_descriptor_cache pdc("PDC");
  pdc.pdc_fill(0, 0xf0000000, 0x40000000);
  pdc.pdc_fill(0, 0xf0004000, 0x40004000);

  uint32_t paddr;

  int i = pdc.pdc_find(0, 0xf0000000, paddr);
  if(i == 0)
     fprintf(stderr, "PDC Cache Hit -> %x\n", paddr);
  else
     fprintf(stderr, "PDC Cache Miss\n");

  i = pdc.pdc_find(0, 0xf0004000, paddr);
  if(i == 0)
     fprintf(stderr, "PDC Cache Hit -> %x\n", paddr);
  else
     fprintf(stderr, "PDC Cache Miss\n");
  
  pdc.pdc_flush(4);

  i = pdc.pdc_find(0, 0xf0000000, paddr);
  if(i == 0)
     fprintf(stderr, "PDC Cache Hit -> %x\n", paddr);
  else
     fprintf(stderr, "PDC Cache Miss\n");

  i = pdc.pdc_find(0, 0xf0004000, paddr);
  if(i == 0)
     fprintf(stderr, "PDC Cache Hit -> %x\n", paddr);
  else
     fprintf(stderr, "PDC Cache Miss\n");
  return 0;
}
