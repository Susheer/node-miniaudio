#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <fstream>
#include <iostream>

int main() {
    ma_engine engine;
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Failed to initialize MiniAudio engine!" << std::endl;
        return -1;
    }

    std::ifstream file("output.wav");
    if (!file) {
    std::cout << "Warning: output.wav not found!" << std::endl;
    } else {
        std::cout << "Found output.wav! Attempting to play..." << std::endl;
    }

    ma_result result = ma_engine_play_sound(&engine, "output.wav", NULL);
    if (result != MA_SUCCESS) {
        std::cout << "Error playing sound: " << result << std::endl;
    } else {
        std::cout << "Audio playing successfully!" << std::endl;
    }

    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    ma_engine_uninit(&engine);
    return 0;
}