# ModuLink C++ sample module code

Shows flight mode, system status or RC status on 8x8 LED matrix.

## Library

See `src/ModuLinkListener` for library code.

## Configuration

See beginning of `arduino_module.ino` for configuration.
Hardware serial 0 is being used for ModuLink communication.

## Tools

* `gen_sprites_code.py`: workaround for Arduino Nano RAM
  limitations, generates sprites as functions instead of
  2D arrays.
