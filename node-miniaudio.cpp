#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <napi.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <direct.h> // Windows-specific header for _getcwd

ma_engine engine;
Napi::Value PlayAudio(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::cout << "-------------PlayAudio invoked -----------" << std::endl;
    if (info.Length() < 1 || !info[0].IsString()) {
        return Napi::String::New(env, "Invalid argument! Please provide a file path.");
    }

    std::string filePath = info[0].As<Napi::String>().Utf8Value();

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
    ma_result result; /* = ma_engine_play_sound(&engine, filePath.c_str(), NULL);*/

    result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound);
    std::cout << "Ma Result " << result << std::endl;
    if (result != MA_SUCCESS) {
        std::cout << "Sound initialization failed: " << result << std::endl;
        return Napi::String::New(env, "Sound initialization failed: ");
    }

    std::cout << "-----Uninit engine------" << std::endl;
    Sleep(100); // Sleep for 3000 milliseconds (3 secon
    ma_engine_uninit(&engine);

    return Napi::String::New(env, "Playing: " + filePath);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("playAudio", Napi::Function::New(env, PlayAudio));
    return exports;
}

NODE_API_MODULE(node-miniaudio, Init)