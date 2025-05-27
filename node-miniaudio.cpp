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

    // Check argument count
    if (info.Length() < 2) {
        Napi::Error::New(env, "Error: Expected 2 arguments (file path, callback function).").ThrowAsJavaScriptException();
        return env.Null(); // Ensure function returns a valid value
    }

    // Check if first argument is a string (file path)
    if (!info[0].IsString()) {
        Napi::Error::New(env, "Error: First argument must be a string (file path to audio).").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[1].As<Napi::Function>(); 

    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Failed to initialize MiniAudio engine!" << std::endl;
        return Napi::String::New(env, "Failed to initialize MiniAudio engine!");
    }
    
    /**
     * Ensure the audio file present at the location provided
     */
    std::ifstream file(filePath);
    if (!file) {
    std::cout << "Warning: output.wav not found!" << std::endl;
    } else {
        std::cout << "Found output.wav! Attempting to play..." << std::endl;
    }

    /*
    *  A more flexible way of playing a sound is to first initialize a sound:
    */

    ma_sound sound;
    ma_result result;

    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        std::cout << "Sound initialization failed: " << result << std::endl;
        return Napi::String::New(env, "Sound initialization failed: ");
    }

    /** 
     * Sounds are not started by default. Start a sound with `ma_sound_start()` 
     * */ 
    ma_sound_start(&sound);
    // Start a separate thread to monitor playback and trigger callback
    CheckSoundCompletion(env, callback, &sound);
    return Napi::String::New(env, "Playing: " + filePath);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("playAudio", Napi::Function::New(env, PlayAudio));
    return exports;
}

NODE_API_MODULE(node-miniaudio, Init)