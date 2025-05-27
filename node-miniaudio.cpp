#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <napi.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <thread> // For non-blocking execution
#include <direct.h> // Windows-specific header for _getcwd

ma_engine engine;

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

    /**
     * Ensure the audio file present at the location provided
     */
    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    std::ifstream file(filePath);
    if (!file) {
      Napi::Error::New(env, "No audio file present at the location!").ThrowAsJavaScriptException();
      return env.Null();
    }

    // Check if second argument is a function (callback)
    if (!info[1].IsFunction()) {
        Napi::Error::New(env, "Error: Second argument must be a function (callback to handle completion).").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Function callback = info[1].As<Napi::Function>();
    // Initialize engine 
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        Napi::Error::New(env, "Failed to initialize MiniAudio engine!").ThrowAsJavaScriptException();
        return env.Null();
    }
    

    /*
    *  A more flexible way of playing a sound is to first initialize a sound:
    */

    ma_sound sound;
    ma_result result;
    // Initialize sound
    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        Napi::Error::New(env, "Sound initialization failed").ThrowAsJavaScriptException();
        return env.Null();
    }

    /*** 
     * Sounds are not started by default. Start a sound with `ma_sound_start()` 
     * */ 

    // Start sound playback
    std::cout <<  "Start sound playback" << result << std::endl;
     // Start sound playback
    ma_sound_start(&sound);

    // Start a separate thread to monitor playback and trigger callback
    // Handle playback completion asynchronously
    while (ma_sound_is_playing(&sound)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
        ma_sound_uninit(&sound);
        ma_engine_uninit(&engine);
    callback.Call({Napi::String::New(env, "Audio finished playing!")});

    return Napi::String::New(env, "Playing: " + filePath);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("playAudio", Napi::Function::New(env, PlayAudio));
    return exports;
}

NODE_API_MODULE(node-miniaudio, Init)