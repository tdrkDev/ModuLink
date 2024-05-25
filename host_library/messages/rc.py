from messages import message
import struct

# struct modulink_message_data_rc_v1 {
#   uint8_t channels_count;
#   uint8_t rssi;
#   uint16_t reserved;
#   uint16_t channels[18];
# };

class RCMessageV1(message.MessageData):
    def __init__(self, channels_count: int, rssi: int, channels: list[int]):
        assert(len(channels) <= 18)

        self.channels_count = channels_count
        self.rssi = rssi
        self.channels = channels

    @property
    def version(self) -> int:
        return 1
    
    def build(self) -> bytes:
        fmt = "<BBxxHHHHHHHHHHHHHHHHHH"
        l = len(self.channels)
        return struct.pack(fmt,
                self.channels_count,
                self.rssi,
                self.channels[0] if l > 0 else 0,
                self.channels[1] if l > 1 else 0,
                self.channels[2] if l > 2 else 0,
                self.channels[3] if l > 3 else 0,
                self.channels[4] if l > 4 else 0,
                self.channels[5] if l > 5 else 0,
                self.channels[6] if l > 6 else 0,
                self.channels[7] if l > 7 else 0,
                self.channels[8] if l > 8 else 0,
                self.channels[9] if l > 9 else 0,
                self.channels[10] if l > 10 else 0,
                self.channels[11] if l > 11 else 0,
                self.channels[12] if l > 12 else 0,
                self.channels[13] if l > 13 else 0,
                self.channels[14] if l > 14 else 0,
                self.channels[15] if l > 15 else 0,
                self.channels[16] if l > 16 else 0,
                self.channels[17] if l > 17 else 0)
