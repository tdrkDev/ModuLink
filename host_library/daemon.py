from pymavlink.dialects.v20 import common as mavlink2
from pymavlink import mavutil
import time
import traceback

import messenger

# Module serial port settings
ARD_PORT = "/dev/ttyS0"
ARD_BAUD = 115200

# MAVLink UDP connection settings
MAV_TARGET = "127.0.0.1:14560"

msgr = messenger.ModulinkMessenger(
    serial_port=ARD_PORT,
    baudrate=ARD_BAUD,
    mavudp=mavutil.mavudp(
        MAV_TARGET,
        input=True,
        source_component=mavlink2.MAV_COMP_ID_ONBOARD_COMPUTER,
        source_system=1),
)

def main():
    print("Starting MAVLink-to-ModuLink daemon...")
    try:
        msgr.routine()
    except Exception as e:
        print("Messenger has crashed: restarting in 1 second...")
        print(e.args)
        print(traceback.format_exc())
        time.sleep(1)
        return main()

if __name__ == "__main__":
    main()
