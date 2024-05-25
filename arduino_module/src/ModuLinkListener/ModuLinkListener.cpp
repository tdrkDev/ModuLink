#include "ModuLink.hpp"

#ifndef __AVR__
#include <memory.h>
#include <cstdio>
#else
#include <Arduino.h>
#endif

#include <stdarg.h>

namespace modulink {

ModuLinkListener::parsing_error_t ModuLinkListener::processByte(uint8_t byte) {
    switch (pParsingStatus) {
        case PARSING_WAITING_MAGIC:
            pHeaderLastMillis = millis();
            if (pBytesRead > 4) {
                pMsgBuf[0] = pMsgBuf[1];
                pMsgBuf[1] = pMsgBuf[2];
                pMsgBuf[2] = pMsgBuf[3];
                pMsgBuf[3] = byte;
            } else {
                pMsgBuf[pBytesRead] = byte;
            }
            pBytesRead++;
            if (pBytesRead > 3) {
                memcpy(&pTempHeader.start_magic, pMsgBuf, 4);
                if (pTempHeader.start_magic != MODULINK_START_MAGIC) {
                    pLog("[E] Invalid message magic %08lx vs %08lx",
                         pTempHeader.start_magic, MODULINK_START_MAGIC);
                    return PAR_INVALID_MAGIC;
                }

                pParsingStatus = PARSING_HEADER;
                pBytesRead = 0;
            }

            return PAR_SUCCESS;

        case PARSING_HEADER:
            pMsgBuf[pBytesRead + 4] = byte;
            pBytesRead++;

            if (pBytesRead == sizeof(pTempHeader) - sizeof(uint32_t)) {
                memcpy(&pTempHeader, pMsgBuf, sizeof(pTempHeader));

                if (pTempHeader.message_length == 0) {
                    if (!pIsSupported()) {
                        pLog("[E] Message %04x (%u) is unsupported (len %d, chksum %08lx, ver %d, reserved %02x%02x%02x)",
                             pTempHeader.message_type,
                             pTempHeader.message_type,
                             pTempHeader.message_length,
                             pTempHeader.message_checksum,
                             pTempHeader.message_version,
                             pTempHeader.reserved[0],
                             pTempHeader.reserved[1],
                             pTempHeader.reserved[2]);
                        pPrepareToListen();
                        return PAR_UNSUPPORTED_TYPE;
                    }

                    pAdvertise();
                } else {
                    memset(static_cast<void*>(pMsgBuf), 0, sizeof(pMsgBuf));
                    pParsingStatus = PARSING_DATA;
                    pBytesRead = 0;
                }
            }

            return PAR_SUCCESS;
        
        case PARSING_DATA:
            pMsgBuf[pBytesRead] = byte;
            pBytesRead++;

            if (pBytesRead != pTempHeader.message_length)
                return PAR_SUCCESS;

            if (!pIsSupported()) {
                pLog("[E] Message %04x (%d) is unsupported",
                     pTempHeader.message_type,
                     pTempHeader.message_type);
                pPrepareToListen();
                return PAR_UNSUPPORTED_TYPE;
            }

            uint32_t t = pMsgBuf[0];
            for (size_t i = 1; i < pBytesRead; i++)
                t ^= pMsgBuf[i];
            uint32_t checksum_final = MODULINK_CHECKSUM_BASE ^ t;

            if (checksum_final != pTempHeader.message_checksum) {
                pLog("[E] Invalid message checksum %08x actual vs %08x expected",
                     checksum_final, pTempHeader.message_checksum);
                pPrepareToListen();
                return PAR_INVALID_CHECKSUM;
            }

            pAdvertise();
            return PAR_SUCCESS;
    }
}

void ModuLinkListener::pAdvertise() {
    unsigned long newMillis = millis();
    //pLog("Msg took %d ms", newMillis - pHeaderLastMillis);

    const auto type = static_cast<modulink_message_type_t>(pTempHeader.message_type);
    const auto it = pHandlers.find(type);

    if (it == pHandlers.end() || it->second == nullptr || *it->second == nullptr) {
        pPrepareToListen();
        return;
    }

    const auto buf = pTempHeader.message_length == 0 ? nullptr : pMsgBuf;
    (*it->second)(pTempHeader.message_version, pTempHeader.message_length, buf);

exit:
    pPrepareToListen();
}

bool ModuLinkListener::pIsSupported() {
    return pTempHeader.message_type < MODULINK_MSG_MAX_ID &&
           pTempHeader.message_type > MODULINK_MSG_BASE_ID;
}

void ModuLinkListener::pPrepareToListen() {
    pBytesRead = 0;
    pParsingStatus = PARSING_WAITING_MAGIC;
    memset(static_cast<void*>(pMsgBuf), 0, sizeof(pMsgBuf));
    memset(static_cast<void*>(&pTempHeader), 0, sizeof(pTempHeader));
}

void ModuLinkListener::setMessageHandler(modulink_message_type_t target,
                                         handler_t handler) {
    if (handler == nullptr || *handler == nullptr)
        return;

    pLog("[D] Added %04x (%d) message handler", target, target);
    pHandlers.insert({target, handler});
}

void ModuLinkListener::setLogHandler(log_handler_t handler) {
    if (handler == nullptr || *handler == nullptr)
        return;

    pLogHandler = handler;
    pLog("[D] ModuLink log handler has been set\n");
}

void ModuLinkListener::pLog(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    (*pLogHandler)(fmt, &args);
    va_end(args);
}
    
ModuLinkListener::ModuLinkListener()
    : pBytesRead(0),
      pParsingStatus(PARSING_WAITING_MAGIC),
      pMsgBuf(),
      pTempHeader(),
      pLogHandler(nullptr) {
    pPrepareToListen();
}

} // namespace modulink

