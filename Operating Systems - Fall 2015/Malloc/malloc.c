/* Name:            Jessie Cisneros
 * Date:            November 5, 2015
 * Description      The purpose of this assignment is to learn memory allocation by creating 
 *                  our own malloc, free, realloc, and calloc. We then link this file with any file
 *                  that uses these functions to allow for memory allocation. When a user first calls 
 *                  malloc, it provides memory of mutlple of eight to the user and then returns the 
 *                  rest of the pagesize memory that it used to the freelist where we can use it over
 *                  and over again until the memory is inefficient and then we have to sbrk again..
*/
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void free(void *ptr);

// Block Structure for memory
typedef struct Block
{
   size_t size;
   struct Block *next;
} Block;

// Global freelist for request memory
Block *freelist = NULL;

/* Function multiple_of_eight:
 * Rounds out the user's requested size.
*/

size_t multiple_of_eight(size_t size)
{
    while( (size % 8) != 0 )
    {
       size++;
    }
    return size;
}

/* Function get_block_ptr:
 * Returns the block of the ptr.
*/
Block *get_block_ptr(void *ptr)
{
  return (Block*)ptr - 1;
}

/* Function malloc:
 * Malloc is used to destribute memory to the user by calling
 * sbrk to increase the heap and then keeps track of how much
 * memory is being released. We can then reuse any memory on 
 * freelist if it is acceptable size.
*/

void *malloc(size_t size)
{ 
   // used for sbrk
   Block *block;
   // the page size block that we will be contanstly split
   // and free since it has the largest address space
   Block *page_block;
   // points to previous and current block on the freelist
   Block *prev;
   Block *curr;
   // the request we make through sbrk
   void* request;
   // the number of pages that we will need
   int num_of_pages;
   // the number that represents page size
   int page_size = sysconf(_SC_PAGESIZE);
   // if nothing on the freelist in compatible with the 
   // request, turn to 1
   int checked_list = 0;
   // user's new argument size
   size_t arg_size = multiple_of_eight(size);
   int right_size = arg_size + sizeof(Block);
   
   // checks for sizes lower than zero
   if (size < 0)
   {
     return NULL;
   }
   
  // printf("This is the user's size: %ld\n", size);
   while(1)
   {
     // checks if there is nothing in the freelist or if nothing on the freelist is compatible
     if ((freelist == NULL) || (checked_list == 1))
     {
      // get the address of where we are breaking
       block = sbrk(0);
       // right size is 4080
       num_of_pages = (right_size + sizeof(Block)) / page_size;
       num_of_pages++;
       
       request = sbrk(page_size * num_of_pages); 
       block->size = num_of_pages * page_size;
       // create new address onto a new block
       page_block = (Block*)((char*)(block+1)+arg_size);
       // the size will become the right amount of memory
       page_block->size = block->size - right_size - sizeof(Block);
       // then we free page_block
       free(page_block + 1);
       // and return block which is the user requested block
       block->size = arg_size;
       block->next = NULL;
      
      // error checking for sbrk
      if ( request == (void *) -1)
      {
         return NULL;
      }
      else
      {
         return (block+1);
      }
    }
    else
    {
     // prev and curr will now point to the head of the freelist
     prev = freelist;
     curr = freelist;

     int lowest_size = arg_size + sizeof(Block);
     // goes through the whole linked list
     while(curr != NULL)
     {
       //if we find a big enough block we split it
       if(lowest_size < curr->size)
       {
         // if the found block is the head of the freelist
         // take it off and make freelist NULL
         if ((curr == freelist))
         {
           freelist = curr->next;
           curr->next = NULL;
         }
         // else point the previous block to current's next
         // so we can take current off the freelist
         else
         {
           prev->next = curr->next;
           curr->next = NULL;
         }
         
         // Beginning splitting the block
         page_block = (Block*)((char*)(curr+1)+arg_size);
         page_block->size = curr->size - lowest_size;
         curr->size = arg_size;

         // free the larger block
         free(page_block+1);
         // return the smaller block to the user
         return (curr+1);
       }
       // continue to the next block on the freelist
       prev = curr;
       curr = curr->next;
     }
     // if there is no suitable block, it will back around
     checked_list = 1;
    }
  }   
}

/* Fuction free:
 * When the user calls free, the function turns
 * free into a block and then puts that block on the
 * freelist, so we may use it again later.
*/

void free(void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }
  Block* curr;  
  curr = get_block_ptr(ptr);
  curr->next = freelist;
  freelist = curr;
}

/*Function calloc:
 * Allocates the requested memory and returns
 * a pointer to that memory.
 * num_of_elts - the number of elements to be allocated
 * size - the size of the elements
*/

void *calloc(size_t num_of_elts, size_t elt_size)
{
  size_t size  = num_of_elts * elt_size;
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}

/*Function realloc:
 * Creates a new allocation, copies as much of the old
 * allocation, frees the old allocation, and returns a
 * pointer to the allocated memory by calling malloc. We
 * then free the pointer that the user requested.
*/
void *realloc(void *pointer, size_t size)
{
  // if the pointer is NULL
  if(pointer == NULL)
  {
    return malloc(size);
  }

  // error check for a size less than zero
  if(size < 0)
  {
    return pointer;
  }
  
  // return the block in the pointer
  Block *check = get_block_ptr(pointer);
  // checks to see if the pointer block size
  // is less than or equal to the user requested size
  if (check->size >= size)
  {
    return pointer;
  }
  

  void *new_ptr;
  new_ptr = malloc(size);
  if (new_ptr == NULL)
  {
    return NULL;
  }
  memcpy(new_ptr, pointer, check->size);
  free(pointer);
  
  return new_ptr;
}
