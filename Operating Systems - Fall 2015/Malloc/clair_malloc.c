/*
 *  * malloc_test.c
 *   *    Allocate bytes as requested by user.
 *    *    Print out the address and the value of the break.
 *     */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void *mymalloc(size_t size);

main()
{
    void *mem = NULL;
    unsigned long size;
    char line[256];

    do {
      printf("\n  sbrk(0) = %p\n",sbrk(0));

      printf("How much memory? ");
      fgets(line,255,stdin);
      size = strtoul(line,NULL,0);

      mem = mymalloc(size);
      printf("  mem     = %p\n",mem);

    } while (mem != NULL);

    perror("malloc");
}

