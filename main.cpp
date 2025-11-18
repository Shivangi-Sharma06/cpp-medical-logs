#include <drogon/drogon.h>
#include <iostream>

int main() {
    // Load config.json if present (listeners, document_root, threads)
    drogon::app().loadConfigFile("config.json");

    // Ensure static folder is used if config doesn't set it
    drogon::app().setDocumentRoot("./static");

    std::cout << "🚀 Server starting on configured listeners (default :8080)\n";
    drogon::app().run();
    return 0;
}
