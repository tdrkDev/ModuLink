#include <mavlink.hpp>

class IMavlinkMessenger {
public:
    virtual ~IMavlinkMessenger() {}
    typedef void (*mav_data_callback)(mavlink_message_t *msg);
    
    /// @brief Send MAVLink message to serial port
    /// @param msg MAVLink message to send
    virtual void sendMessage(const mavlink_message_t *msg) = 0;

    /// @brief Set MAVLink messages listener.
    /// WARNING: consider converting message pointer into smart pointer!
    /// @param callback Callback function which gets MAVLink messages
    virtual void setOnMessageCallback(const mav_data_callback callback) = 0;
};
