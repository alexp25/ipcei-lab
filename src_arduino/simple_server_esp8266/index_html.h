#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 Numeric Display</title>
  <style>
    * {
      box-sizing: border-box;
    }

    body {
      margin: 0;
      min-height: 100vh;
      display: grid;
      place-items: center;
      font-family: Arial, Helvetica, sans-serif;
      color: #1a1f2b;
      background: #eef3f8;
    }

    main {
      width: min(92vw, 420px);
      padding: 28px;
      background: #ffffff;
      border: 1px solid #d7e0ea;
      border-radius: 8px;
      box-shadow: 0 16px 40px rgba(29, 43, 68, 0.12);
    }

    h1 {
      margin: 0 0 18px;
      font-size: 22px;
      font-weight: 700;
    }

    .display {
      width: 100%;
      min-height: 132px;
      display: grid;
      place-items: center;
      border: 1px solid #b8c6d6;
      border-radius: 6px;
      background: #101820;
      color: #67f5a6;
      font-size: clamp(48px, 18vw, 84px);
      font-weight: 700;
      line-height: 1;
      font-variant-numeric: tabular-nums;
    }

    .status {
      min-height: 20px;
      margin-top: 12px;
      color: #5c6878;
      font-size: 14px;
    }

    .chat {
      width: 100%;
      margin-top: 24px;
      padding-top: 20px;
      border-top: 1px solid #d7e0ea;
    }

    .chat h2 {
      margin: 0 0 10px;
      font-size: 15px;
      font-weight: 700;
      color: #5c6878;
    }

    .chat-messages {
      height: 160px;
      overflow-y: auto;
      border: 1px solid #b8c6d6;
      border-radius: 6px;
      padding: 8px 10px;
      background: #f7fafc;
      font-size: 13px;
      line-height: 1.5;
    }

    .chat-messages div {
      word-break: break-word;
    }

    .chat-form {
      display: flex;
      gap: 8px;
      margin-top: 10px;
    }

    .chat-form input {
      flex: 1;
      min-width: 0;
      padding: 8px 10px;
      border: 1px solid #b8c6d6;
      border-radius: 6px;
      font-size: 14px;
    }

    .chat-form button {
      padding: 8px 16px;
      border: none;
      border-radius: 6px;
      background: #1a1f2b;
      color: #ffffff;
      font-size: 14px;
      cursor: pointer;
    }
  </style>
</head>
<body>
  <main>
    <h1>ESP8266 Numeric Display</h1>
    <div id="display" class="display">0</div>
    <div id="status" class="status"></div>

    <div class="chat">
      <h2>General Chat (MQTT /general)</h2>
      <div id="chatMessages" class="chat-messages"></div>
      <form id="chatForm" class="chat-form">
        <input id="chatInput" type="text" maxlength="96" placeholder="Type a message" autocomplete="off">
        <button type="submit">Send</button>
      </form>
    </div>
  </main>

  <script>
    const display = document.getElementById('display');
    const status = document.getElementById('status');
    const chatMessages = document.getElementById('chatMessages');
    const chatForm = document.getElementById('chatForm');
    const chatInput = document.getElementById('chatInput');

    async function loadValue() {
      const response = await fetch('/value');
      if (!response.ok) {
        throw new Error('Unable to read value');
      }

      const data = await response.json();
      display.textContent = Number(data.value).toFixed(2);
      status.textContent = 'Updated from UART';
    }

    async function loadChat() {
      const response = await fetch('/chat');
      if (!response.ok) {
        throw new Error('Unable to read chat');
      }

      const messages = await response.json();
      const atBottom = chatMessages.scrollTop + chatMessages.clientHeight >= chatMessages.scrollHeight - 4;

      chatMessages.innerHTML = '';
      messages.forEach((message) => {
        const line = document.createElement('div');
        line.textContent = message;
        chatMessages.appendChild(line);
      });

      if (atBottom) {
        chatMessages.scrollTop = chatMessages.scrollHeight;
      }
    }

    chatForm.addEventListener('submit', async (event) => {
      event.preventDefault();
      const text = chatInput.value.trim();
      if (text.length === 0) {
        return;
      }

      chatInput.value = '';
      try {
        await fetch('/chat', { method: 'POST', body: text });
      } catch (error) {
        status.textContent = error.message;
      }
    });

    loadValue().catch((error) => {
      status.textContent = error.message;
    });

    loadChat().catch((error) => {
      status.textContent = error.message;
    });

    setInterval(() => {
      loadValue().catch((error) => {
        status.textContent = error.message;
      });
      loadChat().catch((error) => {
        status.textContent = error.message;
      });
    }, 500);
  </script>
</body>
</html>
)rawliteral";
