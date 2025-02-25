# Common settings for all subsequent containers
FROM ubuntu:latest AS alsa-base

# Packages installation
RUN apt-get update
RUN apt-get install make build-essential alsa-base alsa-utils libasound2-dev \
    pipewire-alsa pipewire -y
RUN apt-get clean


# Deploy container
FROM alsa-base AS deploy

# Change user
USER ubuntu


# Devel container
FROM alsa-base AS devel

# Additional devel packages
RUN apt-get update
RUN apt-get install git wget curl unzip cmake gettext -y
RUN apt-get clean

# Install nvim (apt-get package too old for some plugins)
RUN git clone https://github.com/neovim/neovim.git --depth 1 /tmp/neovim
RUN cd /tmp/neovim && make CMAKE_BUILD_TYPE=RelWithDebInfo && make install

# Change user
USER ubuntu

# Setup NVIM as IDE
RUN git clone https://github.com/kozak236/nvim-config.git ~/.config/nvim
