#!/bin/bash

docker run -it -v /run/user/1000/pipewire-0:/tmp/pipewire-0 -e XDG_RUNTIME_DIR=/tmp alsa-audio-player
