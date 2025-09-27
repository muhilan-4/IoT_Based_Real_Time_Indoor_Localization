import serial
import csv
from datetime import datetime

# Replace with your actual COM port
COM_PORT = "COM13"  # e.g., "COM3" on Windows or "/dev/ttyUSB0" on Linux/Mac
BAUD_RATE = 115200
OUTPUT_FILE = "beacon_data.csv"

def main():
    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        print(f"[INFO] Connected to {COM_PORT} at {BAUD_RATE} baud.")
        
        with open(OUTPUT_FILE, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(["Beacon1", "Beacon2", "Beacon3", "Beacon4", "Beacon5", "Beacon6", "Room", "Timestamp"])

            while True:
                line = ser.readline().decode('utf-8').strip()
                if line and "Beacon1" not in line:  # Skip header line from ESP32
                    print("[DATA]", line)
                    parts = [x.strip() for x in line.split(',')]
                    if len(parts) == 7:  # Expecting 6 RSSIs + Room
                        parts.append(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
                        writer.writerow(parts)
    except KeyboardInterrupt:
        print("\n[INFO] Data collection stopped by user.")
    except Exception as e:
        print(f"[ERROR] {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
