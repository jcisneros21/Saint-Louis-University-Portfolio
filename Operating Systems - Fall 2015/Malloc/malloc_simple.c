#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNK 1
extern void *mymalloc(size_t size);
extern void *myfree(void *ptr);


 main()
{
    void *a, *b, *c, *d, *e; 

      a = mymalloc(2*CHUNK);
        printf("  a     = %p\n",a);
          b = mymalloc(6*CHUNK);
            printf("  b     = %p\n",b);
              c = mymalloc(1*CHUNK);
                printf("  c     = %p\n",c);

                  myfree(a);

                    d = mymalloc(3*CHUNK);
                      printf("  d     = %p\n",d);

                        myfree(b);

                          e = mymalloc(3*CHUNK);
                            printf("  e     = %p\n",e);
}

