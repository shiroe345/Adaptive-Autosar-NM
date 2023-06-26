#ifndef ARA_NM_I_STATE_MACHINE_H_
#define ARA_NM_I_STATE_MACHINE_H_

#include <functional>

class IStateMachine {
public:
    virtual void StartInstance() = 0;
    virtual void StopInstance() = 0;
    virtual void setRequested(bool requested) = 0;
};

#endif
