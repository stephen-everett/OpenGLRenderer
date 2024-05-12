#include "Gman.h"
#include "AssimpImport.h"

Gman::Gman(EventBus* eventBus) : BusNode(GMAN, eventBus) {
	model = assimpLoad("models/gman/gman_torso.obj", true);
	model->move(glm::vec3(0, 0, -3));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
}

void Gman::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Gman::update() {}

void Gman::onNotify(Event event) {}