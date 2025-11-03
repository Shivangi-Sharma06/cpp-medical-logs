#include "DoctorController.h"
#include "../include/Doctor.h"
#include "../include/MedicalLogManager.h"
#include "../include/AuthManager.h"

void DoctorController::signup(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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
    d.signUp(); // writes to file

    resp["message"] = "Doctor registered successfully";
    callback(HttpResponse::newHttpJsonResponse(resp));
}

void DoctorController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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

void DoctorController::viewLogs(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("patient_id")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string pid = (*jsonReq)["patient_id"].asString();
    MedicalLogManager mgr;
    Json::Value logs = mgr.getLogs(pid);

    resp["patient_id"] = pid;
    resp["logs"] = logs;
    callback(HttpResponse::newHttpJsonResponse(resp));
}

void DoctorController::updateLog(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("patient_id") || !jsonReq->isMember("updated_report")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string pid = (*jsonReq)["patient_id"].asString();
    std::string updated = (*jsonReq)["updated_report"].asString();

    MedicalLogManager mgr;
    bool ok = mgr.updateLog(pid, updated);

    if (ok) resp["message"] = "Patient log updated successfully";
    else resp["error"] = "Failed to update log";

    callback(HttpResponse::newHttpJsonResponse(resp));
}
