#pragma once

#include "BusNode.h"
#include "Interactable.h"
#include <optional>


class Interactor : public BusNode {
public:
	std::optional<glm::vec3*> interactor_pos;
	std::vector<Interactable*> interactables;

	Interactor() = delete;
	Interactor(EventBus* eventBus);


	void update();
protected:
	void onNotify(Event event);
private:
	bool isInteractable(glm::vec3 interactee);
};