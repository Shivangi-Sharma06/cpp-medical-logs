#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class AuthManager {
public:
    static json loadCredentials(const string &filename) {
        ifstream file(filename);
        json data;
        if (file.is_open()) {
            try { file >> data; } catch (...) { data = json::array(); }
            file.close();
        } else {
            data = json::array();
        }
        return data;
    }

    static void saveCredentials(const string &filename, const json &data) {
        ofstream file(filename);
        file << setw(4) << data << endl;
        file.close();
    }

    static bool userExists(const string &filename, const string &username) {
        json data = loadCredentials(filename);
        for (auto &user : data) {
            if (user.contains("username") && user["username"] == username) return true;
        }
        return false;
    }

    // Hash password using std::hash (prototype; replace with a secure hash for production)
    static string hashPassword(const string &password) {
        size_t h = std::hash<string>{}(password);
        return to_string(h);
    }

    static bool verifyUser(const string &filename, const string &username, const string &password) {
        json data = loadCredentials(filename);
        string hpw = hashPassword(password);
        for (auto &user : data) {
            if (user.contains("username") && user["username"] == username && user.contains("password") && user["password"] == hpw)
                return true;
        }
        return false;
    }

    static string generatePatientID(const string &filename) {
        json data = loadCredentials(filename);
        if (data.empty()) return "P1001";

        int maxID = 1000;
        for (auto &user : data) {
            if (user.contains("patient_id")) {
                string pid = user["patient_id"].get<string>();
                int idNum = stoi(pid.substr(1));
                maxID = max(maxID, idNum);
            }
        }
        return "P" + to_string(maxID + 1);
    }
};
