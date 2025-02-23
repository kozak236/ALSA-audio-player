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
RUN apt-get install neovim git wget curl unzip -y
RUN apt-get clean

# Change user
USER ubuntu

# Setup NVIM as IDE
RUN git clone https://github.com/kozak236/nvim-config.git ~/.config/nvim
