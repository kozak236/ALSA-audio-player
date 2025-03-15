#include "APlayer.hpp"
#include "WaveAudio.hpp"

int main(int argc, char *argv[]){
    // Create player instance
    APlayer player("pipewire");

    // Open wave file
    WaveAudio audio("../audio/PinkPanther60.wav");

    // Parse audio file
    audio.wave_parse_source_file();

    // Display WAVE file properties
    audio.display_params();

    // Display hardware parameters
    player.print_hw_params();
}
