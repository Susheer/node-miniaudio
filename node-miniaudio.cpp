#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <napi.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <thread>

ma_engine engine;

Napi::Value PlayAudio(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Validate parameters
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
        Napi::Error::New(env, "Error: Expected a file path (string) and a callback function.").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string filePath = info[0].As<Napi::String>().Utf8Value();
    Napi::Function callback = info[1].As<Napi::Function>();

    // Ensure the audio file exists
    if (!std::ifstream(filePath)) {
        Napi::Error::New(env, "Error: Audio file not found at path: " + filePath).ThrowAsJavaScriptException();
        return env.Null();
    }

    // Initialize MiniAudio engine
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        Napi::Error::New(env, "Failed to initialize MiniAudio engine!").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Initialize sound
    ma_sound sound;
    if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS) {
        Napi::Error::New(env, "Sound initialization failed.").ThrowAsJavaScriptException();
        ma_engine_uninit(&engine); // Ensure engine is cleaned up on failure
        return env.Null();
    }

    // Start sound playback
    if (ma_sound_start(&sound) != MA_SUCCESS) {
        Napi::Error::New(env, "Error: Failed to start sound playback!").ThrowAsJavaScriptException();
        ma_sound_uninit(&sound); // Cleanup sound if playback fails
        ma_engine_uninit(&engine);
        return env.Null();
    }

    // Create ThreadSafeFunction for callback execution within Node.js event loop
    Napi::ThreadSafeFunction tsfn = Napi::ThreadSafeFunction::New(
        env, callback, "Audio Playback Callback", 0, 1, [](Napi::Env) {});

    // Handle playback completion asynchronously
    std::thread([tsfn, &sound]() {
        while (ma_sound_is_playing(&sound)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }

        tsfn.BlockingCall([](Napi::Env env, Napi::Function callback) {
            callback.Call({Napi::String::New(env, "Audio finished playing!")});
        });

        // Cleanup resources
        ma_sound_uninit(&sound);
        ma_engine_uninit(&engine);

        // Release the ThreadSafeFunction
        tsfn.Release();
    }).detach();

    return Napi::String::New(env, "Playing: " + filePath);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("playAudio", Napi::Function::New(env, PlayAudio));
    return exports;
}

NODE_API_MODULE(node-miniaudio, Init)