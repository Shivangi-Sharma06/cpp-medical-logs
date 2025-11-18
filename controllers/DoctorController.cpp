#include "DoctorController.h"
#include "../include/Doctor.h"
#include "../include/MedicalLogManager.h"
#include "../include/AuthManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static inline void sendJsonResponse(const std::function<void(const HttpResponsePtr &)> &callback, const json &j) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(j.dump());
    callback(resp);
}

void DoctorController::signup(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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
    d.signUp(); // writes to file

    resp["message"] = "Doctor registered successfully";
    sendJsonResponse(callback, resp);
}

void DoctorController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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

void DoctorController::viewLogs(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("patient_id")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
        return;
    }

    std::string pid = (*jsonReq)["patient_id"].asString();
    MedicalLogManager mgr;
    json logs = json::array();
    Json::Value cppLogs = mgr.getLogs(pid);
    Json::StreamWriterBuilder wbuilder;
    std::string s = Json::writeString(wbuilder, cppLogs);
    try { logs = json::parse(s); } catch (...) { logs = json::array(); }

    resp["patient_id"] = pid;
    resp["logs"] = logs;
    sendJsonResponse(callback, resp);
}

void DoctorController::updateLog(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto jsonReq = req->getJsonObject();
    json resp;
    if (!jsonReq || !jsonReq->isMember("patient_id") || !jsonReq->isMember("updated_report")) {
        resp["error"] = "Invalid JSON input";
        sendJsonResponse(callback, resp);
        return;
    }

    std::string pid = (*jsonReq)["patient_id"].asString();
    std::string updated = (*jsonReq)["updated_report"].asString();

    MedicalLogManager mgr;
    bool ok = mgr.updateLog(pid, updated);

    if (ok) resp["message"] = "Patient log updated successfully";
    else resp["error"] = "Failed to update log";

    sendJsonResponse(callback, resp);
}
