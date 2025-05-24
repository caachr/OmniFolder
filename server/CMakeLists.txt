cmake_minimum_required(VERSION 3.29)
project(omniserver VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXXSTANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC
        src/main.cpp
        src/command_handler.cpp
        src/wizards.cpp
        src/server.cpp
        src/omni_network.cpp
        src/omni_folder.cpp
        src/omni_drive.cpp
        src/socket_master.cpp
        src/message.cpp)

add_executable(omniserver ${SRC})
