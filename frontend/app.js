const defaultApiBase = (() => {
    const isHttp = window.location.protocol.startsWith("http");
    return isHttp ? window.location.origin : "http://localhost:8080";
})();

let apiBase = window.API_BASE_URL || defaultApiBase;

const state = {
    patientId: "",
    lastDoctorLogin: null
};

const $ = (selector) => document.querySelector(selector);

function setApiBase(newBase) {
    apiBase = (newBase && newBase.trim()) || defaultApiBase;
    $("#apiBaseInput").value = apiBase;
}

function prettify(json) {
    try {
        return JSON.stringify(json, null, 2);
    } catch {
        return String(json);
    }
}

async function callEndpoint(endpoint, payload, responseEl) {
    responseEl.textContent = "⏳ Sending request...";
    try {
        const res = await fetch(`${apiBase}${endpoint}`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(payload)
        });
        const data = await res.json();
        responseEl.textContent = prettify(data);
        return data;
    } catch (error) {
        responseEl.textContent = prettify({ error: error.message, endpoint, payload });
        throw error;
    }
}

function bindForm(formId, responseId, endpoint, payloadBuilder, onSuccess) {
    const form = document.getElementById(formId);
    const responseEl = document.getElementById(responseId);
    form.addEventListener("submit", async (event) => {
        event.preventDefault();
        const formData = Object.fromEntries(new FormData(form).entries());
        try {
            const builder = payloadBuilder || ((d) => d);
            const payload = builder(formData);
            const result = await callEndpoint(endpoint, payload, responseEl);
            responseEl.dataset.success = "true";
            if (typeof onSuccess === "function") onSuccess(result, formData, form);
            return result;
        } catch {
            responseEl.dataset.success = "false";
        }
    });
}

function renderServerStatus(text, ok = false) {
    const pill = $("#serverStatus");
    pill.textContent = text;
    pill.style.background = ok ? "#dcfce7" : "#fee2e2";
    pill.style.color = ok ? "#15803d" : "#b91c1c";
}

document.addEventListener("DOMContentLoaded", () => {
    setApiBase(apiBase);

    $("#apiBaseInput").addEventListener("change", (e) => setApiBase(e.target.value));

    $("#pingApi").addEventListener("click", async () => {
        try {
            renderServerStatus("Pinging...");
            const ping = await fetch(`${apiBase}/patient/view_logs`, {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ patient_id: "demo" })
            });
            if (ping.ok) renderServerStatus("API reachable", true);
            else renderServerStatus(`API error ${ping.status}`, false);
        } catch (err) {
            renderServerStatus(`API unreachable: ${err.message}`, false);
        }
    });

    bindForm("patientSignupForm", "patientSignupResp", "/patient/signup");

    bindForm("patientLoginForm", "patientLoginResp", "/patient/login", null, (result) => {
        if (result.patient_id) {
            state.patientId = result.patient_id;
            document.querySelectorAll('input[name="patient_id"]').forEach((input) => {
                input.value = state.patientId;
            });
        }
    });

    bindForm("patientAddLogForm", "patientAddLogResp", "/patient/add_log", (data) => ({
        patient_id: data.patient_id || state.patientId,
        report: data.report
    }));

    bindForm("patientViewLogsForm", "patientViewLogsResp", "/patient/view_logs", (data) => ({
        patient_id: data.patient_id || state.patientId
    }));

    bindForm("doctorSignupForm", "doctorSignupResp", "/doctor/signup");

    bindForm("doctorLoginForm", "doctorLoginResp", "/doctor/login", null, (result) => {
        if (result.login) state.lastDoctorLogin = new Date().toISOString();
    });

    bindForm("doctorViewLogsForm", "doctorViewLogsResp", "/doctor/view_logs");

    bindForm("doctorUpdateLogForm", "doctorUpdateLogResp", "/doctor/update_log");
});

