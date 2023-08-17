/* run using ./server <port> */
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "http_server.hh"

using namespace std;
#define thread_count 128
#define MAX_TASKS 16384


queue<int> q;
pthread_mutex_t mutex_q;
pthread_cond_t var;

void error(char *msg) {
  perror(msg);
  // exit(1);
}

//------------------------------------------Code of Question_1--------------------------
// void *func(void * sock){

//   int n;
//   char buffer[256];
//   int newsockfd= *(int *) sock;

//   while (1){
//       bzero(buffer, 256);
//       n = read(newsockfd, buffer, 255);
//       if (n < 0)
//         error("ERROR reading from socket");
//       printf("Here is the message: %s", buffer);

//       if(n==0)            // ctrl c signal from client closes server 
//         break;

//       /* send reply to client */

//       n = write(newsockfd, buffer , 18);
//       if (n < 0)
//         error("ERROR writing to socket");
//     }
  

// }
//-------------------------------------------------------------------------------------------

int executor(int socket){
  // Complete this function

  char buffer[1024];
  bzero(buffer,1024);

  if(read(socket, buffer, 1023)<0) {
    perror("Can't read");
    return 1;
  }

  string monk(buffer);

  HTTP_Response *response = handle_request(monk);
    
  string answer=response->get_string();

  if(write(socket, answer.c_str(), answer.size()) < 0) {
    perror("Can't write");
    free(response);   //edit
    return 1;
  }
  
  free(response);   //edit
  
  return 0;
}



void *start_func(void *arg){
  int socket=0;
  while (1){
    pthread_mutex_lock(&mutex_q);

    while(q.empty())
      pthread_cond_wait(&var,&mutex_q);

    socket = q.front();
    q.pop();

    pthread_mutex_unlock(&mutex_q);
    
    executor(socket);
    close(socket);
  }
}



int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }
  

  pthread_t id[thread_count];           // thread id

  pthread_mutex_init (&mutex_q,NULL);
  pthread_cond_init (&var,NULL);

  for(int i=0;i<thread_count;i++){
    pthread_create(&id[i], NULL, start_func, NULL);
  }


  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
    ///////check here 

  clilen = sizeof(cli_addr);
  /* listen for incoming connection requests */
  listen(sockfd, thread_count);



  printf("Thread Count: %d\nQueue Size: %d\n", thread_count, MAX_TASKS);


  while (1){
    /* accept a new request, create a newsockfd */

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
      perror("ERROR on accept");
      continue;
    }

    pthread_mutex_lock(&mutex_q);

    if(q.size() < MAX_TASKS) {          // 
      q.push(newsockfd);
    }
    else {
      close(newsockfd);
      printf("*****CONNECTION DROPPED*****\n");
    }
    
    pthread_cond_signal(&var);
    pthread_mutex_unlock(&mutex_q);  
  }

  
  return 0;
}
