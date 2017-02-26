#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{   
    int *nums = NULL;
    int full = 0;
    int comp = 4;
    int user_size = 0;
    int size = 0;
    int duplicate = 0;
    int current = 0;

    nums = (int *) malloc(sizeof(int));
	
    while(1)
    {
	if(comp == full)
        {
	   printf("The array is full! By how much would you like to increase the size of the array?  ");
	   scanf("%d", &user_size);
	   nums = (int *) realloc(nums, user_size * sizeof(int));
	   comp = comp + (user_size * sizeof(int));	
	}
	else
	{
	   printf("Please enter a number: ");
	   scanf("%d", &nums[current]);
	   if (argc == 1)
	   {
		int i;
		duplicate = 0;
		size = full/4;

		for(i=0;i < size; i++)
		{
		   if(nums[current] == nums[i])
		   {
			duplicate = 1;
		   }
		}
		if(duplicate == 1)
		{
		   printf("You entered a number that is the same from your previous inputs!\n");
		} 
		full = full + 4;
		current++;
	   }
	}
    }
}
