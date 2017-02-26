/*
 * proxy.C
 *   proxy - An http proxy server
 *   usage: proxy port 
 *   optional extensions: ad (advertisement redirect), edu (.edu censorship)
 *
 *   Jessie Cisneros and Joey Gao 11/23/15
 */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
using namespace std;

#define BUFSIZE 8192

void fail(const char *str) { perror(str); exit(1); }
void backend(char *hostname, vector<string> request, int client_sock);
int switch_edu = 0;
int switch_ad = 0;

/* function: send_400_bad_request
 *    Writes a 400 Bad Request to the sock
 *    inputs: sock
 *    output: HTML 400 Bad Request 
 */
void send_400_bad_request(int sock)
{
  char html[500] = "<html>\r\n<head> <title>400 Bad Request</title></head>\r\n<body>\r\n<h1>400 Bad Request</h1><address>Proxy server</address>\r\n<p>Joey Gao and Jessie Cisneros, 2015</p>\r\n</body> </html>\r\n";
  char buf[500];
  sprintf(buf, "HTTP/1.1 400 Bad Request\r\nContent-Length: %ld\r\nConnection: close\r\nContent-Type: text/html\r\n\n<html>\r\n<head> <title>400 Bad Request</title></head>\r\n<body>\r\n<h1>400 Bad Request</h1><address>Proxy server</address>\r\n<p>Joey Gao and Jessie Cisneros, 2015</p>\r\n</body> </html>\r\n", strlen(html));
    
  write(sock,buf,strlen(buf));
}

/* function: send_404_not_found
 *    Writes a 404 Not Found to the sock
 *    inputs: sock
 *    output: HTML 404 Not Found 
 */
void send_404_not_found(int sock)
{  
  char html[500] = "<html>\r\n<head> <title>404 Not Found</title> </head>\r\n<body>\r\n<h1>404 Not Found</h1><address>Proxy server</address>\r\n<p>Joey Gao and Jessie Cisneros, 2015</p>\r\n</body> </html>\r\n";
  char buf[500];
  sprintf(buf, "HTTP/1.1 404 Not Found\r\nContent-Length: %ld\r\nConnection: close\r\nContent-Type: text/html\r\n\n<html>\r\n<head> <title>404 Not Found</title> </head>\r\n<body>\r\n<h1>404 Not Found</h1><address>Proxy server</address>\r\n<p>Joey Gao and Jessie Cisneros, 2015</p>\r\n</body> </html>\r\n", strlen(html));
  
  write(sock,buf,strlen(buf));
}

/* function: frontend
 *    Handles front end 
 *    inputs: v, a sock
 */
void *frontend(void *v)
{
  int sock = *(int *)v;
  free(v);

  // promote client's socket to a stream
  FILE *myclient = fdopen(sock,"r");
  if (myclient == NULL) {
    perror("fdopen");
    pthread_exit(0);
  }
  
  char buf[BUFSIZE];
  char hostline[BUFSIZE];
  vector<string> requestVector;
  
  int hostTrigger = 0;

  // Moves request from stream to requestVector
  while (fgets(buf,BUFSIZE,myclient) != NULL && strcmp(buf,"\r\n") != 0) {
    if (strncmp(buf, "Host:",5) == 0)
    {
      hostTrigger = 1;
      strcpy(hostline,buf);
    }
    if (strncmp(buf, "Connection:",11) != 0)
    {
      requestVector.push_back(buf);
    }   
  }
  
  // Send 400 Bad Request if hostline not found
  if(hostTrigger == 0)
  {
    send_400_bad_request(sock);
    fclose(myclient);
    close(sock);
    pthread_exit(0);
  } 
  
  requestVector.push_back("Connection: close\r\n");
  requestVector.push_back("\r\n");
  
  // Echoing request
  for (int i = 0; i < requestVector.size(); i++)
  {
    cout << requestVector.at(i);
  }
  
  printf("Connection closed on fdesc %d \n", sock);
 
  // Extract hostname from hostline
  char* saveptr;
  char* hostname = NULL;
  strtok_r(hostline," \r\n\t", &saveptr);
  hostname = strtok_r(NULL, "\r\n\t", &saveptr);
  
  // Restrict sites to .edu
  if (switch_edu == 1)
  {
    char* saveptr2;
    char* extension;
    int eduFound = 0;
    char copyhostname[strlen(hostname)];
    
    strcpy(copyhostname, hostname);
  
    extension = strtok_r(copyhostname, ".",&saveptr2);  

    while (extension != NULL)
    {
      if (strncmp(extension, "edu",3) == 0)
      {
	eduFound = 1;
      }
      extension = strtok_r(NULL, ".", &saveptr2);
    }
    
    if (eduFound == 0)
    {
      send_404_not_found(sock);
      fclose(myclient);
      close(sock);
      pthread_exit(0);
    }
  }

  backend(hostname, requestVector, sock);
  fclose(myclient);
  pthread_exit(0);
}

/* function: backend
 *    Handles backend 
 *    inputs: hostname, request, sock
 */
void backend(char *hostname, vector<string> request, int client_sock)
{
  int err;
  struct addrinfo *host;
  err = getaddrinfo(hostname,"http",NULL,&host);
  if (err) {
    if (switch_ad == 0) // Send 404 Not Found on error
    {
      send_404_not_found(client_sock);
      close(client_sock);
      pthread_exit(0);
    }
    else
    {
      getaddrinfo("hello.com","http",NULL,&host); // Redirect to advertiser on error
    }
  }

  // create socket
  int sock = socket(host->ai_family,SOCK_STREAM,0);
  if (sock < 0) fail("socket");

  // connect to host
  if (connect(sock,host->ai_addr,host->ai_addrlen)) fail("connect");

  // clean up - free the memory allocated by getaddrinfo
  freeaddrinfo(host);
    
  // promote the socket to a FILE stream for outpu
  FILE *serv_stream = fdopen(sock,"w");
  if (serv_stream == NULL) fail("fdopen");
  setlinebuf(serv_stream); 

  // Send request to server stream
  int i = 0;
  for ( i; i < request.size(); i++)
  {
    if (fputs(request.at(i).c_str(),serv_stream) == EOF)
    {
      break;
    }
  }
  
  fflush(serv_stream);

  // Reading from server socket and writing to client's socket
  char buf[BUFSIZE];
  size_t store = 1;
  int error = 0;
  while (store > 0)
  {
    store = read(sock,buf,BUFSIZE);
    if (store > 0)
    { 
      error = write(client_sock,buf,store);
      if ( error < 0 )
      {
        perror("write");
        break;
      }
    }
  }

  fclose(serv_stream);
  close(client_sock);
  close(sock);
}

/* function: main
 *    Initiates proxy
 */
int main(int argc, char *argv[])
{
  signal(SIGPIPE,SIG_IGN); 
  
  int signal_stop = 0;
  
  // Checking command line arguments: ad or edu
  if (argc == 3)
  {
    if ( strcmp(argv[2],"ad") == 0)
    {
      switch_ad = 1;
    }
    else if (strcmp(argv[2],"edu") == 0)
    {
      switch_edu = 1;
    }
    else
    {
      signal_stop = 1;
    }
  }

  // If there aren't two or three arguments OR ad/edu was not found, throw error
  if((argc < 2 || argc > 3) || signal_stop)
  {
    fprintf(stderr,"usage: proxy port\n");
    exit(1);
  }
  
  int err;
  struct addrinfo *host;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE;
  err = getaddrinfo(NULL,argv[1],&hints,&host);
  if (err) {
    fprintf(stderr, "%s : %s \n", argv[1], gai_strerror(err));
    exit(err);
  }
  
  // create socket
  int serv_sock = socket(host->ai_family,SOCK_STREAM,0);
  if (serv_sock < 0) fail("socket");

  // bind public socket to listening address
  if (bind(serv_sock,host->ai_addr,host->ai_addrlen)) fail("bind");

  // set the socket to listen (allow 5 pending connections)
  if (listen(serv_sock,5)) fail("listen");

  // clean up - free the memory allocated by getaddrinfo
  freeaddrinfo(host);

  printf("Waiting for connections on port %s \n", argv[1]);

  int *new_sock;
  pthread_t new_thread;

  while (1) 
  {
    // space for the integer value of the socket (so it can be passed to the frontend() thread routine as an argument)
    new_sock = (int *)malloc(sizeof(int));

    // accept one incoming connection
    *new_sock = accept(serv_sock,NULL,NULL);
    if (*new_sock < 0) fail("accept");

    printf("Accepted new connection on fdesc %d \n", *new_sock); 

    // create & detach a thread to serve the new client
    pthread_create(&new_thread,NULL,frontend,new_sock);
    pthread_detach(new_thread);
  }
}
