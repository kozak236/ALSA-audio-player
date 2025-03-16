#pragma once

#include <cstdint>
#include <fstream>
#include <string>


class WaveAudio {
    public:
        // Constructors, destructors and others
        WaveAudio(std::string _name);
        ~WaveAudio(void);

        // Wave file parser
        void display_params(void);

        // Getters
        uint16_t get_wave_channels(void) const { return wave_channels; }
        uint32_t get_wave_sample_rate(void) const { return wave_sample_rate; }
        uint16_t get_wave_bits_per_sample(void) const { return wave_bits_per_sample; }
    private:
        // WAVE file handle
        const std::string wave_name;
        std::ifstream wave_handle;

        // File parsing
        int parse_source_file(void);
        int check_chunk_header(const std::string _chunk_name);
        int find_chunk_header(const std::string _chunk_name);
        int read_bytes(char* _read_buff, const std::streamsize _size);
 
        // WAVE file parameters
        uint32_t io_size;
        uint32_t wave_file_size;
        uint16_t wave_channels;
        uint32_t wave_sample_rate;
        uint16_t wave_bits_per_sample;
};
