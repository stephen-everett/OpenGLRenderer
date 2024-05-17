#include "Interactor.h"
#include <glm/glm.hpp>

Interactor::Interactor(EventBus* eventBus) : BusNode(INTERACTOR, eventBus) {}

void Interactor::update() {
	for (Interactable* item: interactables) {
		if (isInteractable(*item->position)) {
			item->interact_toggle(true);
		}
		else {
			item->interact_toggle(false);
		}
	}
}	

bool Interactor::isInteractable(glm::vec3 interactee) {
	glm::vec3 vectorAB = interactee - *interactor_pos.value();
	float distance = glm::length(vectorAB);
	return distance < 3;
}

void Interactor::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == I_REGISTER) {
			Interactable* object = static_cast<Interactable*>(event.data);
			interactables.push_back(object);
		}
		else if (ev == CI_REGISTER) {
			interactor_pos = static_cast<glm::vec3*>(event.data);
		}
		else if (ev == UPDATE) {
			update();
		}
		break;
	}
}