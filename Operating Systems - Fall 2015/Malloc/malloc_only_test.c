#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *str;
    char *str_1;
    char *str_2;
    char *str_3;
    char *str_4;
    char *str_5;
    char *str_6;
    char *str_7;
    char *str_8;

    str = (char *) malloc(15);
    strcpy(str, "tutorialspoint");
    printf("String = %s,  Address = %p\n", str, str);
   
    // 16 is on the freeslist
   // myfree(str);
    
    // takes 8 
    str_1 = (char *) malloc(3);
    strcpy(str_1, "hi");
    printf("This is str_1: %p\n",str_1);

    // takes 8
    str_2 = (char *) malloc(4);
    strcpy(str_2, "him");
    printf("This is str_2: %p\n",str_2);
   
   // 8 is on the freelist 
   // myfree(str_2);
    //myfree(str_1);
     
    //printf("This is printing!\n");
    
    str_3 = (char *) malloc(5);
    strcpy(str_3, "him");
    printf("This is str_3: %p\n",str_3);
  
    //myfree(str_3);
   
   // printf("This is being printed!\n");
   // myfree(str_1);

    str_5 = (char*) malloc(5);
    strcpy(str_5, "hell");
    printf("This is str_1: %p\n",str_5);

    str_6 = (char*) malloc(6);
    strcpy(str_6, "hello");
    printf("This is str_2: %p\n",str_6);

    free(str_2);
    free(str_3);
    free(str);
   
    str_7 = (char*) malloc(3);
    strcpy(str_7,"he");
    printf("This is str_7: %s\n", str_7);
  
    free(str_6);
    free(str_1);
    free(str_7);
    free(str_5);
 
    str_8 = (char*) malloc(30);
    strcpy(str_8,"hefjdnswgfvnfkelgnrkefng");
    printf("This is str_8: %s\n", str_8);

    free(str_8);
    
    printf("Starting to insert 4!\n");
    str_4 = (char*) malloc(4);
    strcpy(str_4,"heuk");
    printf("This is str_4: %s\n",str_4);

    free(str_4);

    str_5 = (char*) malloc(3904);
    strcpy(str_5, "It is working");
    printf("This is str_5: %s\n",str_5);

    free(str_5);
}
