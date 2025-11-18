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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
# Find packages
find_package(Drogon REQUIRED)
find_package(nlohmann_json REQUIRED)

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

# Link libraries
target_link_libraries(MedicalAutomation PRIVATE
    Drogon::Drogon
    nlohmann_json::nlohmann_json
)

# Add seed tool
add_executable(seed_data
    tools/seed.cpp
)

target_link_libraries(seed_data PRIVATE nlohmann_json::nlohmann_json)

# If you are using vcpkg on Windows, configure CMake with the vcpkg toolchain, e.g.:
# cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
