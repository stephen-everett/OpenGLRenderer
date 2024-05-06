#include "AnimBunny.h"
#include "AssimpImport.h"

AnimBunny::AnimBunny(EventBus* eventBus) : BusNode(ANIMBUNNY, eventBus) {
	model = assimpLoad("models/bunny_textured.obj", true);
	std::shared_ptr<Object3D> obj = model;
	model->move(glm::vec3(0, 0, -3));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
	animator = std::make_shared<Animator>();
	time = c.getElapsedTime();
	
	animator->addAnimation(
		[&obj]() {
			return std::make_shared<TranslationAnimation>(obj, 1, glm::vec3(4, -3, -15));
		}
	);
	// then moves exactly to(-1, -1, -1)
	animator->addAnimation(
		[&obj]() {
			return std::make_shared<TranslationAnimation>(obj, 1, glm::vec3(-1, -1, -1) * obj->getPosition());
		}
	);

	animator->start();
}

void AnimBunny::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void AnimBunny::update() {
	auto now = c.getElapsedTime();
	auto diff = now - time;
	animator->tick(diff.asSeconds());
	time = now;
}

void AnimBunny::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		sf::Event sf_event = get<sf::Event>(event.event);
		break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == UPDATE) {
			update();
		}
		break;
	}
}
