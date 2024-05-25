#!/bin/bash

ssh moducopter@ModuCopter.local systemctl --user stop modulink

ssh moducopter@ModuCopter.local rm -rf /home/moducopter/ModuLinkPy
scp -r $PWD moducopter@ModuCopter.local:/home/moducopter/ModuLinkPy

ssh moducopter@ModuCopter.local systemctl --user start modulink
#ssh moducopter@ModuCopter.local bash -c '~/venv/bin/python3 ~/ModuLinkPy/daemon.py'
