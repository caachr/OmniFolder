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
        src/port_authority.cpp
        src/message.cpp
        src/message_factory.cpp
        src/fedex.cpp
        src/schema_validators/config_validator.cpp
        src/schema_validators/message_validator.cpp
        src/hasher.cpp
        src/mailbox.cpp
        src/github_portal.cpp)

add_executable(omniserver ${SRC})

find_package(nlohmann_json_schema_validator CONFIG REQUIRED)
target_link_libraries(omniserver PRIVATE nlohmann_json_schema_validator::validator)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(omniserver PRIVATE unofficial-sodium::sodium)