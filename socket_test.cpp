#include "socket/socket.cpp"

#include <iostream>

int main(){
    Socket s1, s2;
    s1.setServerAndBind("224.0.0.6");
    //s2.setServerAndBind("224.0.0.7");

    s1.setClientAndBind();
    s1.setClientAddGroup("224.0.0.6");

    s2.setClientAndBind();
    // s2.setClientAddGroup("224.0.0.7");

    s1.serverSendBuffer('0');
    while (!s2.receiveBroadcast('1'))
        ;  // std::cout<<"Not Yet\n";
    std::cout << "i got a message\n";
}