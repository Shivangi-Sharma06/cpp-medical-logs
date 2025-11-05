#include <iostream> 
#include "User.h"
#include "AuthManager.h"
#include "MedicalLogManager.h"

#define DOCTOR_FILE "data/doctor_credential.json" 

class Doctor : public User{
    string doctorID , speciality; 
    public :  
        Doctor(string u = "" , string p = "") : User(u , p) {}
        void signUp() override{
            json data = AuthManager::loadCredentials(DOCTOR_FILE) ; 
            if(AuthManager::userExists(DOCTOR_FILE , username)){
                cout << "Doctor already Exists \n" ; 
                return ; 
            }
            data.push_back({{"username" , username} , {"password" , password}});
            AuthManager::saveCredentials(DOCTOR_FILE , data) ; 
            cout << "Doctor Registered Successfully\n" ; 
        }
        bool login() override{
            return AuthManager::verifyUser(DOCTOR_FILE , username , password) ;     
        }
        void managePatientLogs(){
            MedicalLogManager logManager ; 
            int choice ; 
            string pid ; 
            cout << "\n1. View Patient Logs\n2. Update Patient Logs\nEnter choice : " ;
            cin >> choice ; 
            cout << "Enter the PatientID" ; 
            cin >> pid ; 
            cin.ignore() ;//Main thing to remember 

            if(choice == 1 ){
                logManager.viewLogs(pid) ; 
            } else if (choice ==2 ) {
                string report ; 
                cout << "Update Record of " << pid << endl ; 
                getline(cin , report) ; 
                logManager.updateLog(pid , report);
            } else {
                cout << "Invalid choice\n";
            }
        }
};