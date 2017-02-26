/* Name:        Jessie Cisneros
   Date:        September 29, 2015
   Description: The purpose of this assignment was to retrieve three arguments from the user:
			- the number of threads the user wants
			- the keysize that correlates to the password
			- the hash that is used to retrieve the salt and compare to the crypt result
		After the user has enter these three arguments, the program then goes through all possible
		passwords of length keysize and then it either prints out the password or exits the program.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <crypt.h>

// sizeCompare is used to stop from being roll overed
char sizeCompare[][8] = {"z","zz","zzz","zzzz","zzzzz","zzzzzz","zzzzzzz","zzzzzzzz"};
char cycle[8]; // gloabl varible of all possible passwords
int keySize = 0; // length of password
char *hash;	// password hash
char salt[3];	// salt from the hash
int numThreads = 0; // number of threads
pthread_mutex_t lock; // global lock on threads
int flag = 1;

/* function increment:
*  The use of the this funtion is to increment the global
*  variable cycle each time until the check_all_keys function
*  is finished.
*/
int increment(char *word)
{
	int i = 0;
	
	// increments word
	for ( i = keySize-1; i >= 0; i--)
	{	
		if ( word[i] < 'z')
		{
			word[i] = word[i] + 1;
			return 0;
		}
		else
		{
			word[i] = 'a';
		}
		
	}	
}

/* function check_all_keys:
*  The use of this function is to check all possible keys
*  and create a hash of each password so we can compare the
*  user's hash and know what the password will be.
*/
void *check_all_keys(void* arg)
{
	// currently the password that is being worked on
	char current[8];
	
	// stores the crypt result
	char *compare;
	
	// creating a struct for crypt_r
	struct crypt_data data;
	data.initialized = 0;
	
	// run increment and crypt until either the password
	// has been found or until the all passwords are checked
	while(flag)
	{
		// unlock each thread after it copys and increments cycle
		pthread_mutex_lock(&lock);
		strcpy(current,cycle);
		increment(cycle);
		pthread_mutex_unlock(&lock);
		compare = crypt_r(current,salt,&data);
		// prints the password and exits
		if (strcmp(hash,compare) == 0)
		{
			printf("%s\n",current);
			exit(0);
		}
		// stop the process once cycle has reached the end
		if (strcmp(current,sizeCompare[keySize-1]) == 0)
		{
			flag = 0;
		}
	}

	return NULL;
}

int main(int argc, char *argv[]) 
{	
	// checks for 4 arguments
	if (argc != 4)
	{
		fprintf(stderr, "usage: crack threads keysize target\n");
		exit(0);
	}
	
	// Places all arguments into global variables	
	numThreads = atoi(argv[1]);
	keySize = atoi(argv[2]);
	hash = argv[3];
	
	// retrieves the salt from the hash
        salt[2] = '\0';
        salt[0] = hash[0];
        salt[1] = hash[1];


	// checks for the right amount of keysize
	if (keySize > 8 || keySize < 1)
	{
		fprintf(stderr, "usage:  crack threads keysize target\n\t0 < keysize <= 8\n");
		exit(0);
	}
	
	// checks for the right number of threads
	if (numThreads > 32 || numThreads < 1)
	{
		fprintf(stderr, "usage:  crack threads keysize target\n\t1 <= threads <= 32\n");
		exit(0);
	}
	
	// checks to see if hash is of length 13	
	if ( strlen(hash) != 13 )
	{
		fprintf(stderr, "usage:  crack threads keysize target\n\ttarget form is incorrect, needs to be 13 characters\n");
                exit(0);
	}
	
	// places 'a' each time in the array until keysize is reach 
	int i;
	for( i = 0; i < keySize; i++)
	{		
	     cycle[i] = 'a';
	}
	cycle[keySize] = '\0';
	
	// creates an array of pthreads
	pthread_t check_thread[numThreads];
	
	// intialize the mutex lock
	pthread_mutex_init(&lock,NULL);
	
	// create number of threads depending on what the number of threads is
	for ( i = 0; i < numThreads; i++)
	{
		pthread_create(&check_thread[i],NULL,check_all_keys,NULL);		
	}	
	
	// joins all the threads
	for ( i = 0; i < numThreads; i++)
	{
		pthread_join(check_thread[i],NULL);
	}
	
	// exit all threads
	pthread_exit(check_thread);

	return 0;
}
