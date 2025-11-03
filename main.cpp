#include <drogon/drogon.h>
#include "Doctor.h"
#include "Patient.h"
#include "MedicalLogManager.h"
#include "AuthManager.h"

using namespace drogon;
using namespace std;

int main() {
    // ---------- DOCTOR SIGNUP ----------
    app().registerHandler("/doctor/signup", [](const HttpRequestPtr &req,
                                               function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();

        Doctor doc(username, password);
        doc.signUp();

        resp["message"] = "Doctor registered successfully.";
        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- DOCTOR LOGIN ----------
    app().registerHandler("/doctor/login", [](const HttpRequestPtr &req,
                                              function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();

        Doctor doc(username, password);
        bool success = doc.login();

        if (success) {
            resp["login"] = true;
            resp["message"] = "Doctor login successful.";
        } else {
            resp["login"] = false;
            resp["error"] = "Invalid credentials.";
        }

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- PATIENT SIGNUP ----------
    app().registerHandler("/patient/signup", [](const HttpRequestPtr &req,
                                                function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();

        Patient pat(username, password);
        pat.signUp();

        resp["message"] = "Patient registered successfully.";
        resp["note"] = "Use your assigned patient_id from the console output.";
        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- PATIENT LOGIN ----------
    app().registerHandler("/patient/login", [](const HttpRequestPtr &req,
                                               function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("username") || !jsonReq->isMember("password")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string username = (*jsonReq)["username"].asString();
        string password = (*jsonReq)["password"].asString();

        Patient pat(username, password);
        bool success = pat.login();

        if (success) {
            resp["login"] = true;
            resp["message"] = "Patient login successful.";
            resp["patient_id"] = pat.getPatientID();
        } else {
            resp["login"] = false;
            resp["error"] = "Invalid credentials.";
        }

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- PATIENT ADD LOG ----------
    app().registerHandler("/patient/add_log", [](const HttpRequestPtr &req,
                                                 function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("patient_id") || !jsonReq->isMember("report")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string patientId = (*jsonReq)["patient_id"].asString();
        string report = (*jsonReq)["report"].asString();

        MedicalLogManager logManager;
        bool success = logManager.addLog(patientId, report);

        if (success)
            resp["message"] = "Log added successfully.";
        else
            resp["error"] = "Failed to add log.";

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- PATIENT VIEW LOGS ----------
    app().registerHandler("/patient/view_logs", [](const HttpRequestPtr &req,
                                                   function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("patient_id")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string patientId = (*jsonReq)["patient_id"].asString();

        MedicalLogManager logManager;
        Json::Value logs = logManager.getLogs(patientId);

        resp["patient_id"] = patientId;
        resp["logs"] = logs;

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- DOCTOR VIEW PATIENT LOGS ----------
    app().registerHandler("/doctor/view_logs", [](const HttpRequestPtr &req,
                                                  function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("patient_id")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string patientId = (*jsonReq)["patient_id"].asString();

        MedicalLogManager logManager;
        Json::Value logs = logManager.getLogs(patientId);

        resp["patient_id"] = patientId;
        resp["logs"] = logs;

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- DOCTOR UPDATE LOG ----------
    app().registerHandler("/doctor/update_log", [](const HttpRequestPtr &req,
                                                   function<void(const HttpResponsePtr &)> callback) {
        Json::Value resp;
        auto jsonReq = req->getJsonObject();

        if (!jsonReq || !jsonReq->isMember("patient_id") || !jsonReq->isMember("updated_report")) {
            resp["error"] = "Invalid JSON input.";
            callback(HttpResponse::newHttpJsonResponse(resp));
            return;
        }

        string patientId = (*jsonReq)["patient_id"].asString();
        string updatedReport = (*jsonReq)["updated_report"].asString();

        MedicalLogManager logManager;
        bool success = logManager.updateLog(patientId, updatedReport);

        if (success)
            resp["message"] = "Patient log updated successfully.";
        else
            resp["error"] = "Failed to update log.";

        callback(HttpResponse::newHttpJsonResponse(resp));
    });

    // ---------- FRONTEND CONNECTION ----------
    app().setDocumentRoot("./static");
    app().addListener("0.0.0.0", 8080);
    cout << "🚀 Server running at http://localhost:8080\n";
    app().run();
}
