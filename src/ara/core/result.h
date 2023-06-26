#ifndef ARA_CORE_RESULT_H_
#define ARA_CORE_RESULT_H_

#include "error_code.h"

namespace ara {
    namespace core {
        //[SWS_CORE_00701], used by NM skeletons
        template <typename T, typename E = ErrorCode>
        class Result {
            public:
            //[SWS_CORE_00722]
            Result(T &&t): result{t} {}

            private:
            T& result;
        };

        //partial specialized Result
        //[SWS_CORE_00801] 
        template <typename E>
        class Result<void, E> {
            public:
            //[SWS_CORE_00821]
            Result() {}
        };
    }
}

#endif