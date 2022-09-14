cmake_minimum_required(VERSION 3.21)
project(KCPDemoCmakeTest)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32)

add_executable(KCPDemoCmakeTest main.cpp)
add_executable(KCPServer  KCPServer.cpp ikcp.h ikcp.cpp)
add_executable(KCPClient  KCPClient.cpp ikcp.h ikcp.cpp)
add_executable(KCPServer1  KCPServer1.cpp ikcp.h ikcp.cpp)
add_executable(KCPClient1  KCPClient1.cpp ikcp.h ikcp.cpp)
add_executable(Test  test.cpp  )
#一个字离谱 只需要一个CPP
