#include "AuthController.h"
#include "../include/Doctor.h"
#include "../include/Patient.h"

using namespace std;

namespace {
std::string findPatientIdByUsername(const std::string &username) {
    const std::string file = "data/patient_credential.json";
    json users = AuthManager::loadCredentials(file);
    for (auto &u : users) {
        if (u.contains("username") && u["username"] == username) {
            if (u.contains("patient_id")) return u["patient_id"].get<std::string>();
        }
    }
    return std::string();
}
}

void AuthController::signupPatient(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    if (AuthManager::userExists("data/patient_credential.json", username)) {
        resp["error"] = "Username already exists";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    Patient p(username, password);
    p.signUp();

    resp["message"] = "Patient registered successfully";
    std::string pid = findPatientIdByUsername(username);
    if (!pid.empty()) resp["patient_id"] = pid;
    callback(HttpResponse::newHttpJsonResponse(resp));
}

void AuthController::loginPatient(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    Patient p(username, password);
    bool ok = p.login();
    if (ok) {
        resp["login"] = true;
        resp["message"] = "Patient login successful";
        std::string pid = findPatientIdByUsername(username);
        if (!pid.empty()) resp["patient_id"] = pid;
    } else {
        resp["login"] = false;
        resp["error"] = "Invalid credentials";
    }

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void AuthController::signupDoctor(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    if (AuthManager::userExists("data/doctor_credential.json", username)) {
        resp["error"] = "Username already exists";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    Doctor d(username, password);
    d.signUp();

    resp["message"] = "Doctor registered successfully";
    callback(HttpResponse::newHttpJsonResponse(resp));
}

void AuthController::loginDoctor(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    Doctor d(username, password);
    bool ok = d.login();
    if (ok) {
        resp["login"] = true;
        resp["message"] = "Doctor login successful";
    } else {
        resp["login"] = false;
        resp["error"] = "Invalid credentials";
    }

    callback(HttpResponse::newHttpJsonResponse(resp));
}

