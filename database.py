"""
إنشاء جداول قاعدة البيانات
شغّل هذا الملف مرة واحدة فقط عند البداية
"""
import psycopg2
import os
from dotenv import load_dotenv

load_dotenv()

conn = psycopg2.connect(
    host=os.getenv("DB_HOST", "localhost"),
    database=os.getenv("DB_NAME", "smarthome"),
    user=os.getenv("DB_USER", "postgres"),
    password=os.getenv("DB_PASSWORD", "1234")
)
cur = conn.cursor()

cur.execute("""
CREATE TABLE IF NOT EXISTS device_logs (
    id        SERIAL PRIMARY KEY,
    topic     TEXT,
    device    TEXT NOT NULL,
    state     TEXT NOT NULL,
    timestamp TIMESTAMPTZ DEFAULT NOW()
);

CREATE INDEX IF NOT EXISTS idx_device_time
    ON device_logs (device, timestamp DESC);
""")

conn.commit()
print("✅ Tables created successfully")
cur.close()
conn.close()
