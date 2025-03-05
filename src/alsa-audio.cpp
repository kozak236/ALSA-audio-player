#include "APlayer.hpp"

int main(int argc, char *argv[]){
    // Create player instance
    APlayer player("pipewire");

    // Display hardware parameters
    player.print_hw_params();
}
