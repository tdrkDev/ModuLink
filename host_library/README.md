# ModuLink Python Library

**Only host support is present** *(for now)*

## Dependencies

* `pymavlink`
* `pyserial`

## Configuration

**Setup mavproxy.py to be ran on boot**

See `daemon.py` for basic settings

A user systemd service is preferred for setting up ModuLink
daemon being ran at boot.

## Limitations

Currently, only host-to-module communication is supported.
Support for module-to-host communication is planned.

## Tools

* `update.sh`: Update library files on host (`moducopter@ModuCopter.local`) via ssh.
