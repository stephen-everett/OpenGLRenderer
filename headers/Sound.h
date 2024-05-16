#pragma once
#include "BusNode.h"
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>

class Sound : public BusNode {
public:
	Sound() = delete;
	Sound( EventBus* eventBus);
	void update();

protected:
	void onNotify(Event event);
	bool loadWavFile(const std::string& filename, std::vector<char>& buffer, ALsizei& size, ALsizei& frequency, ALenum& format);
	void play();

private:
	ALCdevice* device;
	ALCcontext* context;
	ALuint source;
	ALuint buffer;
	std::vector<char> data;
	ALsizei size, freq;
	ALenum format;
};