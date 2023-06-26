#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

#include <vector>
#include <string>

using namespace std;

typedef uint32_t TimeValue;                 

class AdaptivePlatformServiceInstance{

};
/*              Physical Area               */

class EthernetPhysicalChannel{

};

class PhysicalChannel{

};

class EthernetCommunicationConnector{

};

class MachineDesign{

};

/*              Pnc Area                */

class PncMappingIdent{                          //this is only a referrable identifier

};

class PncMapping{
    public:
        PncMappingIdent ident;
        PhysicalChannel physicalChannel;        //map pnc to physical channel
        unsigned int pncIdentifier;
        bool pncWakeupEnable;
        AdaptivePlatformServiceInstance serviceInstance;
        string identifier;                      //pnc name
};



/*              Virtual Area                */

class CommunicationCluster{
    
};

class NmNode{                                   //this is abstract alse, as NmCluster
    public:
        MachineDesign machine;                  //reference to the machine which contain this node
        int nmNodeId;                   
};

class UdpNmNode : public NmNode{
    public:
        bool allNmMessagesKeepAwake;            //Specifies if Nm drops irrelevant NM PDUs
        EthernetCommunicationConnector* communicationConnector; 
        TimeValue nmMsgCycleOffset;              //Node specific time offset in the periodic transmission node
};

class NmCluster{                                //this class is abstract
    public:
        vector<NmNode *> nmNode;                  //collections of mnNodes in this cluster
        CommunicationCluster communicationCluster;
        bool nmPncParticipation = false;        //currently mo use
        unsigned int pncClusterVectorLength;    //currently no use
};

class UdpNmNetworkConfiguration{                //Configuration of a UDP port and UDP multicast IP address of the Nm communication on a VLAN
    public:
        string ipv4MulticastipAaddress;
        string ipv6MulticastipAaddress;
        unsigned int priority;                  //defines VLAN frmae priority
        unsigned int udpPort;
};

class UdpNmCluster : public NmCluster{
    public:
        UdpNmNetworkConfiguration networkConfiguration;
        int nmCbvPosition;                      //Defines the position of the control bit vector within the Nm Pdu (Byte positon).
        TimeValue nmImmediateNmCycleTime;       //Defines the immediate NmPdu cycle time in seconds which is used for nmImmediateNmTransmissions NmPdu transmissions.
        unsigned int nmImmediateNmTransmissions;//Defines the number of immediate NmPdus which shall be transmitted
        TimeValue nmMsgCycleTime;               //Period of a NmPdu in seconds
        TimeValue nmNetworkTimeout;             //Network Timeout for NmPdus in seconds
        int nmNidPosition;                      //Defines the byte position of the source node identifier within the NmPdu
        TimeValue nmRepeatMessageTime;          //Timeout for Repeat Message State in seconds
        int nmUserDataLength;                   //Defines the length in bytes of the user data contained in the Nm message
        unsigned int nmUserDataOffset;          //Specifies the offset (in bytes) of the user data information in the NM message
        TimeValue nmWaitBusSleepTime;           //Timeout for bus calm down phase in seconds
        EthernetPhysicalChannel vlan;           //Reference to vlan
};

class NmConfig{
    public:
        vector<NmCluster *> nmCluster;            //collection of mnClusters
};


class NmNetworkHandle{
    public:
        vector<PncMappingIdent> partialNetwork;         //reference to a pnc that included in this handle
        vector<EthernetCommunicationConnector*> vlan;   //reference to vlan that included in this handle, use pointer type to identify
};

class NmInstantiation{
    public:
        vector<NmNetworkHandle> networkHandle;          //supported handles used to control PNC
};

/*              Declaration             */

extern NmInstantiation nmInstantiation;                
extern NmConfig nmConfig;

#endif
