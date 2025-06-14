cmake_minimum_required(VERSION 3.21)          # 3.21+ is enough for Qt6 API
project(omnifolder-server VERSION 0.1.0 LANGUAGES CXX)

# ─── Build flags ────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─── Qt 6 ───────────────────────────────────────────────────────────────────────
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
# Turn on automoc / autouic / autorcc in one line
qt_standard_project_setup()

# ─── Third-party libraries ─────────────────────────────────────────────────────
find_package(nlohmann_json_schema_validator CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

# ─── Sources ───────────────────────────────────────────────────────────────────
set(SRC
        src/main.cpp
        src/core/app_core.cpp
        src/core/app_core.h
        src/core/state_machine.cpp
        src/core/state_machine.h
        src/core/server_core.cpp
        src/core/server_core.h
        src/core/config_manager.cpp
        src/core/config_manager.h
        src/core/crypto_manager.cpp
        src/core/crypto_manager.h
        src/core/mailbox.cpp
        src/core/mailbox.h
        src/core/mem_guard.h
        src/core/serializable.h
        src/db/omni_network.cpp
        src/db/omni_network.h
        src/db/omni_folder.cpp
        src/db/omni_folder.h
        src/db/omni_drive.cpp
        src/db/omni_drive.h
        src/net/port_authority.cpp
        src/net/port_authority.h
        src/net/fedex.cpp
        src/net/fedex.h
        src/net/message_builder.cpp
        src/net/message_builder.h
        src/net/message.cpp
        src/net/message.h
        src/net/client_session.h
        src/net/client_session.cpp
        src/net/client_info.h
        src/recovery/beacon_manager.cpp
        src/recovery/beacon_manager.h
        src/gui/main_window.cpp
        src/gui/main_window.h
        src/gui/setup_wizard.cpp
        src/gui/setup_wizard.h
        src/gui/recovery_wizard.cpp
        src/gui/recovery_wizard.h
        src/util/version.h
        src/util/logging.cpp
        src/util/logging.h
        src/gui/client_table_model.cpp
        src/gui/client_table_model.h
        resources/schemas/config_schema.json
        # Generated protobuf files will be automatically added by protobuf_generate()
)

qt_add_resources(SRC "resources.qrc")

qt_add_executable(omnifolder-server
        MANUAL_FINALIZATION          # we'll link + finalize ourselves
        MACOSX_BUNDLE
        ${SRC}
)

# ─── Protobuf Generation ────────────────────────────────────────────────────────
set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/schemas/message_schema.proto
)

# Generate protobuf sources and headers (must be after target creation)
protobuf_generate(
    TARGET omnifolder-server
    PROTOS ${PROTO_FILES}
    IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/resources/schemas
)

target_include_directories(omnifolder-server PRIVATE
    src
    ${CMAKE_CURRENT_BINARY_DIR}  # For generated protobuf headers
    ${Protobuf_INCLUDE_DIRS}     # For protobuf system headers
)

target_link_libraries(omnifolder-server PRIVATE
        Qt6::Widgets
        Qt6::Network
        nlohmann_json_schema_validator::validator
        unofficial-sodium::sodium
        OpenSSL::SSL
        OpenSSL::Crypto
        protobuf::libprotobuf        # Use the modern target
)

qt_finalize_executable(omnifolder-server)            # handles rpaths on macOS
