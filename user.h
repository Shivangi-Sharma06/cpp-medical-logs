#include <iostream>
#include <vector>
#include <string>
#include <fstream> 

#ifndef USER_H 
#define USER_H

using namespace std; 

class User {
    protected: 
        string username ; 
        string password ; 
    public : 
        virtual void login() = 0 ; 
        virtual void menu() = 0 ;
        string getUsername() {return username; }
}; 

class Patient : public User{
    string patientID ; 
    public : 
        Patient(string id) {patientID = id ; }
        void login() override {
            cout << "Enter patient username : " ; 
            cin >> username ; 
            cout << "Enter Password : " ; 
            cin >> password ; 
        }

        void menu() override{
            int choice ; 
            do { 
                cout << "\nMenu : \n" ; 
                cout << "\nPatient Menu: \n";
                cout << "1. Add/View Medical Logs\n";
                cout << "2. Book Appointment\n";
                cout << "3. Logout\n";
                cin >> choice;
                switch(choice) {
                    case 1:
                        addViewLogs();
                        break;
                    case 2:
                        bookAppointment();
                        break;
            }
            }while( choice != 3) ;
        }
    void addViewLogs() {
        string logFile = patientID + "_logs.txt";
        cout << "1. Add Log\n2. View Logs\n";
        int c;
        cin >> c;
        if(c == 1) {
            ofstream fout(logFile, ios::app);
            cout << "Enter medication or log: ";
            string log;
            cin.ignore();
            getline(cin, log);
            fout << log << endl;
            fout.close();
        } else {
            ifstream fin(logFile);
            string line;
            cout << "Medical Logs:\n";
            while(getline(fin, line)) {
                cout << line << endl;
            }
            fin.close();
        }
    }
    void bookAppointment() {
        string docID;
        cout << "Enter Doctor ID: ";
        cin >> docID;
        ofstream fout("appointments.txt", ios::app);
        fout << patientID << "," << docID << endl;
        fout.close();
        cout << "Appointment Booked.\n";
    }
    string getPatientID() { return patientID; }
};

class Doctor : public User{
    string doctorID , speciality; 
};

#endif