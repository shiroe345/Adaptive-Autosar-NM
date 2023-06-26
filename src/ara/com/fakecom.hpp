#ifndef ARA_COM_FAKECOM_H_
#define ARA_COM_FAKECOM_H_

#include "../core/vector.h"
namespace ara {
    namespace com {
        //[SWS_CM_00302], used by NM skeletons
        class InstanceIdentifier {
            
        };

        //[SWS_CM_00319], used by NM skeletons
        using InstanceIdentifierContainer = ara::core::Vector<InstanceIdentifier>;

        //AUTOSAR_EXP_ARAComAPI 5.4.5
        enum class MethodCallProcessingMode { kPoll, kEvent, kEventSingleThread };
    }
}

#endif