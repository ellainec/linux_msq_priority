cmake_minimum_required(VERSION 3.15)
project(MsgQ_Priority)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(MsgQ_Priority main.cpp server.cpp msq.cpp msq.h client.h client.cpp)
target_link_libraries(MsgQ_Priority Threads::Threads)