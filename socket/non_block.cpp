#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>


using namespace std;

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 创建 UDP 套接字
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    if (sockfd < 0) {
        cerr << "socket creation failed" << endl;
        return 1;
    }

  int broadcast= 1;


  // if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) < 0)
  //   {
  //       return 0;
  //   }

  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&broadcast, sizeof(broadcast)) < 0)
    {
        return 0;
    }

    struct sockaddr_in servaddr; // 套接字地址结构体
    memset(&servaddr, 0, sizeof(servaddr)); // 初始化地址结构体
    servaddr.sin_family = AF_INET; // 设置协议族为 IPv4
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);// 设置 IP 地址为本地任意地址
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置 IP 地址为本地任意地址
    servaddr.sin_port = htons(11110); // 设置端口号为 12345
    
    cout << "Binding to " << inet_ntoa(servaddr.sin_addr) << ":" << ntohs(servaddr.sin_port) << endl;


    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { // 绑定套接字
        cerr << "bind failed"<< strerror(errno) << endl;
        close(sockfd);
        return 1;
    }

    char buffer[1024];
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);

    while (true) {
        // ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cliaddr, &len); // 接收 UDP 数据报
        // if (n < 0) {
        //     cerr << "recvfrom failed" << endl;
        //     close(sockfd);
        //     return 1;
        // }

        // buffer[n] = '\0'; // 将接收到的数据转换为字符串
        // cout << "Received message: " << buffer << " from " << inet_ntoa(cliaddr.sin_addr) << endl;

        int n = recv( sockfd,buffer, 1024,0);
        if(n == -1){
            cerr << "not recv" << endl;
        }else{
            buffer[n] = '\0'; // 将接收到的数据转换为字符串
            cout << "Received message: " << buffer << " from " << inet_ntoa(cliaddr.sin_addr) << endl;
        }
        buffer[n] = '\0'; // 将接收到的数据转换为字符串
        sleep(5);
    }

    close(sockfd);
    return 0;
}
