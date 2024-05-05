#include "Bunny.h"
#include "AssimpImport.h"

Bunny::Bunny(EventBus* eventBus) : BusNode(BUNNY, eventBus) {
	model = assimpLoad("models/sibenik/sibenik.obj", true);
	model->move(glm::vec3(0, 0, -3));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
}

void Bunny::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Bunny::update() {}

void Bunny::onNotify(Event event){}