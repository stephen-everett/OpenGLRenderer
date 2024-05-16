#pragma once
#include <variant>
#include <SFML/Graphics.hpp>

enum EventType {
	SFML,
	USER
};

enum EventEnums {
	TEST_EVENT,
	UPDATE,
	R_REGISTER,
	I_REGISTER,
	CI_REGISTER,
	PLAY_SOUND
};


struct Event {
	EventType event_type;
	std::variant<sf::Event, EventEnums> event;
	int receiver_id;
	void* data;
	Event(EventType type, std::variant<sf::Event, EventEnums> passed_event, int receiver) {
		event_type = type;
		event = passed_event;
		receiver_id = receiver;
		data = nullptr;
	}
	Event(EventType type, std::variant<sf::Event, EventEnums> passed_event, int receiver, void* some_data) {
		event_type = type;
		event = passed_event;
		receiver_id = receiver;
		data = some_data;
	}
};

