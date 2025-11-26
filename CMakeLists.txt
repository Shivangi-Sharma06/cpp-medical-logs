# cmake_minimum_required(VERSION 3.10)
# project(MedicalAutomationProject)

# set(CMAKE_CXX_STANDARD 17)

# find_package(Drogon REQUIRED)
# include_directories(include)

# add_executable(MedicalAutomation main.cpp
#     controllers/AuthController.cpp
#     controllers/PatientController.cpp
#     controllers/DoctorController.cpp
# )

# target_link_libraries(MedicalAutomation PRIVATE Drogon::Drogon)

cmake_minimum_required(VERSION 3.10)
project(MedicalAutomationProject)
set(CMAKE_TOOLCHAIN_FILE "D:/cpp-medical-logs/vcpkg/scripts/buildsystems/vcpkg.cmake")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Drogon
find_package(Drogon REQUIRED)

# Include all header directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/controllers
)

# Add the executable
add_executable(MedicalAutomation
    main.cpp
    controllers/AuthController.cpp
    controllers/PatientController.cpp
    controllers/DoctorController.cpp
)

# Link Drogon
target_link_libraries(MedicalAutomation PRIVATE Drogon::Drogon)
