#ifndef ARA_NM_NM_CPP_
#define ARA_NM_NM_CPP_

#include "nm.hpp"
#include <iostream>

//#include <bits/stdc++.h>

namespace ara {
    namespace nm {
        NMInstance::NMInstance(UdpNmNode *node, UdpNmCluster *cluster, std::function<void(bool)> &onStateChangeToNetwork)
            : node(node), cluster(cluster), onStateChangeToNetwork(onStateChangeToNetwork) {
            socket.setServerAndBind(cluster->networkConfiguration.ipv4MulticastipAaddress);
            socket.setServerLoopBack();

            socket.setClientAndBind(cluster->networkConfiguration.ipv4MulticastipAaddress);
            socket.setClientAddGroup(cluster->networkConfiguration.ipv4MulticastipAaddress);
        }

        void NMInstance::StartInstance() {
            this->Reset();
            state = NMInstanceState::NM_STATE_BUS_SLEEP;
            timer.start(1000, [this]() { this->Tick(); });
        }

        void NMInstance::StopInstance() {
            timer.stop();
            socket.clientLeaveGroup();
            state = NMInstanceState::NM_STATE_INIT;
        }

        void NMInstance::Reset() {
            _ticks = 0;
            isNetworkRequested = false;
            recievedNmMsg = false;
            isNmTimeoutTimerRunning = false;
            isNmRepeatMessageTimerRunning = false;
            isNmMsgCycleTimerRunning = false;
            isNmImmediateCycleTimerRunning = false;
            isNmWaitBusSleepTimerRunning = false;
            nmTimeoutTimerTicks = 0;
            nmRepeatMessageTimerTicks = 0;
            nmMsgCycleTimerTicks = 0;
            nmImmediateCycleTimerTicks = 0;
            nmWaitBusSleepTimerTicks = 0;
        }

        void NMInstance::RestartTimeoutTimer() {
            isNmTimeoutTimerRunning = true;
            nmTimeoutTimerTicks = 0;
        }

        void NMInstance::setRequested(bool requested) {
            isNetworkRequested = requested;
        }

        void NMInstance::Tick() {
            _ticks++;
            char nodeIDChar = node->nmNodeId;
            if(state == NMInstanceState::NM_STATE_INIT)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_INIT" << std::endl;
            else if(state == NMInstanceState::NM_STATE_BUS_SLEEP)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_BUS_SLEEP" << std::endl;
            else if(state == NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_PREPARE_BUS_SLEEP" << std::endl;
            else if(state == NMInstanceState::NM_STATE_REPEAT_MESSAGE)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_REPEAT_MESSAGE" << std::endl;
            else if(state == NMInstanceState::NM_STATE_NORMAL_OPERATION)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_NORMAL_OPERATION" << std::endl;
            else if(state == NMInstanceState::NM_STATE_READY_SLEEP)
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_READY_SLEEP" << std::endl;
            else
                std::cout << "NodeID: " << node -> nmNodeId << " Tick: " << _ticks << " current_state: "
                          << "NM_STATE_UNKNOWN" << std::endl;
            // check nmMessage
            if (socket.receiveBroadcast(nodeIDChar) == 1) {
                recievedNmMsg = true;
                std::cout<<"recievedNmMsg = true"<<std::endl;
                // recived nmMessage, restart timeout timer
                RestartTimeoutTimer();
            } else {
                recievedNmMsg = false;
            }

            if (state == NMInstanceState::NM_STATE_INIT) {
                return;
            } else if (state == NMInstanceState::NM_STATE_BUS_SLEEP) {  // bus sleep
                if (isNetworkRequested == true || recievedNmMsg) {
                    // network requested, enter network mode
                    onStateChangeToNetwork(true);

                    state = NMInstanceState::NM_STATE_REPEAT_MESSAGE;
                    isNmImmediateCycleTimerRunning = true;
                    isNmTimeoutTimerRunning = true;
                    nmTimeoutTimerTicks = 0;
                    nmImmediateCycleTimerTicks = 0;
                    immediateCycleTimes = cluster -> nmImmediateNmTransmissions;

                    nmMsgCycleOffsetRemainingTicks = node -> nmMsgCycleOffset;
                }
                // bus sleep finish
            } else if (state == NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP) {  // prepare bus sleep
                if (isNetworkRequested == true || recievedNmMsg) {
                    // network requested, enter network mode
                    onStateChangeToNetwork(true);

                    state = NMInstanceState::NM_STATE_REPEAT_MESSAGE;
                    isNmImmediateCycleTimerRunning = true;
                    isNmTimeoutTimerRunning = true;
                    nmTimeoutTimerTicks = 0;
                    nmImmediateCycleTimerTicks = 0;
                    immediateCycleTimes = cluster -> nmImmediateNmTransmissions;
                    nmMsgCycleOffsetRemainingTicks = node -> nmMsgCycleOffset;
                } else if (isNmWaitBusSleepTimerRunning) {
                    if (nmWaitBusSleepTimerTicks >= cluster -> nmWaitBusSleepTime) {
                        // wait bus sleep timeout, enter bus sleep mode
                        state = NMInstanceState::NM_STATE_BUS_SLEEP;
                        isNmWaitBusSleepTimerRunning = false;
                        nmWaitBusSleepTimerTicks = 0;
                    } else {
                        nmWaitBusSleepTimerTicks++;
                    }
                }
                // prepare bus sleep finish
            } else if (state == NMInstanceState::NM_STATE_REPEAT_MESSAGE) {  // repeat message
                bool isInOffset = false;
                if(nmMsgCycleOffsetRemainingTicks >= 0){
                    nmMsgCycleOffsetRemainingTicks--;
                    isInOffset = true;
                }else{
                    isInOffset = false;
                }

                if (isNmImmediateCycleTimerRunning && !isInOffset) {
                    if (immediateCycleTimes <= 0) {
                        // immediate cycle times used up, start repeat message timer and message cycle timer
                        isNmImmediateCycleTimerRunning = false;
                        isNmRepeatMessageTimerRunning = true;
                        isNmMsgCycleTimerRunning = true;
                        nmImmediateCycleTimerTicks = 0;
                        nmRepeatMessageTimerTicks = 0;
                        nmMsgCycleTimerTicks = 0;
                    } else {
                        if (nmImmediateCycleTimerTicks >= cluster -> nmImmediateNmCycleTime) {
                            // send immediate message
                            socket.serverSendBuffer(nodeIDChar);
                            RestartTimeoutTimer();  // message sent, restart timeout timer
                            immediateCycleTimes--;
                            nmImmediateCycleTimerTicks = 0;
                        } else {
                            nmImmediateCycleTimerTicks++;
                        }
                    }
                } else if (isNmRepeatMessageTimerRunning && !isInOffset) {
                    if (nmRepeatMessageTimerTicks >= cluster -> nmRepeatMessageTime) {
                        // repeat message timeout, check request network state
                        isNmRepeatMessageTimerRunning = false;
                        nmRepeatMessageTimerTicks = 0;
                        if (isNetworkRequested == false) {
                            // network not requested, enter ready sleep state
                            state = NMInstanceState::NM_STATE_READY_SLEEP;
                            isNmMsgCycleTimerRunning = false;
                            nmMsgCycleTimerTicks = 0;
                        } else {
                            // network requested, enter normal operation state
                            state = NMInstanceState::NM_STATE_NORMAL_OPERATION;
                        }
                    } else {
                        nmRepeatMessageTimerTicks++;
                        if (isNmMsgCycleTimerRunning) {
                            if (nmMsgCycleTimerTicks >= cluster -> nmMsgCycleTime) {
                                // send repeat message
                                socket.serverSendBuffer(nodeIDChar);  // message sent, restart timeout timer
                                RestartTimeoutTimer();
                                nmMsgCycleTimerTicks = 0;
                            } else {
                                nmMsgCycleTimerTicks++;
                            }
                        }
                    }
                }
                if (isNmTimeoutTimerRunning) {
                    if (nmTimeoutTimerTicks >= cluster -> nmNetworkTimeout) {
                        RestartTimeoutTimer();
                    } else {
                        nmTimeoutTimerTicks++;
                    }
                }
                // repeat message finish
            } else if (state == NMInstanceState::NM_STATE_NORMAL_OPERATION) {  // normal operation
                if (isNetworkRequested == false) {
                    // network not requested, enter ready sleep state
                    state = NMInstanceState::NM_STATE_READY_SLEEP;
                    isNmMsgCycleTimerRunning = false;
                    nmMsgCycleTimerTicks = 0;
                } else if (isNmMsgCycleTimerRunning) {
                    if (nmMsgCycleTimerTicks >= cluster -> nmMsgCycleTime) {
                        // send Nm message
                        socket.serverSendBuffer(nodeIDChar);  // message sent, restart timeout timer
                        RestartTimeoutTimer();
                        nmMsgCycleTimerTicks = 0;
                    } else {
                        nmMsgCycleTimerTicks++;
                    }
                }
                if (isNmTimeoutTimerRunning) {
                    if (nmTimeoutTimerTicks >= cluster -> nmNetworkTimeout) {
                        RestartTimeoutTimer();
                    } else {
                        nmTimeoutTimerTicks++;
                    }
                }
                // normal operation finish
            } else if (state == NMInstanceState::NM_STATE_READY_SLEEP) {
                if (isNetworkRequested == true) {
                    // network requested, enter normal operation state
                    state = NMInstanceState::NM_STATE_NORMAL_OPERATION;
                    isNmMsgCycleTimerRunning = true;
                    nmMsgCycleTimerTicks = 0;
                } else if (isNmTimeoutTimerRunning) {
                    if (nmTimeoutTimerTicks >= cluster -> nmNetworkTimeout) {
                        // timeout, enter prepare bus sleep state
                        onStateChangeToNetwork(false);
                        state = NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP;
                        isNmTimeoutTimerRunning = false;
                        nmTimeoutTimerTicks = 0;
                        isNmWaitBusSleepTimerRunning = true;
                        nmWaitBusSleepTimerTicks = 0;
                    } else {
                        nmTimeoutTimerTicks++;
                    }
                }
                // ready sleep finish
            }
        }
    }  // namespace nm
}  // namespace ara

#endif