#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

class SoundPlayer {
public:
    SoundPlayer();
    ~SoundPlayer();
    void loadSound(const string& name, const string& filepath);
    void playSound(const string& name, bool loop = false);
    void stopSound(const string& name);

private:
    ALCdevice* device;
    ALCcontext* context;
    map<string, ALuint> buffers;
    map<string, ALuint> sources;
    bool loadWavFile(const std::string& filename, std::vector<char>& buffer, ALenum& format, ALsizei& freq);
};
