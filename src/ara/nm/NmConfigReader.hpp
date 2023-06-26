#ifndef ARA_NM_NM_CONFIG_READER_H_
#define ARA_NM_NM_CONFIG_READER_H_

#include <map>
#include <unordered_map>
#include <utility>
#include <iostream>

#include "../../../include/prototype.h"

//The implementation assume that only UdpNmCluster available in NmConfig
class NmConfigReader {
private:
    //map EthernetCommunicationConnector to Pair<cluster index in NmConfig, node index in NmNode>
    std::unordered_map<EthernetCommunicationConnector *, pair<int, int>> connectorToIndices;
    void initializeMaps() {
        for (int i = 0; i < nmConfig.nmCluster.size(); i++) {
            auto cluster = nmConfig.nmCluster[i];
            for (int j = 0; j < cluster -> nmNode.size(); j++) {
                auto node = (UdpNmNode *)cluster -> nmNode[j];
                auto connector = node -> communicationConnector;
                std::cout << "connector: " << connector << " nodeId: " << node -> nmNodeId << std::endl;
                connectorToIndices[connector] = make_pair(i, j);
            }
        }
    }
public:
    UdpNmNode *operator[](EthernetCommunicationConnector *connector) {
        if (connectorToIndices.empty()) initializeMaps();
        auto indices = connectorToIndices[connector];
        return (UdpNmNode *)nmConfig.nmCluster[indices.first] -> nmNode[indices.second];
    }

    UdpNmCluster *operator[](UdpNmNode *node) {
        if (connectorToIndices.empty()) initializeMaps();
        auto indices = connectorToIndices[node -> communicationConnector];
        return (UdpNmCluster *)nmConfig.nmCluster[indices.first];
    }
};

extern NmConfigReader configReader;


#endif
