[Uploading README.md…]()
# 🏠 Smart Home AIoT Platform

نظام منزل ذكي مبني على ESP32 مع ذكاء اصطناعي يتعلم عادات المستخدم تلقائياً.

![Python](https://img.shields.io/badge/Python-3.10+-blue)
![FastAPI](https://img.shields.io/badge/FastAPI-0.100+-green)
![MQTT](https://img.shields.io/badge/MQTT-HiveMQ-orange)
![PostgreSQL](https://img.shields.io/badge/PostgreSQL-15-blue)
![License](https://img.shields.io/badge/License-MIT-yellow)

---

## 📐 معمارية النظام

```
ESP32 Devices
     ↓
MQTT Broker (HiveMQ)
     ↓
Python Backend (subscriber.py)
     ↓
PostgreSQL Database
     ↓
AI Engine → Automation
     ↓
Web Dashboard (FastAPI)
```

---

## ✨ المميزات

- 🔌 **التحكم في الأجهزة** عبر MQTT
- 🧠 **ذكاء اصطناعي** يتعلم عادات المستخدم
- 📊 **Dashboard** لعرض البيانات لحظياً
- 📈 **Grafana** لرسوم بيانية احترافية
- ⚡ **Automation** — المنزل يتصرف وحده
- 🌐 **Web App** للتحكم من المتصفح

---

## 🛠️ المتطلبات

### Hardware
- ESP32 DevKit
- Relay Module (اختياري)
- LED أو لمبة

### Software
- Python 3.10+
- Docker Desktop
- Arduino IDE
- MQTT Explorer

---

## 🚀 التثبيت

### 1) استنساخ المشروع
```bash
git clone https://github.com/username/smart-home-aiot.git
cd smart-home-aiot
```

### 2) إنشاء البيئة الافتراضية
```bash
python -m venv venv
venv\Scripts\activate        # Windows
source venv/bin/activate     # Linux/macOS
```

### 3) تثبيت المكتبات
```bash
pip install -r requirements.txt
```

### 4) إعداد المتغيرات
```bash
cp .env.example .env
# عدّل .env بمعلوماتك
```

### 5) تشغيل PostgreSQL
```bash
docker run --name smart-home-db \
  -e POSTGRES_PASSWORD=your_password \
  -e POSTGRES_DB=smarthome \
  -p 5432:5432 -d postgres
```

### 6) إنشاء الجداول
```bash
python database.py
```

### 7) تشغيل النظام
```bash
# Terminal 1 — استقبال البيانات
python subscriber.py

# Terminal 2 — Web Dashboard
uvicorn main:app --reload
```

### 8) فتح Dashboard
```
http://localhost:8000
```

---

## 📁 هيكل المشروع

```
smart-home-aiot/
├── esp32/
│   └── smart_light.ino      # كود ESP32
├── database.py              # إنشاء الجداول
├── subscriber.py            # استقبال MQTT
├── main.py                  # FastAPI Backend
├── index.html               # Web Dashboard
├── requirements.txt
├── .env.example
├── .gitignore
└── README.md
```

---

## ⚙️ إعداد ESP32

1. افتح `esp32/smart_light.ino` في Arduino IDE
2. عدّل هذه القيم:
```cpp
const char* ssid        = "اسم_الشبكة";
const char* password    = "كلمة_المرور";
const char* mqtt_server = "broker.hivemq.com";
const char* PREFIX      = "YOUR_PREFIX"; // اسم فريد لك
```
3. ارفع الكود على ESP32

---

## 📡 MQTT Topics

| Topic | الوصف |
|-------|-------|
| `PREFIX/livingroom/light/set` | أمر تشغيل/إطفاء |
| `PREFIX/livingroom/light/state` | حالة الجهاز |
| `PREFIX/+/+/telemetry` | بيانات الاستخدام |

---

## 🤝 المساهمة

Pull Requests مرحب بها! للتغييرات الكبيرة، افتح Issue أولاً.

---

## 📄 الرخصة

MIT License — استخدم بحرية مع الإشارة للمصدر.
