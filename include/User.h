#ifndef MEDICAL_LOGS_USER_H
#define MEDICAL_LOGS_USER_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream> 


using namespace std; 

class User {
    protected: 
        string username ; 
        string password ; 
    public : 
    User(string u = "" , string p = "") : username(u) , password(p) {}
        virtual bool login() = 0 ; 
        virtual void signUp() = 0 ; 
        string getUsername() {return username; }
}; 

#endif // MEDICAL_LOGS_USER_H
