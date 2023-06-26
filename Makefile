all:
	g++ -std="c++14" include/prototype.h include/prototype.cpp socket/socket.cpp src/main.cpp src/ara/com/*.hpp src/ara/com/*.cpp src/ara/core/*.h src/ara/nm/*.hpp src/ara/nm/*.cpp -pthread -o main
