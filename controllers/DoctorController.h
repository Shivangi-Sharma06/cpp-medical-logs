#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class DoctorController : public drogon::HttpController<DoctorController> {
public:
    METHOD_LIST_BEGIN
    // POST /doctor/signup
    ADD_METHOD_TO(DoctorController::signup, "/doctor/signup", Post);
    // POST /doctor/login
    ADD_METHOD_TO(DoctorController::login, "/doctor/login", Post);
    // POST /doctor/view_logs
    ADD_METHOD_TO(DoctorController::viewLogs, "/doctor/view_logs", Post);
    // POST /doctor/update_log
    ADD_METHOD_TO(DoctorController::updateLog, "/doctor/update_log", Post);
    METHOD_LIST_END

    void signup(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void viewLogs(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void updateLog(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};
