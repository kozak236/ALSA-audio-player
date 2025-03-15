#include "WaveAudio.hpp"
#include <cstring>
#include <iostream>
#include <stdexcept>


static constexpr unsigned int wave_header_name_length = 4;
static constexpr unsigned int wave_header_ctrl_length = 8;
static constexpr unsigned int wave_fmt_chunk_data_len = 16;
static constexpr unsigned int wave_pcm_format = 1;


WaveAudio::WaveAudio(std::string _name) : wave_name(_name){
    try {
        // Open Audio file
        wave_handle.open(_name, std::ios::binary);

        // Check if audio file was opened successfully
        if(!wave_handle.is_open())
            throw std::runtime_error("Failed to open audio file");

        // Get file size
        wave_handle.seekg(0, std::ios::end);
        io_size = wave_handle.tellg();
        wave_handle.seekg(0, std::ios::beg);

        // Parse WAVE header
        parse_source_file();

    } catch (const std::exception &e) {
        std::cerr << "WAVE file processing error: " << std::endl;
        throw;
    }
}

WaveAudio::~WaveAudio(void){
    // Close WAVE file
    wave_handle.close();
}
 
int WaveAudio::check_chunk_header(const std::string _chunk_name){
    char read_buff[wave_header_name_length];
    
    wave_handle.read(read_buff, wave_header_name_length);
    if(!wave_handle)
        return -1;

    if(std::strncmp(read_buff, _chunk_name.c_str(), wave_header_name_length)){
        return 1;
    }

    return 0;
}

int WaveAudio::read_bytes(char* _read_buff, const std::streamsize _size){
    wave_handle.read(_read_buff, _size);
    if(!wave_handle)
        return -1;

    return 0;
}

int WaveAudio::find_chunk_header(const std::string _chunk_name){
    uint32_t read_word;

    while(true){
        // Header found
        int res = check_chunk_header("fmt ");
        if(res == 0)
            return 0;

        // End of file reached
        if(res < 0)
            return -1;

        // Get chunk size
        if(read_bytes(reinterpret_cast<char *>(&read_word), sizeof(read_word))){
            return -1;
        }

         // Ingore chunk
         wave_handle.ignore(read_word);
         if(wave_handle.eof())
            return -1;
    }

    return 0;
}

int WaveAudio::parse_source_file(void){
    // Temporary buffer
    uint32_t data_in;

    // Check file type
    if(check_chunk_header("RIFF"))
        throw std::runtime_error("Not a WAVE file!");

    // Get file size
    if(read_bytes(reinterpret_cast<char *>(&wave_file_size), sizeof(wave_file_size)))
        throw std::runtime_error("Read error!");

    // Check file size
    if(wave_file_size + wave_header_ctrl_length != io_size)
        throw std::runtime_error("File length error!");

    // Ensure that audio file is of WAVE type
    if(check_chunk_header("WAVE"))
        throw std::runtime_error("Not a WAVE file!");

    // Find format chunk
    if(find_chunk_header("fmt "))
        throw std::runtime_error("Chunk header \"fmt \" could not be found");

    // Enable file exeptions
    wave_handle.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);

    // Ensure that chunk has correct size
    wave_handle.read(reinterpret_cast<char *>(&data_in), sizeof(data_in));
    if(data_in != wave_fmt_chunk_data_len)
        throw std::runtime_error("Wrong fmt len!");

    // Ensure that format is PCM
    wave_handle.read(reinterpret_cast<char *>(&data_in), sizeof(uint16_t));
    if(static_cast<uint16_t>(data_in) != wave_pcm_format)
        throw std::runtime_error("Wrong file format!");

    // Read number of channels
    wave_handle.read(reinterpret_cast<char *>(&wave_channels), sizeof(wave_channels));

    // Read sample rate
    wave_handle.read(reinterpret_cast<char *>(&wave_sample_rate), sizeof(wave_sample_rate));

    // Ignore bytes per second and block align
    wave_handle.ignore(sizeof(uint32_t) + sizeof(uint16_t));

    // Get number of bits per sample
    wave_handle.read(reinterpret_cast<char *>(&wave_bits_per_sample), sizeof(wave_bits_per_sample));

    // Disable file exeptions
    wave_handle.exceptions(std::ifstream::goodbit);

    // Wave header parsed without errors
    return 0;
}


void WaveAudio::display_params(void){
    std::cout << "---------------------------" << std::endl;
    std::cout << "|           WAVE          |" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "WAVE audio file properties:" << std::endl;
    std::cout << "File name: " << wave_name << std::endl;
    std::cout << "Size: " << io_size << std::endl;
    std::cout << "Channels: " << wave_channels << std::endl;
    std::cout << "Sample rate: " << wave_sample_rate << std::endl;
    std::cout << "Bits per sample: " << wave_bits_per_sample << std::endl;
    std::cout << std::endl;
}
