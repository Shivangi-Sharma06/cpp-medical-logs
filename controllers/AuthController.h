#pragma once
#include <drogon/HttpController.h>
#include "../include/AuthManager.h"
#include "../include/nlohmann/json.hpp"

using namespace drogon;
using json = nlohmann::json;

class AuthController : public drogon::HttpController<AuthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::signupPatient, "/patient/signup", Post);
    ADD_METHOD_TO(AuthController::loginPatient, "/patient/login", Post);
    ADD_METHOD_TO(AuthController::signupDoctor, "/doctor/signup", Post);
    ADD_METHOD_TO(AuthController::loginDoctor, "/doctor/login", Post);
    METHOD_LIST_END

    void signupPatient(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void loginPatient(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void signupDoctor(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void loginDoctor(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
