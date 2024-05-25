#include "px4.h"

#define MODULINK_START_MAGIC   0x4badbeef
#define MODULINK_CHECKSUM_BASE 0xaa55f00f

struct modulink_message_header {
  /// @brief = MODULINK_START_MAGIC
  uint32_t start_magic;

  /// @brief Message type, one of @ref modulink_message_type_t
  uint16_t message_type;

  /// @brief Message data version
  uint8_t  message_version;

  /// @brief Length of message data, coming after header
  uint16_t message_length;

  /// @brief XOR message by 32bit chunks (padded with zeroes if needed),
  ///        then XOR MODULINK_CHECKSUM_BASE using calculated value
  ///
  ///        Equals to zero if @ref modulink_message_header.message_length == 0
  uint32_t message_checksum;

  /// @brief For 32-bit padding
  uint8_t reserved[3];
};

struct modulink_message_data_rc_v1 {
  uint8_t channels_count;
  uint8_t rssi;
  uint16_t reserved;
  uint16_t channels[18];
};

struct modulink_message_data_battery_v1 {
  int8_t remaining;
  int32_t current_ma;
  uint16_t voltage_mv;
  uint8_t reserved;
};

struct modulink_message_data_systime_v1 {
  uint64_t epoch_us;
  uint32_t uptime_ms;
};

struct modulink_message_data_system_status_v1 {
  uint8_t last_prearm_status;
  uint8_t status;
  uint8_t main_mode;
  uint8_t sub_mode;
};

struct modulink_message_data_rc_status_v1 {
  uint8_t present;
  uint8_t enabled;
  uint8_t healthy;
  uint8_t reserved;
};

typedef enum : uint16_t {
  MODULINK_MSG_BASE_ID = 0x3000,
  MODULINK_MSG_RC,
  MODULINK_MSG_BATTERY,
  MODULINK_MSG_SYSTIME,
  MODULINK_MSG_SYSTEM_STATUS,
  MODULINK_MSG_RC_STATUS,
  MODULINK_MSG_MAX_ID,
} modulink_message_type_t;
