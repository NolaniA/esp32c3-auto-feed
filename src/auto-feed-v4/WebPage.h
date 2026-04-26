#pragma once

#include <Arduino.h>

const char SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 Setup</title>

  <style>
    * {
      box-sizing: border-box;
    }

    body {
      margin: 0;
      min-height: 100vh;
      font-family: Arial, Helvetica, sans-serif;
      background:
        radial-gradient(circle at top left, #2dd4bf 0, transparent 35%),
        radial-gradient(circle at bottom right, #3b82f6 0, transparent 35%),
        linear-gradient(135deg, #0f172a, #020617);
      display: flex;
      justify-content: center;
      align-items: center;
      padding: 20px;
      color: #e5e7eb;
    }

    .card {
      width: 100%;
      max-width: 420px;
      background: rgba(15, 23, 42, 0.88);
      border: 1px solid rgba(148, 163, 184, 0.25);
      border-radius: 22px;
      padding: 28px;
      box-shadow: 0 24px 80px rgba(0, 0, 0, 0.45);
      backdrop-filter: blur(12px);
    }

    .header {
      text-align: center;
      margin-bottom: 26px;
    }

    .icon {
      width: 58px;
      height: 58px;
      margin: 0 auto 14px;
      border-radius: 18px;
      background: linear-gradient(135deg, #14b8a6, #3b82f6);
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 28px;
      box-shadow: 0 10px 30px rgba(20, 184, 166, 0.35);
    }

    h2 {
      margin: 0;
      font-size: 24px;
      letter-spacing: 0.3px;
    }

    .subtitle {
      margin-top: 8px;
      color: #94a3b8;
      font-size: 14px;
      line-height: 1.5;
    }

    .field {
      margin-bottom: 18px;
    }

    label {
      display: block;
      margin-bottom: 8px;
      font-size: 14px;
      font-weight: 600;
      color: #cbd5e1;
    }

    input {
      width: 100%;
      padding: 14px 15px;
      border-radius: 14px;
      border: 1px solid rgba(148, 163, 184, 0.35);
      background: rgba(2, 6, 23, 0.75);
      color: #f8fafc;
      font-size: 15px;
      outline: none;
      transition: 0.2s ease;
    }

    input::placeholder {
      color: #64748b;
    }

    input:focus {
      border-color: #2dd4bf;
      box-shadow: 0 0 0 4px rgba(45, 212, 191, 0.14);
    }

    .password-wrap {
      position: relative;
    }

    .password-wrap input {
      padding-right: 74px;
    }

    .toggle-btn {
      position: absolute;
      right: 8px;
      top: 50%;
      transform: translateY(-50%);
      border: none;
      background: rgba(51, 65, 85, 0.9);
      color: #cbd5e1;
      border-radius: 10px;
      padding: 8px 10px;
      font-size: 12px;
      cursor: pointer;
    }

    .save-btn {
      width: 100%;
      margin-top: 8px;
      padding: 15px;
      border: none;
      border-radius: 16px;
      background: linear-gradient(135deg, #14b8a6, #2563eb);
      color: white;
      font-size: 16px;
      font-weight: 700;
      cursor: pointer;
      transition: 0.2s ease;
      box-shadow: 0 14px 34px rgba(37, 99, 235, 0.35);
    }

    .save-btn:hover {
      transform: translateY(-1px);
      box-shadow: 0 18px 42px rgba(37, 99, 235, 0.45);
    }

    .save-btn:active {
      transform: scale(0.98);
    }

    .hint {
      margin-top: 18px;
      text-align: center;
      color: #94a3b8;
      font-size: 12px;
      line-height: 1.5;
    }

    .status {
      display: inline-flex;
      align-items: center;
      gap: 6px;
      margin-top: 10px;
      padding: 6px 10px;
      border-radius: 999px;
      background: rgba(20, 184, 166, 0.13);
      color: #5eead4;
      font-size: 12px;
      border: 1px solid rgba(45, 212, 191, 0.25);
    }

    .dot {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      background: #5eead4;
      box-shadow: 0 0 12px #5eead4;
    }

    .api-link {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      margin-top: 10px;
      padding: 8px 12px;
      border-radius: 999px;
      background: rgba(20, 184, 166, 0.12);
      border: 1px solid rgba(45, 212, 191, 0.25);
      color: #5eead4;
      font-size: 13px;
      font-weight: 600;
      text-decoration: none;
      transition: 0.2s ease;
    }

    .api-link:hover {
      background: rgba(20, 184, 166, 0.2);
      color: #99f6e4;
    }

    .clear-btn {
      width: 100%;
      margin-top: 12px;
      padding: 14px;
      border: 1px solid rgba(248, 113, 113, 0.45);
      border-radius: 16px;
      background: rgba(127, 29, 29, 0.55);
      color: #fecaca;
      font-size: 15px;
      font-weight: 700;
      cursor: pointer;
      transition: 0.2s ease;
    }

    .clear-btn:hover {
      background: rgba(185, 28, 28, 0.65);
    }

    @media (max-width: 480px) {
      body {
        padding: 14px;
        align-items: flex-start;
      }

      .card {
        margin-top: 24px;
        padding: 22px;
        border-radius: 18px;
      }

      h2 {
        font-size: 22px;
      }

      input,
      .save-btn {
        font-size: 15px;
      }
    }
  </style>
</head>

<body>
  <div class="card">
    <div class="header">
      <div class="icon">📡</div>
      <h2>ESP32 Setup</h2>
      <div class="subtitle">
        Configure WiFi and API endpoint for your device
      </div>
      <div class="status">
        <span class="dot"></span>
        Setup Mode
      </div>
    </div>

    <form action="/save" method="POST">
      <div class="field">
        <label for="ssid">WiFi SSID</label>
        <input
          id="ssid"
          name="ssid"
          type="text"
          placeholder="Enter WiFi name"
          required
        />
      </div>

      <div class="field">
        <label for="pass">WiFi Password</label>
        <div class="password-wrap">
          <input
            id="pass"
            name="pass"
            type="password"
            placeholder="Enter WiFi password"
          />
          <button class="toggle-btn" type="button" onclick="togglePassword()">
            Show
          </button>
        </div>
      </div>

      <div class="field">
        <label for="api">API URL</label>
        <input
          id="api"
          name="api"
          type="text"
          placeholder="https://example.com/api/iot/ingest"
          required
        />

        <a
          class="api-link"
          href="https://misoza.vercel.app/settings"
          target="_blank"
          rel="noopener noreferrer"
        >
          * Get API URL
        </a>
      </div>

      <button class="save-btn" type="submit">
        Save Configuration
      </button>
    </form>

    <div class="hint">
      After saving, the ESP32 will use this configuration to connect and send data.
    </div>
  </div>

  <script>
    function confirmClear() {
      return confirm("Clear saved WiFi config?");
    }

    function togglePassword() {
      const input = document.getElementById("pass");
      const btn = document.querySelector(".toggle-btn");

      if (input.type === "password") {
        input.type = "text";
        btn.innerText = "Hide";
      } else {
        input.type = "password";
        btn.innerText = "Show";
      }
    }
  </script>
</body>
</html>
)rawliteral";

const char SAVED_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      margin: 0;
      height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      background: #020617;
      color: #e5e7eb;
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .box {
      max-width: 360px;
      width: 90%;
      background: #0f172a;
      border-radius: 20px;
      padding: 28px;
      box-shadow: 0 20px 60px rgba(0,0,0,.45);
    }
    h2 { color: #5eead4; }
  </style>
</head>
<body>
  <div class="box">
    <h2>Saved</h2>
    <p>Configuration saved successfully.</p>
    <p>ESP32 is rebooting...</p>
  </div>
</body>
</html>
)rawliteral";

const char CLEARED_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      margin: 0;
      height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      background: #020617;
      color: #e5e7eb;
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .box {
      max-width: 360px;
      width: 90%;
      background: #0f172a;
      border-radius: 20px;
      padding: 28px;
      box-shadow: 0 20px 60px rgba(0,0,0,.45);
    }
    h2 { color: #5eead4; }
  </style>
</head>
<body>
  <div class="box">
    <h2>Config Cleared</h2>
    <p>WiFi SSID / Password / API URL removed.</p>
    <p>ESP32 is rebooting...</p>
  </div>
</body>
</html>
)rawliteral";
