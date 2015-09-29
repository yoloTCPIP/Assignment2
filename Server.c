#include "inet.h"
#define BUFSIZE 1024
  
void main() {  
 /* Get the IP address*/
 FILE *fp;
 char *token;
 char returnIP[64];
 char arrIP[80][80];
 char * IPaddr;
 int i=0;

 fp = popen("/sbin/ifconfig eth0| grep 'inet addr:'", "r");
 while (fgets(returnIP, 64, fp) != NULL){
    token = strtok(returnIP," :");
    while(token!=NULL){
      strcpy(arrIP[i],token);
      i++;
      token=strtok(NULL," :");
    }
 }
 IPaddr = arrIP[2];

 /* End of get the IP address */
 struct sockaddr_in addr, cl_addr;  
 int sockfd, len, ret, newsockfd;  
 char buffer[BUFSIZE];  
 pid_t childpid;  
 char clientAddr[CLADDR_LEN];  


 sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 if (sockfd < 0) {  
  printf("Error creating socket!\n");  
  exit(1);  
 }  
 printf("Socket created...\n");  
   
 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = inet_addr(IPaddr); //INADDR_ANY; 
 addr.sin_port = SERV_TCP_PORT;   
   
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (ret < 0) {  
  printf("Error binding!\n");  
  exit(1);  
 }  
 printf("Binding done...\n");  
  
 printf("Waiting for a connection...\n");  
 listen(sockfd, 5);  
  
 for (;;) { //infinite loop  
  len = sizeof(cl_addr);  
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);  
  if (newsockfd < 0) {  
   printf("Error accepting connection!\n");  
   exit(1);  
  }  
  printf("Connection accepted...\n");  
  
  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);  
  if ((childpid = fork()) == 0) { //creating a child process  
  
   close(sockfd);   
//stop listening for new connections by the main process.   
//the child will continue to listen.   
//the main process now handles the connected client.  
  
   for (;;) {  
    memset(buffer, 0, BUFSIZE);  
    ret = recvfrom(newsockfd, buffer, BUFSIZE, 0, (struct sockaddr *) &cl_addr, &len);  
    if(ret < 0) {  
     printf("Error receiving data!\n");    
     exit(1);  
    }  
    printf("Received data from %s: %s\n", clientAddr, buffer);   
  
    ret = sendto(newsockfd, buffer, BUFSIZE, 0, (struct sockaddr *) &cl_addr, len);     
    if (ret < 0) {    
     printf("Error sending data!\n");    
     exit(1);    
    }    
    printf("Sent data to %s: %s\n", clientAddr, buffer);  
   }  
  }  
  close(newsockfd);  
 }  
  pclose(fp);
}  
