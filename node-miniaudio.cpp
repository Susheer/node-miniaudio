#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <napi.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <thread> // For non-blocking execution
#include <direct.h> // Windows-specific header for _getcwd

ma_engine engine;

void CheckSoundCompletion(Napi::Env env, Napi::Function callback, ma_sound* sound) {
    std::thread([env, callback, sound]() {
        while (ma_sound_is_playing(sound)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        callback.Call({Napi::String::New(env, "Audio finished playing!")});
        ma_sound_uninit(sound);
        ma_engine_uninit(&engine);
    }).detach();
}

Napi::Value PlayAudio(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction) {
        return Napi::String::New(env, "Invalid arguments! Provide a file path and a callback function.");
    }

    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[1].As<Napi::Function>(); 

    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Failed to initialize MiniAudio engine!" << std::endl;
        return Napi::String::New(env, "Failed to initialize MiniAudio engine!");
    }

    std::ifstream file(filePath);
    if (!file) {
    std::cout << "Warning: output.wav not found!" << std::endl;
    } else {
        std::cout << "Found output.wav! Attempting to play..." << std::endl;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }

    /*
    *  A more flexible way of playing a sound is to first initialize a sound:
    */

    ma_sound sound;
    ma_result result;

    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    std::cout << "Ma Result " << result << std::endl;
    if (result != MA_SUCCESS) {
        std::cout << "Sound initialization failed: " << result << std::endl;
        return Napi::String::New(env, "Sound initialization failed: ");
    }

    /** 
     * Sounds are not started by default. Start a sound with `ma_sound_start()` 
     * */ 
   
    ma_sound_start(&sound);
    // Wait until the sound finishes playing
    while (ma_sound_is_playing(&sound)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
    
    std::cout <<  "Audio finished playing!" << std::endl;
    // Cleanup
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine); // ✅ Uninitialize the engine
    return Napi::String::New(env, "Playing: " + filePath);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("playAudio", Napi::Function::New(env, PlayAudio));
    return exports;
}

NODE_API_MODULE(node-miniaudio, Init)