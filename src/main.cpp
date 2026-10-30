#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // 引入 WiFiManager

// 網頁服務器在 80 端口監聽
WebServer server(80);

// ========== 震動模式模擬函數 ==========
void simulateVibrationPattern1() {
  Serial.println(">>> 執行：模擬震動模式 1 (短促) <<<");
}

void simulateVibrationPattern2() {
  Serial.println(">>> 執行：模擬震動模式 2 (連續) <<<");
}

void simulateVibrationPattern3() {
  Serial.println(">>> 執行：模擬震動模式 3 (警報) <<<");
}

// ========== 網頁處理函數 ==========
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<meta charset=\"UTF-8\">"; // 確保中文顯示正常
  html += "<title>AI安全帽 控制面板</title>";
  html += "<style>body{font-family:sans-serif;text-align:center;margin-top:50px;background:#f4f4f9;}";
  html += ".btn{background:#007BFF;color:white;padding:15px 30px;text-decoration:none;display:block;";
  html += "font-size:18px;margin:15px auto;width:80%;max-width:300px;border-radius:10px;border:none;}";
  html += ".btn:active{background:#0056b3;}";
  html += "h1{color:#333;}</style></head><body>";
  html += "<h1>AI 安全帽模擬控制</h1>";
  html += "<p>狀態：已連線至網路</p>";
  html += "<a href=\"/motor1\" class=\"btn\">模式 1 (短促)</a>";
  html += "<a href=\"/motor2\" class=\"btn\">模式 2 (連續)</a>";
  html += "<a href=\"/motor3\" class=\"btn\">模式 3 (警報)</a>";
  html += "<p><small>點擊按鈕後請查看 VS Code 序列埠監控視窗</small></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleMotor1() {
  simulateVibrationPattern1();
  server.send(200, "text/plain", "Mode 1 Triggered");
}
void handleMotor2() {
  simulateVibrationPattern2();
  server.send(200, "text/plain", "Mode 2 Triggered");
}
void handleMotor3() {
  simulateVibrationPattern3();
  server.send(200, "text/plain", "Mode 3 Triggered");
}

void setup() {
  Serial.begin(115200);
  delay(1000); // 給序列埠一點時間初始化
  Serial.println("\n--- AI 安全帽系統啟動 ---");

  // 初始化 WiFiManager
  WiFiManager wm;

  // 如果你需要清除已儲存的 Wi-Fi 密碼來測試配網過程，可以取消註解下面這行
  // wm.resetSettings();

  // 自訂配網時的熱點名稱。如果它連不上已知網路，就會建立這個熱點。
  // autoConnect 會嘗試連接已保存的網路。如果失敗，則啟動 AP 模式並開啟配網網頁。
  Serial.println("正在嘗試連線 Wi-Fi，或啟動設定熱點...");
  bool res = wm.autoConnect("AI-Helmet-Setup"); // 熱點名稱為 "AI-Helmet-Setup"

  if(!res) {
    Serial.println("連線失敗，請檢查網路或重新設定");
    // ESP.restart(); // 失敗時可以選擇重啟
  } else {
    // 如果走到這裡，表示已經成功連上 Wi-Fi
    Serial.println("\n✅ Wi-Fi 連線成功!");
    Serial.print("IP 位址: ");
    Serial.println(WiFi.localIP());

    // 設定網頁路由
    server.on("/", handleRoot);
    server.on("/motor1", handleMotor1);
    server.on("/motor2", handleMotor2);
    server.on("/motor3", handleMotor3);

    // 啟動伺服器
    server.begin();
    Serial.println("🌐 網頁伺服器已啟動");
  }
}

void loop() {
  // 處理進來的 HTTP 請求
  server.handleClient();
}