#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "../include/AuthManager.h"
#include "../include/MedicalLogManager.h"

int main() {
    using namespace std;
    cout << "Seeding demo data...\n";

    // Seed doctor credentials
    {
        string docFile = "data/doctor_credential.json";
        auto doctors = AuthManager::loadCredentials(docFile);
        if (doctors.empty()) {
            string username = "doc1";
            string password = "pass123";
            string hpw = AuthManager::hashPassword(password);
            doctors.push_back({{"username", username}, {"password", hpw}});
            AuthManager::saveCredentials(docFile, doctors);
            cout << "- Added doctor: " << username << " (password: " << password << ")\n";
        } else {
            cout << "- doctor credentials already present, skipping\n";
        }
    }

    // Seed patient credentials
    {
        string patFile = "data/patient_credential.json";
        auto patients = AuthManager::loadCredentials(patFile);
        if (patients.empty()) {
            string username = "patient1";
            string password = "pword";
            string hpw = AuthManager::hashPassword(password);
            string pid = AuthManager::generatePatientID(patFile);
            patients.push_back({{"patient_id", pid}, {"username", username}, {"password", hpw}});
            AuthManager::saveCredentials(patFile, patients);
            cout << "- Added patient: " << username << " (patient_id: " << pid << ", password: " << password << ")\n";
        } else {
            cout << "- patient credentials already present, skipping\n";
        }
    }

    // Seed a patient log
    {
        string pid = "P1001";
        MedicalLogManager mgr;
        // Add a sample log entry
        mgr.addLog(pid, "Initial demo log: patient registered.");
        cout << "- Added demo log for " << pid << "\n";
    }

    cout << "Seeding complete.\n";
    return 0;
}
