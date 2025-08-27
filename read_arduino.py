import serial

# Change '/dev/ttyACM0' to your Arduino's serial port if needed
SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
            while True:
                line = ser.readline().decode('utf-8', errors='replace').strip()
                if line:
                    print(line)
    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == '__main__':
    main()