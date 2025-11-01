#include <iostream> 
#include "user.h"

class Doctor : public User{
    string doctorID , speciality; 
    public : 
        void signUp() {
            
        }
        void login() override{
            cout << "Enter Doctor Username : " ; 
            cin >> username ; 
            cout << "Password : " ; 
            cin >> password ; 
            
            
        }
};