/*
 * ═══════════════════════════════════════════════
 *  Smart Home AIoT — ESP32 Light Controller
 *  
 *  المكتبات المطلوبة:
 *    - PubSubClient  (MQTT)
 *    - ArduinoJson
 * ═══════════════════════════════════════════════
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ── إعدادات الشبكة ───────────────────────────
const char* ssid        = "اسم_الشبكة";
const char* password    = "كلمة_المرور";
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;

// ── معرّف فريد لمشروعك ──────────────────────
const char* PREFIX      = "YOUR_PREFIX";   // غيّر هذا

// ── إعداد الجهاز ────────────────────────────
const int   LED_PIN     = 2;               // LED المدمجة
const char* DEVICE_ID   = "livingroom_light";

// ── بناء Topics تلقائياً ────────────────────
char TOPIC_SET[64];
char TOPIC_TELEMETRY[64];

WiFiClient   espClient;
PubSubClient client(espClient);

// ══════════════════════════════════════════════
//  إرسال بيانات التليمتري
// ══════════════════════════════════════════════
void publishTelemetry(const char* state) {
  StaticJsonDocument<128> doc;
  doc["device"] = DEVICE_ID;
  doc["state"]  = state;

  char output[128];
  serializeJson(doc, output);
  client.publish(TOPIC_TELEMETRY, output);

  Serial.print("📤 Telemetry: ");
  Serial.println(output);
}

// ══════════════════════════════════════════════
//  معالجة الرسائل الواردة
// ══════════════════════════════════════════════
void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<64> doc;
  deserializeJson(doc, payload, length);

  String state = doc["state"].as<String>();

  if (state == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("💡 Light → ON");
    publishTelemetry("ON");

  } else if (state == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("🌙 Light → OFF");
    publishTelemetry("OFF");
  }
}

// ══════════════════════════════════════════════
//  إعادة الاتصال بـ MQTT
// ══════════════════════════════════════════════
void reconnect() {
  while (!client.connected()) {
    Serial.print("🔌 Connecting to MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" ✅ Connected!");
      client.subscribe(TOPIC_SET);
      Serial.print("📡 Subscribed: ");
      Serial.println(TOPIC_SET);
    } else {
      Serial.print(" ❌ Failed (rc=");
      Serial.print(client.state());
      Serial.println(") — retry in 5s");
      delay(5000);
    }
  }
}

// ══════════════════════════════════════════════
//  Setup
// ══════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // بناء Topics
  snprintf(TOPIC_SET,       sizeof(TOPIC_SET),
           "%s/livingroom/light/set",       PREFIX);
  snprintf(TOPIC_TELEMETRY, sizeof(TOPIC_TELEMETRY),
           "%s/livingroom/light/telemetry", PREFIX);

  // اتصال WiFi
  Serial.print("📶 Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("📍 IP: ");
  Serial.println(WiFi.localIP());

  // إعداد MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ══════════════════════════════════════════════
//  Loop
// ══════════════════════════════════════════════
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
