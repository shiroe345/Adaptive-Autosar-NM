#ifndef ARA_NM_NETWORK_STATE_TYPE_H_
#define ARA_NM_NETWORK_STATE_TYPE_H_

#include <cstdint>

namespace ara {
    namespace nm {
        enum class NetworkStateType: uint8_t {
            kNoCom, kFullCom
        };
    }
}

#endif