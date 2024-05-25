from pymavlink.dialects.v20 import common as mavlink2
from pymavlink import mavutil

import serial
import time
import struct
import traceback
import random

from messages.message import *
from messages.system_status import SystemStatusMessageV1
from messages.battery import BatteryMessageV1
from messages.systime import SysTimeMessageV1
from messages.rc import RCMessageV1
from messages.rc_status import RCStatusMessageV1

from threading import Thread, Lock

class ModulinkMessenger():
    def __init__(self,
                 serial_port: str,
                 mavudp: mavutil.mavudp,
                 baudrate: int = 115200):
        self.port = serial.Serial(port=serial_port, baudrate=baudrate)
        self.mavudp = mavudp
        self.__handlers = {
            "RC_CHANNELS": self.__handle_rc_channels,
            "BATTERY_STATUS": self.__handle_battery_status,
            "HEARTBEAT": self.__handle_heartbeat,
            "SYSTEM_TIME": self.__handle_systime,
            "SYS_STATUS": self.__handle_sys_status,
        }
        self.lock = Lock()
        self.rng = random.Random()
        self.im_being_killed = False
        self.last_prearm_status = True

    def __handle_systime(self, message):
        self.__send_message(
            Message(
                typ=MODULINK_MSG_SYSTIME,
                data=SysTimeMessageV1(
                    epoch_us=message.time_unix_usec,
                    uptime_ms=message.time_boot_ms,
                )
            )
        )

    def __handle_rc_channels(self, message):
        channels = []
        for i in range(1, 19):
            channels.append(message.format_attr(f'chan{i}_raw'))

        print(f"Chan7: {channels[6]}")

        self.__send_message(
            Message(
                typ=MODULINK_MSG_RC,
                data=RCMessageV1(
                    rssi=message.rssi,
                    channels_count=message.chancount,
                    channels=channels,
                )
            )
        )

    def __handle_battery_status(self, message):
        self.__send_message(
            Message(
                typ=MODULINK_MSG_BATTERY,
                data=BatteryMessageV1(
                    current_ma=message.current_battery * 10,
                    voltage_mv=message.voltages[0],
                    remaining=message.battery_remaining,
                )
            )
        )

    def __handle_heartbeat(self, message):
        custom_mode : int = message.custom_mode
        state : int = message.system_status

        main_mode = 0
        sub_mode = 0
        main_mode, sub_mode = struct.unpack("<xxBB", struct.pack('<I', custom_mode))

        self.__send_message(
            Message(
                typ=MODULINK_MSG_SYSTEM_STATUS,
                data=SystemStatusMessageV1(
                    main_mode=main_mode,
                    sub_mode=sub_mode,
                    status=state,
                    prearm_status=self.last_prearm_status,
                )
            )
        )

    def __handle_sys_status(self, message):
        self.last_prearm_status = message.onboard_control_sensors_health & mavlink2.MAV_SYS_STATUS_PREARM_CHECK
        self.__send_message(
            Message(
                typ=MODULINK_MSG_RC_STATUS,
                data=RCStatusMessageV1(
                    present=message.onboard_control_sensors_present & mavlink2.MAV_SYS_STATUS_SENSOR_RC_RECEIVER,
                    enabled=message.onboard_control_sensors_enabled & mavlink2.MAV_SYS_STATUS_SENSOR_RC_RECEIVER,
                    healthy=message.onboard_control_sensors_health & mavlink2.MAV_SYS_STATUS_SENSOR_RC_RECEIVER,
                )
            )
        )


    def __send_message(self, message : Message):
        with self.lock:
            if self.im_being_killed:
                return
            
            self.port.write(message.build())
            self.port.flush()

    def __request_streams(self):
        streams = [
            [mavlink2.MAVLINK_MSG_ID_RC_CHANNELS, 100],
            [mavlink2.MAVLINK_MSG_ID_BATTERY_STATUS, 1000],
            [mavlink2.MAVLINK_MSG_ID_SYSTEM_TIME, 1000],
            [mavlink2.MAVLINK_MSG_ID_SYS_STATUS, 100],
        ]

        for stream in streams:
            self.__request_stream(stream[0], stream[1])

    def __request_stream(self, stream_id, interval):
        print(f"Requesting stream {stream_id}...")

        message = self.mavudp.mav.command_long_encode(
            self.mavudp.target_system,
            self.mavudp.target_component,
            mavutil.mavlink.MAV_CMD_SET_MESSAGE_INTERVAL,
            0, stream_id, interval * 1000,
            0, 0, 0, 0, 0)
        self.mavudp.mav.send(message)

        response = self.mavudp.recv_match(type='COMMAND_ACK', blocking=True)
        if response and response.command == mavutil.mavlink.MAV_CMD_SET_MESSAGE_INTERVAL and response.result == mavutil.mavlink.MAV_RESULT_ACCEPTED:
            print(f"Requested id {stream_id} stream successfully")
        else:
            print(f"Command failed: {response}")
            time.sleep(0.5)
            return self.__request_stream(stream_id, interval)

    def routine(self):
        print("Waiting for MAVLink heartbeat...")
        self.mavudp.wait_heartbeat()

        print("Starting ModuLink messenger...")
        self.__request_streams()
        print("MAVLink initialization has been finished")

        try:
            self.heartbeat_thread = Thread(
                target=self.__heartbeat_loop,
                args=())
            self.receiver_thread = Thread(target=self.__receiver,
                args=())
            self.heartbeat_thread.start()
            self.receiver_thread.start()
        except Exception as ex:
            print(f"Couldn't start main threads: ", end='')
            print(ex.args)
            print(traceback.format_exc())

        print("Started listening to the MAVLink and ModuLink connections...")
        try:
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            self.im_being_killed = True
            try:
                self.receiver_thread.join()
            except Exception:
                # Pass is acceptable here because we're killing the program
                pass

            for i in self.threads.keys():
                try:
                    self.threads[i].join()
                    self.threads.pop(i)
                except Exception:
                    pass
            try:
                self.heartbeat_thread.join()
            except Exception:
                pass

            exit(0)
            

    def __handle_msg_common(self, type, message, token):
        try:
            self.__handlers[type](message)
        finally:
            self.threads.pop(token)

    def __receiver(self):
        self.threads = {}
        try:
            while True:
                if self.im_being_killed:
                    return

                #time.sleep(0.01)
                if message := self.mavudp.recv_match():
                    type = message.get_type()
                    if type in self.__handlers:
                        token = self.rng.randint(0, 1 << 32)
                        self.threads[token] = Thread(target=self.__handle_msg_common,
                                                args=(type, message, token))
                        self.threads[token].start()
        except Exception as ex:
            print(ex.args)
            print(traceback.format_exc())
            return self.__receiver()

    def __heartbeat_loop(self):
        while True:
            if self.im_being_killed:
                return
            
            self.__send_heartbeat()
            time.sleep(1)

    def __send_heartbeat(self):
        self.mavudp.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_ONBOARD_CONTROLLER,
                                       mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
        