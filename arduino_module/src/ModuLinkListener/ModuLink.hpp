#include "modulink_data.h"

#ifdef __AVR__
#include <ArduinoSTL.h>
#endif

#include <map>
#include <stdarg.h>

namespace modulink {

class ModuLinkListener {
public:
    typedef enum {
        MH_SUCCESS = 0,
        MH_INTERNAL_ERROR,
        MH_ERROR_MESSAGE_INVALID,
        MH_ERROR_MESSAGE_UNSUPPORTED,
    } handler_status_t;

    typedef enum {
        PAR_SUCCESS = 0,
        PAR_INVALID_MAGIC,
        PAR_INVALID_CHECKSUM,
        PAR_UNSUPPORTED_TYPE,
    } parsing_error_t;

    typedef handler_status_t (*handler_t)(uint8_t version, uint16_t length, void *data);
    typedef int (*log_handler_t)(const char *fmt, va_list *args);

    /// @brief Process a single byte of data from RX stream
    /// @param byte Data
    parsing_error_t processByte(uint8_t byte);
    
    void setMessageHandler(modulink_message_type_t target,
                           handler_t handler);

    void setLogHandler(log_handler_t handler);

    ModuLinkListener();
private:
    typedef enum {
        PARSING_WAITING_MAGIC,
        PARSING_HEADER,
        PARSING_DATA,
    } parsing_status_t;

    uint8_t pMsgBuf[128];
    uint8_t pBytesRead;

    unsigned long pHeaderLastMillis;

    parsing_status_t pParsingStatus;
    struct modulink_message_header pTempHeader;

    void pPrepareToListen();
    bool pIsSupported();
    void pAdvertise();
    void pLog(const char *fmt, ...);

    log_handler_t pLogHandler;
    std::map<modulink_message_type_t, handler_t> pHandlers;
};

} // namespace modulink


