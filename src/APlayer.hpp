#pragma once

#include <string>
#include <alsa/asoundlib.h>

class APlayer {
    public:
        APlayer(void) = delete;
        APlayer(std::string _name);
        ~APlayer(void);
    private:
        // Hardware parameters
        const std::string dev_name;
        snd_pcm_t *dev_handle;
        unsigned int dev_rate;
        unsigned int dev_buffer_time; 
        unsigned int dev_period_time; 
        snd_pcm_uframes_t dev_buffer_size;
        snd_pcm_uframes_t dev_period_size;

        // Hardware parameters - default values
        static constexpr unsigned int DEV_HW_RESAMPLE = 1;
        static constexpr snd_pcm_access_t DEV_HW_ACCESS_TYPE = SND_PCM_ACCESS_RW_INTERLEAVED; 
        static constexpr snd_pcm_format_t DEV_HW_FORMAT = SND_PCM_FORMAT_S16;
        static constexpr unsigned int DEV_HW_CHANNELS = 1;
        static constexpr unsigned int DEV_HW_RATE = 44100;
        static constexpr unsigned int DEV_HW_BUFFER_TIME = 500000;
        static constexpr unsigned int DEV_HW_PERIOD_TIME = 100000;
        static constexpr snd_pcm_uframes_t DEV_HW_BUFFER_SIZE = 8192; // todo: sframes?
        static constexpr snd_pcm_uframes_t DEV_HW_PERIOD_SIZE = 1024; // todo: sframes?
        
        // Device initialization
        int init_hw_params(void) noexcept(false);
        int init_sw_params(void) noexcept(false);
};
