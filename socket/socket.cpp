#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>
#include <ctime>


//todo : call broacast addr
//todo : package fix
//todo : test

class Socket{
public:
    int sock_Client,sock_Server;
    int broadcast = 1;
    struct sockaddr_in client_Addr,server_Addr;
    struct ip_mreq stMreqServer,stMreqClient;
    int port = 11115;
    std::string IP;
    int setServerAndBind(std::string addr){
        sock_Server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock_Server < 0) {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        if(setsockopt(sock_Server, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) < 0)
        {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        /*
        if(setsockopt(sock_Server, SOL_SOCKET, SO_REUSEADDR, (const char*)&broadcast, sizeof(broadcast)) < 0)
        {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }

        */
        memset(&server_Addr, 0, sizeof(server_Addr));
        server_Addr.sin_family = AF_INET;
        server_Addr.sin_port = htons(port);
        // server_Addr.sin_addr.s_addr = INADDR_BROADCAST ;//need to set addr
        IP = addr;
        server_Addr.sin_addr.s_addr = inet_addr(addr.c_str());
        /*
        if(bind(sock_Server, (struct sockaddr *)&server_Addr, sizeof(server_Addr)) < 0 ){
            std::cout << "Bind error" << std::endl;
            return 0;
        }
        */
        std::cout << "Binding to " << inet_ntoa(server_Addr.sin_addr) << ":" << ntohs(server_Addr.sin_port) << std::endl;
        return 1;
    }
    int setServerLoopBack(){
        int loop = 0;
        int err = setsockopt(sock_Server, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
        if (err < 0) {
            std::cerr << "setsockopt():IP_MULTICAST_LOOP"<< strerror(errno) << std::endl;
            return 0;
        }
        return 1;
    }
    // int setServerMultipleGroup(std::string addr){
    //     stMreqServer.imr_multiaddr.s_addr = inet_addr(addr.c_str());
    //     stMreqServer.imr_interface.s_addr = IP;
    //     if (setsockopt(sock_Server, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreqServer, sizeof(stMreqServer)) < 0) {
    //         std::cout << " join multiple group error" << std::endl;
    //         return 0;
    //     }
        
    // }
    int serverSendBuffer(char node){
        char buf[4] = {0,node,0,0};
        unsigned slen=sizeof(sockaddr);
        std::cout << "sent message, content: " << buf[0]+'0' << ' ' << buf[1]+'0' << std::endl; 
        sendto(sock_Server,buf,4,0,(sockaddr *)&server_Addr,sizeof(server_Addr));
        return 1;
    }
    int setClientAndBind(std::string multicastIp){
        sock_Client = socket(AF_INET, SOCK_DGRAM, 0);
        fcntl(sock_Client, F_SETFL, O_NONBLOCK);
        if (sock_Client < 0) {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        if(setsockopt(sock_Client, SOL_SOCKET, SO_REUSEADDR, (const char*)&broadcast, sizeof(broadcast)) < 0)
        {
            std::cerr << "socket set failed" << std::endl;
            return 0;
        }
        sockaddr_in client_Addr;
        memset(&client_Addr, 0, sizeof(client_Addr));
        client_Addr.sin_family = AF_INET;
        client_Addr.sin_addr.s_addr = inet_addr(multicastIp.c_str());
        // client_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        client_Addr.sin_port = htons(port);
        //std::cout << "Binding to " << inet_ntoa(client_Addr.sin_addr) << ":" << ntohs(client_Addr.sin_port) << std::endl;

        int status = bind(sock_Client, (struct sockaddr *) &client_Addr, sizeof(client_Addr));
        if (status < 0) { 
            std::cerr << "bind failed"<< strerror(errno) << std::endl;
            closeClientSocket();
            return 0;
        }
        return 1;
    }
    int setClientAddGroup(std::string str){
        stMreqClient.imr_multiaddr.s_addr = inet_addr(str.c_str());
        stMreqClient.imr_interface.s_addr = htonl(INADDR_ANY);
        int err = setsockopt(sock_Client, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreqClient, sizeof(stMreqClient));
        if (err < 0) {
            std::cerr << "setsockopt():IP_ADD_MEMBERSHIP"<< strerror(errno) << std::endl;
            return 0;
        }
        return 1;
    }
    int receiveBroadcast(char node){
        char buffer[4];
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        //int n = recv( sock_Client,buffer, 4,0);
        int length = sizeof(client_Addr);
        int n =recvfrom(sock_Client, buffer, 4, 0, (struct sockaddr*)&client_Addr,&len);

        if(n == -1 || buffer[1] == node){
            if (buffer[1] == node){
                std::cout << "receive broadcast from myself which is " << buffer[1]+'0' << std::endl;
            }
            return 0;
        }else{
            std::cout << "receive broadcast from " << buffer[1]+'0' << std::endl;
            return 1;
        }
    }
    void clientLeaveGroup(){
        setsockopt(sock_Client, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreqClient, sizeof(stMreqClient));
    }
    void closeClientSocket(){
        close(sock_Client);
    }
    void closeServerSocket(){
        close(sock_Server);
    }
    void setPort(int expect){
        port = expect;
    }
};
