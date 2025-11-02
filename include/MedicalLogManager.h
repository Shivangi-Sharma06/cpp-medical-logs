#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

class MedicalLogManager {
private:
    const string LOG_FILE = "patient_logs.json";

    json loadLogs() {
        ifstream file(LOG_FILE);
        json data;
        if (file.is_open()) {
            file >> data;
            file.close();
        } else {
            data = json::array();
        }
        return data;
    }

    void saveLogs(const json &data) {
        ofstream file(LOG_FILE);
        file << setw(4) << data << endl;
        file.close();
    }

public:
    void addLog(const string &patient_id, const string &report) {
        json data = loadLogs();
        bool found = false;

        for (auto &entry : data) {
            if (entry["patient_id"] == patient_id) {
                // Append to existing logs
                entry["logs"].push_back({
                    {"date", currentDate()},
                    {"report", report}
                });
                found = true;
                break;
            }
        }

        if (!found) {
            // New patient entry
            data.push_back({
                {"patient_id", patient_id},
                {"logs", json::array({
                    {{"date", currentDate()}, {"report", report}}
                })}
            });
        }

        saveLogs(data);
        cout << "✅ Log added successfully for patient ID: " << patient_id << endl;
    }

    void viewLogs(const string &patient_id) {
        json data = loadLogs();
        for (auto &entry : data) {
            if (entry["patient_id"] == patient_id) {
                cout << "\n📜 Medical Logs for " << patient_id << ":\n";
                for (auto &log : entry["logs"]) {
                    cout << "Date: " << log["date"] << "\nReport: " << log["report"] << "\n---\n";
                }
                return;
            }
        }
        cout << "⚠️ No logs found for patient ID: " << patient_id << endl;
    }

    void updateLog(const string &patient_id, const string &newReport) {
        json data = loadLogs();
        bool updated = false;

        for (auto &entry : data) {
            if (entry["patient_id"] == patient_id) {
                entry["logs"].push_back({
                    {"date", currentDate()},
                    {"report", newReport}
                });
                updated = true;
                break;
            }
        }

        if (updated) {
            saveLogs(data);
            cout << "🩺 Log updated successfully for patient " << patient_id << endl;
        } else {
            cout << "⚠️ No existing logs found for " << patient_id << ". Try adding a log first.\n";
        }
    }

private:
    string currentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char date[11];
        sprintf(date, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return string(date);
    }
};
