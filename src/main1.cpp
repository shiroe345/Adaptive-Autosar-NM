#include "../include/prototype.h"
#include "ara/nm/NetworkState_HandleImpl.hpp"
#include "ara/com/com_set_handler.hpp"

#include <chrono>
#include <iostream>

void initNode(UdpNmNode &node, int id, EthernetCommunicationConnector *connector) {
    // here we don't initialize node.machine, it's not in use
    node.nmNodeId = id;
    node.allNmMessagesKeepAwake = true;
    node.communicationConnector = connector;
    node.nmMsgCycleOffset = 1;
} 

void initCluster(UdpNmCluster &cluster, std::string multicastIp) {
    cluster.nmCbvPosition = 0;
    cluster.nmImmediateNmCycleTime = 3;
    cluster.nmImmediateNmTransmissions = 0;
    cluster.nmMsgCycleTime = 5;
    cluster.nmNetworkTimeout = 20;
    cluster.nmNidPosition = 1;
    cluster.nmPncParticipation = false;
    cluster.nmRepeatMessageTime = 10;
    cluster.nmUserDataLength = 0;
    cluster.nmUserDataOffset = 3;
    cluster.nmWaitBusSleepTime = 20;
    cluster.pncClusterVectorLength = 0;
    cluster.networkConfiguration.ipv4MulticastipAaddress = multicastIp;
}

int main() {
    // network structure
    EthernetCommunicationConnector connector0;
    UdpNmNode node0{};
    initNode(node0, 0, &connector0);

    EthernetCommunicationConnector connector1;
    UdpNmNode node1{};
    initNode(node1, 1, &connector1);

    EthernetCommunicationConnector connector2;
    UdpNmNode node2{};
    initNode(node2, 2, &connector2);

    EthernetCommunicationConnector connector3;
    UdpNmNode node3{};
    initNode(node3, 3, &connector3);

    UdpNmCluster cluster1{};
    initCluster(cluster1, "224.0.0.6");
    cluster1.nmNode.push_back(&node0);
    cluster1.nmNode.push_back(&node1);

    UdpNmCluster cluster2{};
    initCluster(cluster2, "224.0.0.7");
    cluster2.nmNode.push_back(&node2);
    cluster2.nmNode.push_back(&node3);

    nmConfig.nmCluster = vector<NmCluster *>{ 
        &cluster1,
        &cluster2
    };

    // handles
    nmInstantiation.networkHandle = vector<NmNetworkHandle>{
            {{}, {&connector0}},
            {{}, {&connector1}},
            {{}, {&connector2}},
            {{}, {&connector3}}
    };

    //controls cluster 1
    ara::nm::NetworkState_HandleImpl networkHandle0(ara::com::InstanceIdentifier{}, 0);
    //controls cluster 2
    ara::nm::NetworkState_HandleImpl networkHandle1(ara::com::InstanceIdentifier{}, 2);

    while(true) {
        std::string input;
        std::cin >> input;
        if (input == "on1") {
            for (auto &handle: handlers[&networkHandle0.NetworkRequestedState]) {
                handle(ara::nm::NetworkStateType::kFullCom);
            }
        } else if (input == "on2") {
            for (auto &handle: handlers[&networkHandle1.NetworkRequestedState]) {
                handle(ara::nm::NetworkStateType::kFullCom);
            }
        } else if (input == "off1") {
            for (auto &handle: handlers[&networkHandle0.NetworkRequestedState]) {
                handle(ara::nm::NetworkStateType::kNoCom);
            }
        } else if (input == "off2") {
            for (auto &handle: handlers[&networkHandle1.NetworkRequestedState]) {
                handle(ara::nm::NetworkStateType::kNoCom);
            }
        } else {
            break;
        }
    }

    networkHandle0.StopOfferService();
    networkHandle1.StopOfferService();
}
