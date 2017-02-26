/* Name:        Jessie Cisneros
 * Date:        November 26, 2015
 * Description: The purpose of this assignment was for the user to enter a path
 *              directory on the command line or just run the program by itself to
 *              indicate the directory the user is in and print out a description 
 *              of each file in the directory. The details of each file include:
 *              - the file name
 *              - the owner's name
 *              - the group's name
 *              - the permissions for the file
 *              - the byte size
 *              - the block size
 *              - last access
 *              - last modified
 *              - last changed
 *              The user is able to enter multiple path directories into the command
 *              line as well. We then print out the information above to the user for
 *              each request.
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>

/* Function: print_details
 * This function's main purpose is to take a file's
 * name and print out all required information
 * about it.
*/
void print_details(char* file)
{
   struct stat sb;
   struct passwd *pwd;
   struct group *grp;
   
   // creates the stat struct for a file
   if (lstat(file, &sb) == -1)
   {
       perror("error: ");
   }
   
   // prints the file's name
   printf("%s\t", file);
   
   // prints out the file type
   switch (sb.st_mode & S_IFMT) {
     case S_IFDIR:  printf("(directory)");               break;
     case S_IFCHR:  printf("(character special)");       break;
     case S_IFBLK:  printf("(block device)");            break;
     case S_IFLNK:  printf("(symbolic link)");           break;
     case S_IFIFO:  printf("(named pipe)");              break;
     case S_IFSOCK: printf("(socket)");                  break;
   }
   printf("\n");
   
   // prints out the owner's name
   if ((pwd = getpwuid(sb.st_uid)) != NULL)
   {
       printf("\towner: %s\t", pwd->pw_name);
   }
   else
   {
       printf("\towner: %d\t", sb.st_uid);
   }
  
   // prints out the groups name
   if ((grp = getgrgid(sb.st_gid)) != NULL)
   {
       printf("group:%s\t", grp->gr_name);
   }
   else
   {
       printf("group:%d\t", sb.st_gid);
   }
   
   // prints out the file's permissions
   printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
   printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
   printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
   printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
   printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
   printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
   printf( (sb.st_mode & S_IROTH) ? "r" : "-");
   printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
   printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
   printf("\n");
   
   printf("\t%ld bytes, %ld blocks.\n", (long) sb.st_size, (long) sb.st_blocks);
   printf("\tlast access:              %s", ctime(&sb.st_atime));
   printf("\tlast modification:        %s", ctime(&sb.st_mtime));
   printf("\tlast file status change:  %s", ctime(&sb.st_ctime));
   printf("\n");  
}

/* Function: main
 * Takes in the command line arguments and opens
 * streams for each path name. Afterwords, it
 * relies on print_details to print out each file
 * in the stream.
*/
int main(int argc, char* argv[])
{
   struct dirent *pDirent;
   DIR *pDir;
   char *cwd;
   int i;
   int badDir = 0;
   char buff[PATH_MAX + 1];

   if (argc > 1)
   {
      cwd = getcwd(NULL, 0);
      printf("%s\n",cwd);
      for (i = 1; i < argc; i++)
      {
          // print headers for more than two directories
          if (argc > 2)
          {
              printf("** %s **\n",argv[i]);
          }
          
          badDir = 0;
          // named directory becomes the current directory
          if (chdir(argv[i]) == -1)
          {
              perror(argv[i]);
              badDir = 1;
          }
          
          // if the file name does exist
          if ( badDir == 0 )
          {  
              // opens the the current directory stream
              pDir = opendir(".");
              if ( pDir == NULL )
              {
                   perror(argv[i]);
                   exit(0);
              }
              
              // goes through each file in the directory
              while ((pDirent = readdir(pDir)) != NULL )
              {   
                   print_details(pDirent->d_name);
              }   
              
              // close the current directory stream
              if (closedir(pDir) == -1) 
              {   
                   perror("close");
                   exit(0);
              }
          }
          // make the directory the user is running the file,
          // the current working directory 
          if (chdir(cwd) == -1)
          { 
              perror(cwd);
          }
       }

      free(cwd);
   }
   else
   {
      // copies the current path directory into memory
      cwd = getcwd( buff, PATH_MAX + 1 );
      if (cwd == NULL)
      {
          perror("cwd");
          exit(0);
      }

      // opens the stream for the current file directory
      pDir = opendir(buff);
      if ( pDir == NULL )
      {
          perror(".");
          exit(0);
      }
      
      // goes through each file in the current directory
      while ((pDirent = readdir(pDir)) != NULL )
      {   
           print_details(pDirent->d_name);
      }
      
      // checks if readdir sets errno
      if(errno)
      {
           perror("readdir() failed");
      }    
    
      // close the directory stream
      if (closedir(pDir) == -1) 
      {   
           perror("close");
           exit(0);
      } 
   }

   return 0;
}
