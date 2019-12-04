#ifndef SAFE_MEM_H
#define SAFE_MEM_H

// Destroy memory byte-by-byte
void mem_erase(void* ptr)
{
  if (ptr == NULL) return;
  char* ptcpy = (char*)ptr;
  while(*ptcpy)
  {
    *ptcpy++ = 0x00;
  }
}

void sfree(void* ptr){ if(ptr != NULL){free(ptr); mem_erase(ptr); ptr = NULL;} }

#endif
