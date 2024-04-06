#include <exception>
#include <iostream>
#include <host_messenger.hpp>

#include <fcntl.h>
#include <unistd.h>

void HostMessenger::pReceiver() {
    uint8_t byte = 0;
    ssize_t count = 0;
    mavlink_message_t msg;
    mavlink_status_t status;
    while (true) {
        if (pKillFlag) {
            close(pFd);
            pFd = -1;
            return;
        }
        
        count = read(pFd, &byte, 1);
        if (count == 0)
            continue;
        
        if (!mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status))
            continue;

        if (pDebug)
            std::cout << "[0]: " << std::hex << msg.msgid << std::endl;

        if (pCallback == nullptr)
            continue;

        auto buf = new mavlink_message_t;
        memcpy(buf, &msg, sizeof(msg));
        pCallback(buf);
    }
}

void HostMessenger::pHeartbeatPublisher() {
    mavlink_message_t msg;
    mavlink_msg_heartbeat_pack(
        pSystemId,
        pComponentId,
        &msg,
        MAV_TYPE_ONBOARD_CONTROLLER,
        MAV_AUTOPILOT_INVALID,
        MAV_MODE_MANUAL_ARMED,
        0,
        MAV_STATE_ACTIVE
    );
    while (true) {
        if (pKillFlag) return;
        sendMessage(&msg);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

HostMessenger::HostMessenger(
    std::string path,
    int system,
    int component
) : pSystemId(system),
    pComponentId(component)
{
    pFd = open(path.c_str(), O_RDWR);
    pReceiveWorker = std::thread(&HostMessenger::pReceiver, this);
    pHeartbeatWorker = std::thread(&HostMessenger::pHeartbeatPublisher, this);
}

HostMessenger::~HostMessenger() {
    pKillFlag = true;
    pReceiveWorker.join();
    pHeartbeatWorker.join();
}

void HostMessenger::setDebug(bool debug) {
    pDebug = debug;
}

void HostMessenger::sendMessage(const mavlink_message_t *msg) {
    auto data = std::unique_ptr<uint8_t>(new uint8_t[msg->len]);
    if (data == nullptr) return;

    auto len = mavlink_msg_to_send_buffer(data.get(), msg);
    if (!len) return;

    auto written = write(pFd, data.get(), len);
    if (written != len)
        throw "Serial port is dead";
}

void HostMessenger::setOnMessageCallback(mav_data_callback callback) {
    pCallback = callback;
}
