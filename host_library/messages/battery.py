from messages import message
import struct

# struct modulink_message_data_battery_v1 {
#   int8_t remaining;
#   int32_t current_ma;
#   uint16_t voltage_mv;
#   uint8_t reserved;
# };

class BatteryMessageV1(message.MessageData):
    def __init__(self, remaining : int, current_ma : int, voltage_mv : int):
        self.remaining = remaining
        self.current_ma = current_ma
        self.voltage_mv = voltage_mv

    @property
    def version(self) -> int:
        return 1
    
    def build(self) -> bytes:
        fmt = "<biHxxx"
        return struct.pack(fmt,
                self.remaining,
                self.current_ma,
                self.voltage_mv)
