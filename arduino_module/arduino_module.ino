#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <LEDMatrixDriver.hpp>

#include "src/ModuLinkListener/ModuLink.hpp"
#include "mav.h"
#include "sprites.hpp"

//
// ModuLink example:
// Host status display on 8x8 LED matrix
//

// CONFIGURATION STARTS HERE //

// ModuLink serial port
#define TX_PIN  A0
#define RX_PIN  A1

// LED matrix
#define LED_SDA 11 // DIN
#define LED_SCL 13
#define LED_CS  5 

#define MATRIX_FPS 10

// CONFIGURATION ENDS HERE //

static SoftwareSerial serial{RX_PIN, TX_PIN};
LEDMatrixDriver matrix{1, LED_CS};

using parsing_error_t = modulink::ModuLinkListener::parsing_error_t;
using handler_status_t = modulink::ModuLinkListener::handler_status_t;

static modulink::ModuLinkListener ml{};

//
// Matrix UI
//

static void anim_loading(uint32_t frame) {
  // SWITCH-CASE DOESN'T WORK HERE
  uint8_t sprite = frame % 8;
  if (sprite == 0) {
    show_loading_0();
  } else if (sprite == 1) {
    show_loading_1();
  } else if (sprite == 2) {
    show_loading_2();
  } else if (sprite == 3) {
    show_loading_3();
  } else if (sprite == 4) {
    show_loading_4();
  } else if (sprite == 5) {
    show_loading_5();
  } else if (sprite == 6) {
    show_loading_6();
  } else if (sprite == 7) {
    show_loading_7();
  }
}

static bool blink_modes = false;

static void anim_service(uint32_t frame) {
  if (frame % 10 >= 4) {
    matrix.clear();
    matrix.display();
  } else {
    show_service();
  }
}

static void anim_acro(uint32_t frame) {
  bool clear = false;
  if (blink_modes) {
    clear = frame % 2 == 1;
  } else {
    clear = frame % 10 >= 5;
  }

  if (clear) {
    matrix.clear();
    matrix.display();
  } else {
    show_acro();
  }
}

static void anim_stabilized(uint32_t frame) {
  if (frame % 10 >= 6 && blink_modes) {
    matrix.clear();
    matrix.display();
  } else {
    show_stabilized();
  }
}

static void anim_manual(uint32_t frame) {
  if (frame % 10 >= 6 && blink_modes) {
    matrix.clear();
    matrix.display();
  } else {
    show_manual();
  }
}

static void anim_error(uint32_t frame) {
  if (frame % 10 >= 5) {
    matrix.clear();
    matrix.display();
  } else {
    show_error();
  }
}

static void anim_ks(uint32_t frame) {
  if (frame % 10 >= 4) {
    matrix.clear();
    matrix.display();
  } else {
    show_dead();
  }
}

static void anim_rc(uint32_t frame) {
  if (frame % 10 >= 4) {
    matrix.clear();
    matrix.display();
  } else {
    show_rc_lost();
  }
}

static void anim_smile(uint32_t frame) {
  show_smile();
}

static void (*current_anim_func)(uint32_t) = anim_loading;
static void (*saved_anim_func)(uint32_t);
static bool rc_lost = false;
static bool block_anims = false;

static void matrix_process_rc_status(struct modulink_message_data_rc_status_v1 *status) {
  rc_lost = !status->present;
  if (rc_lost)
    current_anim_func = anim_rc;
}

static void matrix_process_rc(struct modulink_message_data_rc_v1 *data) {
  if (data->channels[6] > 1500) {
    block_anims = true;
    saved_anim_func = current_anim_func;
    current_anim_func = anim_smile;
  } else {
    if (block_anims)
      current_anim_func = saved_anim_func;
    block_anims = false;
  }
}

static void matrix_process_system_status(struct modulink_message_data_system_status_v1 *status) {
  if (rc_lost || block_anims) return;

  switch (status->status) {
    case MAV_STATE_CALIBRATING:
    case MAV_STATE_BOOT:
    case MAV_STATE_UNINIT:
      current_anim_func = anim_service;
      break;
    case MAV_STATE_STANDBY:
    case MAV_STATE_POWEROFF:
      blink_modes = false;
      switch (status->main_mode) {
        case PX4_CUSTOM_MAIN_MODE_ACRO:
          current_anim_func = anim_acro;
          break;
        case PX4_CUSTOM_MAIN_MODE_STABILIZED:
          current_anim_func = anim_stabilized;
          break;
        case PX4_CUSTOM_MAIN_MODE_MANUAL:
          current_anim_func = anim_manual;
          break;
        default:
          current_anim_func = anim_service;
          break;
      }
      break;
    case MAV_STATE_ACTIVE:
      blink_modes = true;
      switch (status->main_mode) {
        case PX4_CUSTOM_MAIN_MODE_ACRO:
          current_anim_func = anim_acro;
          break;
        case PX4_CUSTOM_MAIN_MODE_STABILIZED:
          current_anim_func = anim_stabilized;
          break;
        case PX4_CUSTOM_MAIN_MODE_MANUAL:
          current_anim_func = anim_manual;
          break;
        default:
          current_anim_func = anim_service;
          break;
      }
      break;
    case MAV_STATE_EMERGENCY:
    case MAV_STATE_CRITICAL:
      current_anim_func = anim_error;
      break;
    case MAV_STATE_FLIGHT_TERMINATION:
      current_anim_func = anim_ks;
      break;
  }

  if (status->last_prearm_status == 0
      && status->status != MAV_STATE_FLIGHT_TERMINATION) {
    current_anim_func = anim_error;
    return;
  }
}

static constexpr int matrix_update_ms = 1000 / MATRIX_FPS;
static long prevMs = 0;
static uint32_t curFrame = 0;
void matrix_process() {
  auto curMs = millis();
  if (curMs >= prevMs + matrix_update_ms) {
    prevMs = curMs;
    if (current_anim_func == nullptr)
      return;
    
    current_anim_func(curFrame);
    curFrame++;
  }
}

//
// Handlers
//

handler_status_t modulink_battery_handler(uint8_t version, uint16_t length, void *raw) {
  if (version != 1) return handler_status_t::MH_ERROR_MESSAGE_UNSUPPORTED;
  const auto data = static_cast<struct modulink_message_data_battery_v1*>(raw);

  const auto ms = millis();
  mprintf("---------------------------------");
  mprintf("[%08ld] Battery voltage: %u mV",
          ms, data->voltage_mv);
  mprintf("[%08ld] Battery current: %ld mA",
          ms, data->current_ma);

  return handler_status_t::MH_SUCCESS;
}

handler_status_t modulink_systime_handler(uint8_t version, uint16_t length, void *raw) {
  if (version != 1) return handler_status_t::MH_ERROR_MESSAGE_UNSUPPORTED;
  const auto data = static_cast<struct modulink_message_data_systime_v1*>(raw);

  uint64_t ms = data->epoch_us / 1000,
           s = ms / 1000,
           m = s / 60,
           h = m / 60;

  const auto mis = millis();
  mprintf("---------------------------------");
  mprintf("[%08ld] Current time: %02d:%02d:%02d",
          mis, (uint16_t)(h % 24), (uint16_t)(m % 60), (uint16_t)(s % 60));

  return handler_status_t::MH_SUCCESS;
}

handler_status_t modulink_sys_status_handler(uint8_t version, uint16_t length, void *raw) {
  if (version != 1) return handler_status_t::MH_ERROR_MESSAGE_UNSUPPORTED;
  const auto data = static_cast<struct modulink_message_data_system_status_v1*>(raw);

  const auto mis = millis();
  mprintf("---------------------------------");
  mprintf("[%08ld] System status: %d (%d:%d)",
          mis, data->status, data->main_mode, data->sub_mode);

  matrix_process_system_status(data);

  return handler_status_t::MH_SUCCESS;
}

handler_status_t modulink_rc_status_handler(uint8_t version, uint16_t length, void *raw) {
  if (version != 1) return handler_status_t::MH_ERROR_MESSAGE_UNSUPPORTED;
  const auto data = static_cast<struct modulink_message_data_rc_status_v1*>(raw);

  // const auto mis = millis();
  // mprintf("---------------------------------");
  // mprintf("[%08ld] RC status: %s -> %s -> %s",
  //         mis, data->present ? "present" : "not present",
  //         data->enabled ? "enabled" : "disabled",
  //         data->healthy ? "healthy" : "dead");

  matrix_process_rc_status(data);

  return handler_status_t::MH_SUCCESS;
}

handler_status_t modulink_rc_handler(uint8_t version, uint16_t length, void *raw) {
  if (version != 1) return handler_status_t::MH_ERROR_MESSAGE_UNSUPPORTED;
  const auto data = static_cast<struct modulink_message_data_rc_v1*>(raw);

  // const auto mis = millis();
  // mprintf("---------------------------------");
  // mprintf("[%08ld] RC ch 7: %d",
  //         mis, data->channels[6]);

  matrix_process_rc(data);

  return handler_status_t::MH_SUCCESS;
}

void setupHandlers() {
  //ml.setMessageHandler(MODULINK_MSG_BATTERY, &modulink_battery_handler);
  //ml.setMessageHandler(MODULINK_MSG_SYSTIME, &modulink_systime_handler);
  ml.setMessageHandler(MODULINK_MSG_SYSTEM_STATUS, &modulink_sys_status_handler);
  ml.setMessageHandler(MODULINK_MSG_RC_STATUS, &modulink_rc_status_handler);
  ml.setMessageHandler(MODULINK_MSG_RC, &modulink_rc_handler);
}

///////////

static int mprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  modulink_logger(fmt, &args);
  va_end(args);
}

extern "C" {
int serialputc(char c, FILE *fp) { 
    Serial.write(c);
    return 0;
}
}

int modulink_logger(const char *fmt, va_list *args) {
  FILE stdserial;
  fdev_setup_stream(&stdserial, serialputc, NULL, _FDEV_SETUP_WRITE);
  vfprintf(&stdserial, fmt, *args);
  //Serial.flush();
  Serial.println();
}

static void setup() {
  // Logging
  Serial.begin(115200);
  // Module serial port
  //serial.begin(57600);
  // Clear status matrix
  matrix.setEnabled(true);
  matrix.setIntensity(1);

  ml.setLogHandler(&modulink_logger);
  setupHandlers();
}

static void loop() {
  while (Serial.available())
    ml.processByte(Serial.read());

  matrix_process();
}

