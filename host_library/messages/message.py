import struct

MODULINK_START_MAGIC   = 0x4badbeef
MODULINK_CHECKSUM_BASE = 0xaa55f00f

MODULINK_MSG_BASE_ID = 0x3000
MODULINK_MSG_RC = 0x3001
MODULINK_MSG_BATTERY = 0x3002
MODULINK_MSG_SYSTIME = 0x3003
MODULINK_MSG_SYSTEM_STATUS = 0x3004
MODULINK_MSG_RC_STATUS = 0x3005
MODULINK_MSG_MAX_ID = 0x3006

class MessageData():
    def build(self) -> bytes:
        pass

    @property
    def version(self) -> int:
        pass

class Message():
    def __init__(self, typ : int, data : MessageData):
        self.magic = MODULINK_START_MAGIC
        self.typ = typ
        self.data = data

    @property
    def checksum(self) -> int:
        data = self.data.build()
        t = data[0]
        for i in range(1, len(data)):
            t ^= data[i]
        return MODULINK_CHECKSUM_BASE ^ t;

    def build(self) -> bytes:
        header_fmt = "<IHBHIxxx"
        data = self.data.build()
        header = struct.pack(header_fmt,
                             self.magic,
                             self.typ,
                             self.data.version,
                             len(data),
                             self.checksum)
        return header + data
