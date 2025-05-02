#include "../../include/core/SoundPlayer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

SoundPlayer::SoundPlayer() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        cerr << "Failed to open audio device\n";
        return;
    }
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
}

SoundPlayer::~SoundPlayer() {
    for (auto& pair : sources) {
        alDeleteSources(1, &pair.second);
    }
    for (auto& pair : buffers) {
        alDeleteBuffers(1, &pair.second);
    }
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void SoundPlayer::loadSound(const std::string& name, const std::string& filepath) {
    ALuint buffer, source;
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    std::vector<char> data;
    ALenum format;
    ALsizei freq;
    if (!loadWavFile(filepath, data, format, freq)) {
        std::cerr << "Failed to load WAV: " << filepath << "\n";
        return;
    }

    alBufferData(buffer, format, data.data(), data.size(), freq);
    alSourcei(source, AL_BUFFER, buffer);

    buffers[name] = buffer;
    sources[name] = source;
}


bool SoundPlayer::loadWavFile(const std::string& filename, std::vector<char>& buffer, ALenum& format, ALsizei& freq) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    char riff[4]; file.read(riff, 4); // "RIFF"
    file.ignore(4); // file size
    char wave[4]; file.read(wave, 4); // "WAVE"
    char fmt[4];  file.read(fmt, 4); // "fmt "
    int fmtLength; file.read(reinterpret_cast<char*>(&fmtLength), 4);

    short audioFormat, channels;
    int sampleRate, byteRate;
    short blockAlign, bitsPerSample;

    file.read(reinterpret_cast<char*>(&audioFormat), 2);
    file.read(reinterpret_cast<char*>(&channels), 2);
    file.read(reinterpret_cast<char*>(&sampleRate), 4);
    file.read(reinterpret_cast<char*>(&byteRate), 4);
    file.read(reinterpret_cast<char*>(&blockAlign), 2);
    file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

    file.ignore(fmtLength - 16); // Skip extra bytes if any

    // Skip to "data"
    char data[4];
    int dataSize = 0;
    while (file.read(data, 4)) {
        file.read(reinterpret_cast<char*>(&dataSize), 4);
        if (std::string(data, 4) == "data") break;
        file.ignore(dataSize);
    }

    if (dataSize == 0) return false;

    buffer.resize(dataSize);
    file.read(buffer.data(), dataSize);

    freq = sampleRate;
    if (channels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    } else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    return true;
}


void SoundPlayer::playSound(const std::string& name, bool loop) {
    auto it = sources.find(name);
    if (it != sources.end()) {
        ALuint source = it->second;

        // Check current playback state
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        // Stop and rewind if already playing
        if (state == AL_PLAYING) {
            alSourceStop(source);
            alSourceRewind(source);
        }

        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(source);
    }
}


void SoundPlayer::stopSound(const std::string& name) {
    auto it = sources.find(name);
    if (it != sources.end()) {
        alSourceStop(it->second);
    }
}
