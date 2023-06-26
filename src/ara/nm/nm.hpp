#ifndef ARA_NM_NM_HPP_
#define ARA_NM_NM_HPP_

#include <cstdint>

#include "../../../include/prototype.h"
#include "../../../socket/socket.cpp"
#include "CallbackTimer.cpp"
#include "IStateMachine.hpp"
#include "NetworkStateType.hpp"
namespace ara{
    namespace nm{
        enum class NMInstanceState: uint8_t{
            NM_STATE_INIT,
            NM_STATE_BUS_SLEEP,
            NM_STATE_PREPARE_BUS_SLEEP,
            NM_STATE_REPEAT_MESSAGE,
            NM_STATE_NORMAL_OPERATION,
            NM_STATE_READY_SLEEP
        };

        class NMInstance : public IStateMachine {
            UdpNmNode *node;
            UdpNmCluster *cluster;
            CallbackTimer timer;
            NMInstanceState state = NMInstanceState::NM_STATE_INIT;
            std::function<void(bool)> &onStateChangeToNetwork;
            Socket socket;

           public:
            NMInstance(UdpNmNode *node, UdpNmCluster *cluster, std::function<void(bool)> &onStateChangeToNetwork);
            void StartInstance();
            void StopInstance();
            void setRequested(bool requested);
            void Reset();
            NMInstanceState GetState() { return state; }

           private:
            int _ticks = 0;
            bool isNetworkRequested = false;
            bool recievedNmMsg = false;
            bool isNmTimeoutTimerRunning = false;
            bool isNmRepeatMessageTimerRunning = false;
            bool isNmMsgCycleTimerRunning = false;
            bool isNmImmediateCycleTimerRunning = false;
            bool isNmWaitBusSleepTimerRunning = false;
            int nmMsgCycleOffsetRemainingTicks = 0;
            int nmTimeoutTimerTicks = 0;
            int nmRepeatMessageTimerTicks = 0;
            int nmMsgCycleTimerTicks = 0;
            int nmImmediateCycleTimerTicks = 0;
            int immediateCycleTimes = 0;
            int nmWaitBusSleepTimerTicks = 0;
            void RestartTimeoutTimer();
            void Tick();
        };
    }
}

#endif