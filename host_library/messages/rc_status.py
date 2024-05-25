from messages import message
import struct

# struct modulink_message_data_rc_status_v1 {
#   uint8_t present;
#   uint8_t enabled;
#   uint8_t healthy;
#   uint8_t reserved;
# };

class RCStatusMessageV1(message.MessageData):
    def __init__(self, present: bool, healthy: bool, enabled: bool):
        self.present = present
        self.healthy = healthy
        self.enabled = enabled

    @property
    def version(self) -> int:
        return 1
    
    def build(self) -> bytes:
        fmt = "<BBBx"
        return struct.pack(fmt,
                1 if self.present else 0,
                1 if self.enabled else 0,
                1 if self.healthy else 0)
