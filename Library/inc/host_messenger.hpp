#include <messenger.hpp>
#include <thread>

class HostMessenger : public IMavlinkMessenger {
private:
    int pFd;
    std::thread pReceiveWorker, pHeartbeatWorker;
    bool pKillFlag = false;
    bool pDebug = false;

    int pSystemId, pComponentId;
    mav_data_callback pCallback;

    void pReceiver();
    void pHeartbeatPublisher();

public:
    HostMessenger(
        std::string path,
        int system,
        int component
    );
    ~HostMessenger();

    void setDebug(bool debug);

    /// @brief Send MAVLink message to serial port
    /// @param msg MAVLink message to send
    virtual void sendMessage(const mavlink_message_t *msg);

    /// @brief Set MAVLink messages listener
    /// @param callback Callback function which gets MAVLink messages
    virtual void setOnMessageCallback(mav_data_callback callback);
};
