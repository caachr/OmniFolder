#include <sodium.h>

#include <google/protobuf/stubs/common.h>

#include <QApplication>
#include <QLabel>

#include "core/app_core.h"

int main(int argc, char* argv[]) {
    // Initialize crypto lib
    if (sodium_init() < 0) {
       qDebug("Error: Sodium library initialization failed.");
       return 0;
    }

    // Initialize protobuf lib
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Initialize app
    QApplication app(argc, argv);
    // app.setApplicationName("OmniFolder Server");

    AppCore core;
    core.boot();

    // Cleanup protobuf library before exit
    google::protobuf::ShutdownProtobufLibrary();

    // Run
    return app.exec();
}
