#include "AuthController.h"
#include "../include/Patient.h"
#include "../include/Doctor.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static std::string findPatientIdByUsername(const std::string &username) {
    const std::string file = "data/patient_credential.json";
    std::ifstream f(file);
    if (!f.is_open()) return std::string();
    nlohmann::json users;
    try { f >> users; } catch (...) { return std::string(); }
    for (auto &u : users) {
        if (u.contains("username") && u["username"] == username) {
            if (u.contains("patient_id")) return u["patient_id"].get<std::string>();
        }
    }
    return std::string();
}

static inline void sendJsonResponse(const std::function<void(const HttpResponsePtr &)> &callback, const json &j) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(j.dump());
    callback(resp);
}

void AuthController::signupPatient(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    if (AuthManager::userExists("data/patient_credential.json", username)) {
        resp["error"] = "Username already exists";
        sendJsonResponse(callback, resp);
        return;
    }

    Patient p(username, password);
    p.signUp();

    std::string patientId = findPatientIdByUsername(username);
    resp["message"] = "Patient registered successfully";
    if (!patientId.empty()) resp["patient_id"] = patientId;
    else resp["note"] = "Registered but patient_id not found (check server console/data file)";

    sendJsonResponse(callback, resp);
}

void AuthController::loginPatient(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    Patient p(username, password);
    bool ok = p.login();
    if (ok) {
        std::string pid = findPatientIdByUsername(username);
        resp["login"] = true;
        resp["message"] = "Patient login successful";
        if (!pid.empty()) resp["patient_id"] = pid;
    } else {
        resp["login"] = false;
        resp["error"] = "Invalid credentials";
    }
    sendJsonResponse(callback, resp);
}

void AuthController::signupDoctor(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    if (AuthManager::userExists("data/doctor_credential.json", username)) {
        resp["error"] = "Username already exists";
        sendJsonResponse(callback, resp);
        return;
    }

    Doctor d(username, password);
    d.signUp();

    resp["message"] = "Doctor registered successfully";
    sendJsonResponse(callback, resp);
}

void AuthController::loginDoctor(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
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
    sendJsonResponse(callback, resp);
}
