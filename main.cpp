#include <drogon/drogon.h>
#include "Doctor.h"
#include "Patient.h"

using namespace drogon;
using namespace std;

int main() {
    // Doctor signup
    app().registerHandler("/doctor/signup", [](const HttpRequestPtr& req,
                                                    function<void(const HttpResponsePtr &)> callback) {
        auto jsonReq = req->getJsonObject();
        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();
        Doctor doc(username, password);
        doc.signUp();
        auto resp = HttpResponse::newHttpJsonResponse({{"message", "Doctor Registered"}});
        callback(resp);
    });

    // Doctor login
    app().registerHandler("/doctor/login", [](const HttpRequestPtr& req,
                                              function<void(const HttpResponsePtr &)> callback) {
        auto jsonReq = req->getJsonObject();
        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();
        Doctor doc(username, password);
        bool success = doc.login();
        auto resp = HttpResponse::newHttpJsonResponse({{"login", success}});
        callback(resp);
    });

    // Patient signup (returns ID)
    app().registerHandler("/patient/signup", [](const HttpRequestPtr& req,
                                                function<void(const HttpResponsePtr &)> callback) {
        auto jsonReq = req->getJsonObject();
        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();
        Patient pat(username, password);
        pat.signUp();
        auto resp = HttpResponse::newHttpJsonResponse({{"message", "Patient Registered"}});
        callback(resp);
    });

    // Patient login
    app().registerHandler("/patient/login", [](const HttpRequestPtr& req,
                                               function<void(const HttpResponsePtr &)> callback) {
        auto jsonReq = req->getJsonObject();
        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();
        Patient pat(username, password);
        bool success = pat.login();
        auto resp = HttpResponse::newHttpJsonResponse({{"login", success}});
        callback(resp);
    });

    // Serve frontend
    app().setDocumentRoot("./static");
    app().addListener("0.0.0.0", 8080);
    app().run();
}
