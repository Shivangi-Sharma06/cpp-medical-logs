#pragma once
#include "User.h"
#include "AuthManager.h"

#define PATIENT_FILE "data/patient_credential.json"

class Patient : public User {
    string patientID;
public:
    Patient(string u = "", string p = "") : User(u, p) {}

    void signUp() override {
        json data = AuthManager::loadCredentials(PATIENT_FILE);
        if (AuthManager::userExists(PATIENT_FILE, username)) {
            cout << "Patient already exists.\n";
            return;
        }
        patientID = AuthManager::generatePatientID(PATIENT_FILE);
        data.push_back({{"patient_id", patientID}, {"username", username}, {"password", password}});
        AuthManager::saveCredentials(PATIENT_FILE, data);
        cout << "Patient registered successfully. Patient ID: " << patientID << endl;
    }

    bool login() override {
        return AuthManager::verifyUser(PATIENT_FILE, username, password);
    }

    string getPatientID() const { return patientID; }
private: 
    void checklogs(string patientID) {
        
    }
};
