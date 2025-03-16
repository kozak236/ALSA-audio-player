#include "APlayer.hpp"
#include "WaveAudio.hpp"
#include <alsa/pcm.h>

int main(int argc, char *argv[]){
    // Open wave file
    WaveAudio audio("../audio/PinkPanther60.wav");

    // Create player instance
    APlayer player("pipewire",
            static_cast<unsigned int>(audio.get_wave_channels()),
            audio.get_wave_bits_per_sample() == 8 ? SND_PCM_FORMAT_S8 : SND_PCM_FORMAT_S16_LE,
            static_cast<unsigned int>(audio.get_wave_sample_rate()));

    // Display WAVE file properties
    audio.display_params();

    // Display hardware parameters
    player.print_hw_params();
}
