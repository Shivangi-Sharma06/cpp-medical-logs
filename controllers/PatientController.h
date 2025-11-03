#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class PatientController : public drogon::HttpController<PatientController> {
public:
    METHOD_LIST_BEGIN
    // POST /patient/signup
    ADD_METHOD_TO(PatientController::signup, "/patient/signup", Post);
    // POST /patient/login
    ADD_METHOD_TO(PatientController::login, "/patient/login", Post);
    // POST /patient/add_log
    ADD_METHOD_TO(PatientController::addLog, "/patient/add_log", Post);
    // POST /patient/view_logs
    ADD_METHOD_TO(PatientController::viewLogs, "/patient/view_logs", Post);
    METHOD_LIST_END

    void signup(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void addLog(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void viewLogs(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};
