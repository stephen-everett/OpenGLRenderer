#include "Sound.h"
#include <iostream>
#include <fstream>


Sound::Sound(EventBus* eventBus) : BusNode(SOUND, eventBus) {
    device = alcOpenDevice(NULL);
    if (!device) {
        std::cerr << "Unable to open default device\n";
    }

    context = alcCreateContext(device, NULL);
    if (!context) {
        std::cerr << "Failed to create context\n";
    }

    alcMakeContextCurrent(context);

   
    alGenSources(1, &source);
    alGenBuffers(1, &buffer);

    if (!loadWavFile("boing.wav", data, size, freq, format)) {
        std::cerr << "Failed to load WAV file\n";
    }

    alBufferData(buffer, format, &data[0], size, freq);
    alSourcei(source, AL_BUFFER, buffer);
}

bool Sound::loadWavFile(const std::string& filename, std::vector<char>& buffer, ALsizei& size, ALsizei& frequency, ALenum& format) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.good()) {
        return false;
    }

    char type[4];
    file.read(type, 4); // Should be "RIFF"
    if (std::strncmp(type, "RIFF", 4) != 0) {
        return false;
    }

    file.seekg(4, std::ios::cur); // Skip chunk size

    file.read(type, 4); // Should be "WAVE"
    if (std::strncmp(type, "WAVE", 4) != 0) {
        return false;
    }

    file.read(type, 4); // Should be "fmt "
    file.seekg(4, std::ios::cur); // Skip chunk size

    ALushort audioFormat;
    file.read(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat)); // Audio format
    ALushort numChannels;
    file.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels)); // Number of channels
    file.read(reinterpret_cast<char*>(&frequency), sizeof(frequency)); // Sample rate

    file.seekg(6, std::ios::cur); // Skip byte rate and block align

    ALushort bitsPerSample;
    file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample)); // Bits per sample

    file.read(type, 4); // Should be "data"
    file.read(reinterpret_cast<char*>(&size), sizeof(size)); // Data chunk size

    buffer.resize(size);
    file.read(&buffer[0], size);

    if (numChannels == 1) {
        format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    else {
        format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    return true;
}

void Sound::update() {
}

void Sound::play() {
    alSourcePlay(source);
    std::cout << "Playing...\n";
    ALint source_state;
    /*do {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    } while (source_state == AL_PLAYING);*/

    std::cout << "Finished playing\n";
}

void Sound::onNotify(Event event) {
	switch (event.event_type) {
    case SFML:
        sf::Event ev2 = get<sf::Event>(event.event);
        if (ev2.type == sf::Event::KeyPressed) {
            if (ev2.key.scancode == sf::Keyboard::Scan::P) {
				play();
            }
	    }
        break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == PLAY_SOUND) {
			play();
		}
		break;
	}
}