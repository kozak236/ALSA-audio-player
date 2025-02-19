FROM ubuntu:latest
MAINTAINER kozak236 <kozak236@gmail.com>

# Packages installation
RUN apt-get update
RUN apt-get install make build-essential alsa-base alsa-utils libasound2-dev \
    pipewire-alsa pipewire -y
RUN apt-get clean
