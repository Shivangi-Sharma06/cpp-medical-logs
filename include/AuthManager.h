#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

class AuthManager {
public:
    static json loadCredentials(const string &filename) {
        ifstream file(filename);
        json data;
        if (file.is_open()) {
            file >> data;
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
            if (user["username"] == username) return true;
        }
        return false;
    }

    static bool verifyUser(const string &filename, const string &username, const string &password) {
        json data = loadCredentials(filename);
        for (auto &user : data) {
            if (user["username"] == username && user["password"] == password)
                return true;
        }
        return false;
    }

    static string generatePatientID(const string &filename) {
        json data = loadCredentials(filename);
        if (data.empty()) return "P1001";

        int maxID = 1000;
        for (auto &user : data) {
            string pid = user["patient_id"];
            int idNum = stoi(pid.substr(1));
            maxID = max(maxID, idNum);
        }
        return "P" + to_string(maxID + 1);
    }
};
