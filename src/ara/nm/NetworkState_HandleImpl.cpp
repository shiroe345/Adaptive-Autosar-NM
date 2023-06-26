#include "NetworkState_HandleImpl.hpp"
#include "nm.hpp"
#include "NmConfigReader.hpp"

#include <iostream>

using ara::nm::NetworkState_HandleImpl;

NetworkState_HandleImpl::NetworkState_HandleImpl(
    ara::com::InstanceIdentifier instanceIdentifier,
    int handlerIndex
): NetworkState_HandleSkeleton(instanceIdentifier, ara::com::MethodCallProcessingMode::kEventSingleThread), 
    handle{nmInstantiation.networkHandle[handlerIndex]} {
    initialize();
}

NetworkState_HandleImpl::NetworkState_HandleImpl(
    ara::com::InstanceIdentifierContainer instanceIds,
    int handlerIndex
): NetworkState_HandleSkeleton(instanceIds, ara::com::MethodCallProcessingMode::kEventSingleThread),
    handle{nmInstantiation.networkHandle[handlerIndex]} {
    initialize();
}

NetworkState_HandleImpl::NetworkState_HandleImpl(
    ara::core::InstanceSpecifier instanceSpec,
    int handlerIndex
): NetworkState_HandleSkeleton(instanceSpec, ara::com::MethodCallProcessingMode::kEventSingleThread),
    handle{nmInstantiation.networkHandle[handlerIndex]} {
    initialize();
}


NetworkState_HandleImpl::~NetworkState_HandleImpl() {
    for (auto &machineThread: machines) {
        machineThread -> stateMachine -> StopInstance();
        delete machineThread -> stateMachine;
    }
}

ara::core::Future<ara::nm::NetworkStateType> networkRequestedStateSetHandler(NetworkState_HandleImpl *handle,ara::nm::NetworkStateType newValue) {
    for (auto &machineThread: handle -> machines) {
        //[SWS_ANM_00084], here we notify every network so they know whether they're requested
        machineThread -> stateMachine -> setRequested(newValue == ara::nm::NetworkStateType::kFullCom);
    }
    //no need to check, the validity is guaranteed by enum NetworkStateType
    auto promise = ara::core::Promise<ara::nm::NetworkStateType>();
    promise.set_value(newValue);
    return promise.get_future();
}

IStateMachine *NetworkState_HandleImpl::createMachine(EthernetCommunicationConnector *connector, std::function<void(bool)> &onStateChangeToNetwork) {
    auto node = configReader[connector];
    auto cluster = configReader[node];
    return new ara::nm::NMInstance(node, cluster, onStateChangeToNetwork);
}

int NetworkState_HandleImpl::getEthernetConnectorNumber() {
    return handle.vlan.size();
}

void NetworkState_HandleImpl::initialize() {
    NetworkRequestedState.RegisterSetHandler([&](ara::nm::NetworkStateType newValue) {
        return networkRequestedStateSetHandler(this, newValue);
    });
    for (int i = 0; i < this->getEthernetConnectorNumber(); i++) {
        Machine *machine = new Machine();
        machines.emplace_back(machine);
        auto &last = machines.back();
        last -> handle = this;
        last -> stateMachine = createMachine(handle.vlan[i], last -> machineStateChangeCallback);
    }

    for (auto &machine: machines) {
        machine -> stateMachine -> StartInstance();
    }

    OfferService();
}

void NetworkState_HandleImpl::updateNetworkCurrentState() {
    //[SWS_ANM_00083], NetworkCurrentState should be kFullCom only if all associated networks are in Network Mode
    auto targetState = NetworkStateType::kFullCom;
    for (auto &machineThread: machines) {
        std::cout << &machineThread << " in network mode in update: " << machineThread -> machineInNetworkMode << std::endl;
        if (!machineThread -> machineInNetworkMode) {
            targetState = NetworkStateType::kNoCom;
            break;
        }
    }
    NetworkCurrentState.Update(targetState);
}
