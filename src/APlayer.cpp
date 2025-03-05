#include "APlayer.hpp"
#include <iostream>


APlayer::APlayer(std::string _name) : dev_name(_name), dev_rate(DEV_HW_RATE), dev_buffer_time(DEV_HW_BUFFER_TIME), 
            dev_period_time(DEV_HW_PERIOD_TIME), dev_buffer_size(DEV_HW_BUFFER_SIZE), dev_period_size(DEV_HW_PERIOD_SIZE) {

    // Local variables
    int err;

    // Open and configure device
    try {
        // Open audio device
        if((err = snd_pcm_open(&dev_handle, _name.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0)
            throw std::runtime_error(std::string("Open playback device: ") + snd_strerror(err));

        // Set hardware parameters
        init_hw_params();

        // Set software parameters
        init_sw_params();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
    
}

APlayer::~APlayer(){
    snd_pcm_close(dev_handle);
}

int APlayer::init_hw_params(void) noexcept(false){
    // Local variables
    snd_pcm_hw_params_t *hw_params;
    int err, dir;

    // Allocate configuration structure on the heap
    if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
        throw std::runtime_error(std::string("Allocate hardware parameters structure: ") + snd_strerror(err));

    // Start initialization with full configuration space
    if((err = snd_pcm_hw_params_any(dev_handle, hw_params)) < 0)
        throw std::runtime_error(std::string("Init HW params any: ") + snd_strerror(err));

    // Restrict configuration space to real hardware rates
    if((err = snd_pcm_hw_params_set_rate_resample(dev_handle, hw_params, DEV_HW_RESAMPLE)) < 0)
        throw std::runtime_error(std::string("Init HW params resample: ") + snd_strerror(err));

    // Set access type
    if((err = snd_pcm_hw_params_set_access(dev_handle, hw_params, DEV_HW_ACCESS_TYPE)) < 0)
        throw std::runtime_error(std::string("Set access type: ") + snd_strerror(err));

    // Set sample format
    if((err = snd_pcm_hw_params_set_format(dev_handle, hw_params, DEV_HW_FORMAT)) < 0)
        throw std::runtime_error(std::string("Set format: ") + snd_strerror(err));

    // Set number of channels
    if((err = snd_pcm_hw_params_set_channels(dev_handle, hw_params, DEV_HW_CHANNELS)) < 0)
        throw std::runtime_error(std::string("Set channels: ") + snd_strerror(err));

    // Set rate
    if((err = snd_pcm_hw_params_set_rate_near(dev_handle, hw_params, &dev_rate, 0)) < 0)
        throw std::runtime_error(std::string("Set rate: ") + snd_strerror(err));

    // Set buffer time
    if((err = snd_pcm_hw_params_set_buffer_time_near(dev_handle, hw_params, &dev_buffer_time, &dir)) < 0)
        throw std::runtime_error(std::string("Set playback time: ") + snd_strerror(err));

    // Get buffer size
    if((err = snd_pcm_hw_params_get_buffer_size(hw_params, &dev_buffer_size)) < 0)
        throw std::runtime_error(std::string("Get buffer size: ") + snd_strerror(err));

    // Set period time
    if((err = snd_pcm_hw_params_set_period_time_near(dev_handle, hw_params, &dev_period_time, &dir)) < 0)
        throw std::runtime_error(std::string("Set period time: ") + snd_strerror(err));
 
    // Get period size
    if((err = snd_pcm_hw_params_get_period_size(hw_params, &dev_period_size, &dir)) < 0)
        throw std::runtime_error(std::string("Get period size: ") + snd_strerror(err));

    // Write selected options to the device
    if((err = snd_pcm_hw_params(dev_handle, hw_params)) < 0)
        throw std::runtime_error(std::string("Set hardware parameters: ") + snd_strerror(err));

    // Free hardware parameters structure
    snd_pcm_hw_params_free(hw_params);

    return 0;
}

int APlayer::init_sw_params(void) noexcept(false){
    // Local variables
    snd_pcm_sw_params_t *sw_params;
    int err;

    // Allocate configuration structure on the heap
    if((err = snd_pcm_sw_params_malloc(&sw_params)) < 0)
        throw std::runtime_error(std::string("Allocate hardware parameters structure: ") + snd_strerror(err));
 
    // Get intial software parameters
    if((err = snd_pcm_sw_params_current(dev_handle, sw_params)) < 0)
        throw std::runtime_error(std::string("Get initial software parameters: ") + snd_strerror(err));

    // Start the transfer when the buffer is almost full
    snd_pcm_uframes_t threshold = (dev_buffer_size / dev_period_size) * dev_period_size;
    if((err = snd_pcm_sw_params_set_start_threshold(dev_handle, sw_params, threshold)) < 0)
        throw std::runtime_error(std::string("Set start threshold value: ") + snd_strerror(err));

    // Allow the transfer when at least period_size samples can be processed */
    if((err = snd_pcm_sw_params_set_avail_min(dev_handle, sw_params, dev_buffer_size)) < 0)
        throw std::runtime_error(std::string("Set avail min: ") + snd_strerror(err));
    
    // Write selected options to the device
    if((err = snd_pcm_sw_params(dev_handle, sw_params)) < 0)
        throw std::runtime_error(std::string("Set sw params: ") + snd_strerror(err));

    // Free hardware parameters structure
    snd_pcm_sw_params_free(sw_params);

    return 0;
}

void APlayer::print_hw_params(void){
    std::cout << "Opened PCM: " << dev_name << std::endl;
    std::cout << "Sample rate: " << dev_rate << std::endl;
    std::cout << "Number of channels: " << DEV_HW_CHANNELS << std::endl;
    std::cout << "Access type:  SND_PCM_ACCESS_RW_INTERLEAVED" << std::endl;
    std::cout << "Sample format: SND_PCM_FORMAT_S16" << std::endl;
    std::cout << "Buffer time " << dev_buffer_time << std::endl;
    std::cout << "Buffer size: " << dev_buffer_size << std::endl;
    std::cout << "Period time: " << dev_period_time << std::endl;
    std::cout << "Period size: " << dev_period_size << std::endl;
}
