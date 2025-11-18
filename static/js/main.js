// Minimal frontend wiring to call backend patient endpoints
const $ = id => document.getElementById(id);

function showOutput(obj) {
  const out = typeof obj === 'string' ? obj : JSON.stringify(obj, null, 2);
  $('output').textContent = out;
}

async function postJson(path, body) {
  const res = await fetch(path, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });
  return res.json();
}

document.addEventListener('DOMContentLoaded', () => {
  $('signup-patient').addEventListener('click', async () => {
    const username = $('p-username').value.trim();
    const password = $('p-password').value.trim();
    if (!username || !password) { showOutput('Enter username and password'); return; }
    try {
      const data = await postJson('/patient/signup', { username, password });
      showOutput(data);
      if (data.patient_id) {
        $('patient-id').value = data.patient_id;
        $('patient-actions').style.display = 'flex';
      }
    } catch (e) { showOutput('Request failed: ' + e); }
  });

  $('login-patient').addEventListener('click', async () => {
    const username = $('p-username').value.trim();
    const password = $('p-password').value.trim();
    if (!username || !password) { showOutput('Enter username and password'); return; }
    try {
      const data = await postJson('/patient/login', { username, password });
      showOutput(data);
      if (data.login && data.patient_id) {
        $('patient-id').value = data.patient_id;
        $('patient-actions').style.display = 'flex';
      }
    } catch (e) { showOutput('Request failed: ' + e); }
  });

  $('add-log').addEventListener('click', async () => {
    const patient_id = $('patient-id').value.trim();
    const report = $('patient-report').value.trim();
    if (!patient_id || !report) { showOutput('Provide patient_id and report'); return; }
    try {
      const data = await postJson('/patient/add_log', { patient_id, report });
      showOutput(data);
    } catch (e) { showOutput('Request failed: ' + e); }
  });

  $('view-logs').addEventListener('click', async () => {
    const patient_id = $('patient-id').value.trim();
    if (!patient_id) { showOutput('Provide patient_id'); return; }
    try {
      const data = await postJson('/patient/view_logs', { patient_id });
      showOutput(data);
    } catch (e) { showOutput('Request failed: ' + e); }
  });
});
