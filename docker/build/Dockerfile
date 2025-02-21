FROM ubuntu:latest AS deploy
MAINTAINER kozak236 <kozak236@gmail.com>

# Packages installation
RUN apt-get update
RUN apt-get install make build-essential alsa-base alsa-utils libasound2-dev \
    pipewire-alsa pipewire -y
RUN apt-get clean

FROM deploy AS devel

# Additional devel packages
RUN apt-get update
RUN apt-get install neovim git wget curl unzip -y
RUN apt-get clean

# Setup NVIM as IDE
COPY ./nvim-config /root/.config/nvim
RUN git clone https://github.com/kozak236/nvim-config.git ~/.config/nvim
