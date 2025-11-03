cmake_minimum_required(VERSION 3.10)
project(MedicalAutomationProject)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)
include_directories(include)

add_executable(MedicalAutomation main.cpp
    controllers/AuthController.cpp
    controllers/PatientController.cpp
    controllers/DoctorController.cpp
)

target_link_libraries(MedicalAutomation PRIVATE Drogon::Drogon)
