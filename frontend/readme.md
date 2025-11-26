## Medical Logs Frontend

The `frontend/` directory contains a static dashboard (`index.html`, `styles.css`, `app.js`) that exercises **all** public REST endpoints exposed by the Drogon backend:

- `POST /patient/signup`
- `POST /patient/login`
- `POST /patient/add_log`
- `POST /patient/view_logs`
- `POST /doctor/signup`
- `POST /doctor/login`
- `POST /doctor/view_logs`
- `POST /doctor/update_log`

### Running the frontend

1. Build & start the backend (serves on `http://localhost:8080` by default).
2. Ensure `config.json` points `document_root` to `./frontend` (already set).
3. Visit `http://localhost:8080/index.html` or open `frontend/index.html` directly in a browser.

The “API base URL” field lets you target a different backend host/port if needed. Each form sends a JSON payload via `fetch` and renders the raw server response for quick testing.

