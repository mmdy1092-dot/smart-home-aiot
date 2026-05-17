"""
MQTT Subscriber — يستقبل بيانات الأجهزة ويحفظها في قاعدة البيانات
شغّله في Terminal منفصل ودائماً
"""
import paho.mqtt.client as mqtt
import json, psycopg2, logging, os
from dotenv import load_dotenv

load_dotenv()

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(levelname)s | %(message)s"
)

BROKER = os.getenv("MQTT_BROKER", "broker.hivemq.com")
PORT   = int(os.getenv("MQTT_PORT", 1883))
PREFIX = os.getenv("MQTT_PREFIX", "YOUR_PREFIX")
TOPIC  = f"{PREFIX}/+/+/telemetry"

conn = psycopg2.connect(
    host=os.getenv("DB_HOST", "localhost"),
    database=os.getenv("DB_NAME", "smarthome"),
    user=os.getenv("DB_USER", "postgres"),
    password=os.getenv("DB_PASSWORD", "1234")
)
cur = conn.cursor()

def on_connect(client, userdata, flags, rc):
    logging.info(f"Connected to Broker (code: {rc})")
    client.subscribe(TOPIC)
    logging.info(f"Subscribed: {TOPIC}")

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        device  = payload.get("device", "unknown")
        state   = payload.get("state",  "unknown")

        cur.execute(
            "INSERT INTO device_logs (topic, device, state) VALUES (%s, %s, %s)",
            (msg.topic, device, state)
        )
        conn.commit()
        logging.info(f"Saved → {device} = {state}")

    except Exception as e:
        logging.error(f"Error: {e}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT, 60)
client.loop_forever()
