#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <napi.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <mutex>

class AudioEngine {
public:
    AudioEngine() {
        //std::cout << "[DEBUG] Initializing MiniAudio engine..." << std::endl;
        if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
            std::cerr << "[ERROR] Failed to initialize MiniAudio engine!" << std::endl;
            throw std::runtime_error("Failed to initialize MiniAudio engine.");
        }
        //std::cout << "[DEBUG] MiniAudio engine initialized successfully!" << std::endl;
    }

    ~AudioEngine() {
        //std::cout << "[DEBUG] Cleaning up MiniAudio engine..." << std::endl;
        ma_engine_uninit(&engine);
    }

    Napi::Value PlayAudio(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        //std::cout << "[DEBUG] PlayAudio function called..." << std::endl;

        if (info.Length() < 2 || !info[0].IsString() || !info[1].IsFunction()) {
            std::cerr << "[ERROR] Invalid arguments! Expected a file path and a callback function." << std::endl;
            Napi::Error::New(env, "Error: Expected a file path (string) and a callback function.").ThrowAsJavaScriptException();
            return env.Null();
        }

        std::string filePath = info[0].As<Napi::String>().Utf8Value();
        Napi::Function callback = info[1].As<Napi::Function>();

        //std::cout << "[DEBUG] Received file path: " << filePath << std::endl;

        if (!std::ifstream(filePath)) {
            std::cerr << "[ERROR] Audio file not found at path: " << filePath << std::endl;
            Napi::Error::New(env, "Error: Audio file not found at path: " + filePath).ThrowAsJavaScriptException();
            return env.Null();
        }

        ma_sound sound;
        //std::cout << "[DEBUG] Initializing sound..." << std::endl;
        if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS) {
            std::cerr << "[ERROR] Sound initialization failed!" << std::endl;
            Napi::Error::New(env, "Sound initialization failed.").ThrowAsJavaScriptException();
            return env.Null();
        }

        //std::cout << "[DEBUG] Starting sound playback..." << std::endl;
        if (ma_sound_start(&sound) != MA_SUCCESS) {
            std::cerr << "[ERROR] Failed to start sound playback!" << std::endl;
            ma_sound_uninit(&sound);
            Napi::Error::New(env, "Error: Failed to start sound playback!").ThrowAsJavaScriptException();
            return env.Null();
        }

        //std::cout << "[DEBUG] Audio is now playing: " << filePath << std::endl;

        // Sleep for a reasonable duration to allow audio playback before cleanup
        //std::cout << "[DEBUG] Sleeping to allow sound playback..." << std::endl;
        Sleep(500);  // Adjust the sleep duration based on your audio length

        //std::cout << "[DEBUG] Uninitializing sound..." << std::endl;
        ma_sound_uninit(&sound);

        //std::cout << "[DEBUG] Executing callback..." << std::endl;
        callback.Call({Napi::String::New(env, "Audio finished playing!")});

        return Napi::String::New(env, "Playing: " + filePath);
    }

private:
    ma_engine engine;
};

// Singleton engine instance with mutex for safety
std::mutex engineMutex;
AudioEngine* globalAudioEngine = nullptr;

Napi::Value PlayAudioWrapper(const Napi::CallbackInfo& info) {
    std::lock_guard<std::mutex> lock(engineMutex);
    //std::cout << "[DEBUG] PlayAudioWrapper function called..." << std::endl;

    if (!globalAudioEngine) {
        //std::cout << "[DEBUG] Creating new AudioEngine instance..." << std::endl;
        globalAudioEngine = new AudioEngine();
    }

    return globalAudioEngine->PlayAudio(info);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    //std::cout << "[DEBUG] Initializing module exports..." << std::endl;
    exports.Set("playAudio", Napi::Function::New(env, PlayAudioWrapper));
    return exports;
}

NODE_API_MODULE(node_miniaudio, Init)