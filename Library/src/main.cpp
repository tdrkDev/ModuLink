#include <iostream>
#include <host_messenger.hpp>
#include <fstream>
#include <json11.hpp>

static std::ofstream *arduino;

static void sendModulinkMessage(std::string type, json11::Json::object data) {
    auto json = json11::Json({
        {"type", type},
        {"data", data},
    });
    std::string jsons;
    json.dump(jsons);
    if (arduino == nullptr) {
        std::cout << "Arduino port is null" << std::endl;
        return;
    }
    arduino->write(jsons.c_str(), jsons.length());
}

static void callback(mavlink_message_t *msg) {
    switch (msg->msgid) {
    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
        mavlink_rc_channels_raw_t raw;
        mavlink_msg_rc_channels_raw_decode(msg, &raw);
        sendModulinkMessage("rc", {
            {"channels", json11::Json::array{
                raw.chan1_raw,
                raw.chan2_raw,
                raw.chan3_raw,
                raw.chan4_raw,
                raw.chan5_raw,
                raw.chan6_raw,
                raw.chan7_raw,
                raw.chan8_raw
            }},
            {"rssi", raw.rssi},
        });
        sendModulinkMessage("uptime", {
            {"ms", static_cast<int>(raw.time_boot_ms)},
        });
    default:
        break;
    }
    delete msg;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [MAV serial port] [Arduino serial port]" << std::endl;
        return 1;
    }

    std::cout << "ModuLink daemon is starting..." << std::endl;

    auto messenger = HostMessenger(
        std::string(argv[1]),
        0, MAV_COMP_ID_ONBOARD_COMPUTER
    );
    messenger.setOnMessageCallback(callback);

    arduino = new std::ofstream(argv[2]);

    while (true)
        std::this_thread::sleep_for(std::chrono::hours(1));
}
