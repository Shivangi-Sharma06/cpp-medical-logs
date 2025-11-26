#include <drogon/drogon.h>
#include <filesystem>

int main() {
    const std::filesystem::path projectRoot = std::filesystem::path(__FILE__).parent_path();
    const auto configPath = (projectRoot / "config.json").string();
    const auto staticRoot = (projectRoot / "frontend").string();

    drogon::app().loadConfigFile(configPath);
    drogon::app().setDocumentRoot(staticRoot);
    drogon::app().run();
    return 0;
}
