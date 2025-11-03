#include "PatientController.h"
#include "../include/Patient.h"
#include "../include/AuthManager.h"
#include "../include/MedicalLogManager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Helper: find patient_id in credentials by username
static std::string findPatientIdByUsername(const std::string &username) {
    const std::string file = "data/patient_credential.json";
    json users = AuthManager::loadCredentials(file);
    for (auto &u : users) {
        if (u.contains("username") && u["username"] == username) {
            if (u.contains("patient_id")) return u["patient_id"].get<std::string>();
        }
    }
    return std::string();
}

void PatientController::signup(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string username = (*jsonReq)["username"].asString();
    std::string password = (*jsonReq)["password"].asString();

    // If username exists -> reject
    if (AuthManager::userExists("data/patient_credential.json", username)) {
        resp["error"] = "Username already exists";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    Patient p(username, password);
    p.signUp(); // writes the patient_id to file

    // Retrieve the generated ID from file
    std::string patientId = findPatientIdByUsername(username);
    resp["message"] = "Patient registered successfully";
    if (!patientId.empty()) resp["patient_id"] = patientId;
    else resp["note"] = "Registered but patient_id not found (check server console/data file)";

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void PatientController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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
        // After successful login, load patient_id from file
        std::string pid = findPatientIdByUsername(username);
        resp["login"] = true;
        resp["message"] = "Patient login successful";
        if (!pid.empty()) resp["patient_id"] = pid;
    } else {
        resp["login"] = false;
        resp["error"] = "Invalid credentials";
    }

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void PatientController::addLog(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value resp;
    auto jsonReq = req->getJsonObject();
    if (!jsonReq || !jsonReq->isMember("patient_id") || !jsonReq->isMember("report")) {
        resp["error"] = "Invalid JSON input";
        callback(HttpResponse::newHttpJsonResponse(resp));
        return;
    }

    std::string pid = (*jsonReq)["patient_id"].asString();
    std::string report = (*jsonReq)["report"].asString();

    MedicalLogManager mgr;
    bool ok = mgr.addLog(pid, report);
    if (ok) resp["message"] = "Log added successfully";
    else resp["error"] = "Failed to add log (invalid patient_id?)";

    callback(HttpResponse::newHttpJsonResponse(resp));
}

void PatientController::viewLogs(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
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
