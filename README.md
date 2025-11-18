cpp-medical-logs

Overview:
Simple prototype: C++ (Drogon) backend that stores credentials and patient logs in JSON files, with a small static frontend served from `static/`.

Build & Run (Windows, recommended with vcpkg):

Prerequisites:
- CMake (>= 3.10)
- A C++17 toolchain (Visual Studio or MSVC on Windows)
- vcpkg (recommended) to install dependencies

Install dependencies with vcpkg (example):

    vcpkg install drogon nlohmann-json

Build with CMake (from repo root):

    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
    cmake --build . --config Release

Run the server (after build):

    # Example path to built exe (varies by generator and config)
    .\Release\MedicalAutomation.exe

The server listens on port 8080 by default and serves static files from ./static.

Notes:
- The project uses a prototype password hashing (not production-secure). For demonstration this is acceptable; replace with bcrypt/argon2 for real systems.
- The backend now standardizes responses to `nlohmann::json` when possible so JsonCpp is not required for controllers.
- If CMake cannot find packages, ensure vcpkg is installed and use the `-DCMAKE_TOOLCHAIN_FILE` argument.

API Endpoints (examples):

- POST /patient/signup
    - body: { "username": "patient1", "password": "pword" }
- POST /patient/login
    - body: { "username": "patient1", "password": "pword" }
- POST /patient/add_log
    - body: { "patient_id": "P1001", "report": "some report text" }
- POST /patient/view_logs
    - body: { "patient_id": "P1001" }

Sample curl (signup -> add log -> view logs):

```bash
# Signup
curl -X POST http://localhost:8080/patient/signup -H "Content-Type: application/json" -d '{"username":"patient1","password":"pword"}'

# Add a log (use patient_id returned from signup/login)
curl -X POST http://localhost:8080/patient/add_log -H "Content-Type: application/json" -d '{"patient_id":"P1001","report":"Demo entry"}'

# View logs
curl -X POST http://localhost:8080/patient/view_logs -H "Content-Type: application/json" -d '{"patient_id":"P1001"}'
```

Next steps:
- Standardize JSON library usage.
- Add password hashing and session tokens.
- Improve frontend UX (demo exists in static/).
