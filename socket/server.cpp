#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#include <iostream>



int main(int argc, char const *argv[]) {

  sockaddr_in si_me, si_other;
  int s;
  printf("Making socket\n");
  assert((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))!=-1);
  fprintf(stderr,"usage %s hostname port\n", argv[0]);
  int port=11110;
  int broadcast= 1;


  if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) < 0)
    {
        return 0;
    }
// #ifdef __APPLE__
//   setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &broadcast, sizeof(broadcast));
// #endif

  memset(&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  // si_me.sin_addr.s_addr = inet_addr("127.0.0.1");;
  si_me.sin_addr.s_addr = INADDR_BROADCAST ;
  std::cout << "Binding to " << inet_ntoa(si_me.sin_addr) << ":" << ntohs(si_me.sin_port) << std::endl;

  //assert(::bind(s, (sockaddr *)&si_me, sizeof(sockaddr))!=-1);

  while(1)
  {
     printf("Send message to broadcast\n");
     char buf[1000];
     strcpy(buf, "twireshark\n");
     unsigned slen=sizeof(sockaddr);
     // send(s, buf, sizeof(buf)-1, 0);
      sendto(s,buf,strlen(buf)-1,0,(sockaddr *)&si_me,sizeof(si_me));
     //recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);
     printf("recv: %s\n", buf);
     sleep(1);
  }
}
