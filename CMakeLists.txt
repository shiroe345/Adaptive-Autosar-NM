cmake_minimum_required(VERSION 3.16)
project(APNM)

set(CMAKE_CXX_STANDARD 11)

#include_directories(src/ara/com)
#include_directories(src/ara/core)
#include_directories(src/ara/nm)

add_executable(APNM src/main.cpp
        src/main1.cpp
        src/main2.cpp
        src/main3.cpp
        socket/socket.cpp
        src/ara/nm/CallbackTimer.cpp
        src/ara/nm/NetworkState_Handle0Impl.cpp
        src/ara/nm/NetworkState_Handle0Skeleton.cpp
        src/ara/nm/nm.cpp
        include/prototype.h
        include/prototype.cpp
        src/ara/com/com_set_handler.hpp
        src/ara/com/com_set_handler.cpp
        src/ara/com/fakecom.hpp
        src/ara/core/error_code.h
        src/ara/core/future.h
        src/ara/core/instance_specifier.h
        src/ara/core/result.h
        src/ara/core/vector.h
        src/ara/nm/IStateMachine.hpp
        src/ara/nm/NetworkState_Handle0Impl.hpp
        src/ara/nm/NetworkState_Handle0Skeleton.hpp
        src/ara/nm/NetworkStateType.hpp
        src/ara/nm/nm.hpp
        src/ara/nm/NmConfigReader.hpp
        src/ara/nm/NmConfigReader.cpp)
