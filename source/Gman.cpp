#include "Gman.h"
#include "AssimpImport.h"

Gman::Gman(EventBus* eventBus) : BusNode(GMAN, eventBus) {
	model = assimpLoad("models/gman-half-life-2/gman half life 2.fbx", true);
	model->move(glm::vec3(16.5168, 2.91243, -2.84142));
	model->setScale(glm::vec3(0.001, 0.001, 0.001));
	model->setOrientation(glm::vec3(0, -1.56, 0));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
}

void Gman::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Gman::update() {}

void Gman::onNotify(Event event) {}