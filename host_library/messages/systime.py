from messages import message
import struct

# struct modulink_message_data_systime_v1 {
#   uint64_t epoch_us;
#   uint32_t uptime_ms;
# };

class SysTimeMessageV1(message.MessageData):
    def __init__(self, epoch_us : int, uptime_ms : int):
        self.epoch_us = epoch_us
        self.uptime_ms = uptime_ms

    @property
    def version(self) -> int:
        return 1
    
    def build(self) -> bytes:
        fmt = "<QI"
        return struct.pack(fmt,
                self.epoch_us,
                self.uptime_ms)
