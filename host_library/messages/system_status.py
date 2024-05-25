from messages import message
import struct

# struct modulink_message_data_system_status_v1 {
#   uint8_t last_prearm_status;
#   uint8_t status;
#   uint8_t main_mode;
#   uint8_t sub_mode;
# };

class SystemStatusMessageV1(message.MessageData):
    def __init__(self, main_mode : int, sub_mode : int, status : int, prearm_status: bool):
        self.main_mode = main_mode
        self.sub_mode = sub_mode
        self.status = status
        self.last_prearm_status = prearm_status

    @property
    def version(self) -> int:
        return 1
    
    def build(self) -> bytes:
        fmt = "<BBBB"
        return struct.pack(fmt,
                1 if self.last_prearm_status else 0,
                self.status,
                self.main_mode,
                self.sub_mode)
