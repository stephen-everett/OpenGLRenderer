#include "Renderer.h"
#include <iostream>

Renderer::Renderer(EventBus* eventBus) : BusNode(RENDERER,eventBus) {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode{ 1000, 1000 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	camera = std::make_unique<Camera>(eventBus, window);
	gladLoadGL();
	glEnable(GL_TEXTURE_2D); //  when uncommented, triangle is grey. When commented triangle is white

	// Load shaders and bind them for use.
	ShaderProgram defaultShader;
	shaders.push_back(defaultShader);
	try {
		shaders[0].load("shaders/texture_perspective.vert", "shaders/texturing.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	shaders[0].activate();

	ShaderProgram outlineShader;
	shaders.push_back(outlineShader);
	try {
		shaders[1].load("shaders/light_perspective.vert", "shaders/lighting.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	//shaders[1].activate();

	perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window->getSize().x) / window->getSize().y, 0.1, 100.0);

	shaders[0].setUniform("view", camera->getCamera());
	shaders[0].setUniform("projection", perspective);
	shaders[0].setUniform("viewPos", camera->getLocation());

	//shaders[1].setUniform("view", camera->getCamera());
	//shaders[1].setUniform("projection", perspective);

	glm::vec4 material = glm::vec4(0.001, .001, 0.1, 32); // TODO, change for each model
	glm::vec3 ambientColor = glm::vec3(255, 255, 255); // ambient light color

	glm::vec3 directionalLight = glm::vec3(0, -1, 0); // direction of light
	glm::vec3 directionalColor = glm::vec3(255, 255, 255); // directional light color

	shaders[0].setUniform("material", material);
	shaders[0].setUniform("ambientColor", ambientColor);
	shaders[0].setUniform("directionalLight", directionalLight);
	shaders[0].setUniform("directionalColor", directionalColor);
	shaders[0].setUniform("viewPos", camera->getLocation());

	// OpenGL setup
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	mouseLock = true;

}

void Renderer::update() {
	if (mouseLock) {
		sf::Mouse::setPosition(sf::Vector2i(500, 500), *window);
	}
}

void Renderer::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		sf::Event sf_event = get<sf::Event>(event.event);
		if (sf_event.type == sf::Event::KeyPressed) {
			if (sf_event.key.scancode == sf::Keyboard::L) {
				mouseLock = !mouseLock;
			}
		}
		break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == R_REGISTER) {
			std::shared_ptr<Object3D>object =  std::shared_ptr<Object3D>(static_cast<Object3D*>(event.data));
			render_objects.push_back(object);
		}
		if (ev == UPDATE) {
			update();
		}
		break;
	}
}

void Renderer::render() {
	window->clear();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaders[0].setUniform("view", camera->getCamera());
	shaders[0].setUniform("viewPos", camera->getLocation());

	//shaders[1].setUniform("view", camera->getCamera());
	//shaders[0].setUniform("viewPos", camera->getLocation());
	for(auto object : render_objects) {
		object->render(*window, shaders[0]);
		//object->render(*window, shaders[1]);
	}
	window->display();
}

bool Renderer::pollEvents() {
	sf::Event ev;
	while (window->pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window->close();
			return false;
		}
		else if (ev.type == sf::Event::KeyPressed) {
			switch (ev.key.code) {
			case sf::Keyboard::Escape:
				window->close();
				return false;
				break;
			default:
				//sf::Event key_event = ev;
				std::shared_ptr<struct Event> new_event = std::make_shared<Event>(SFML, ev, GLOBAL);
				eventBus->sendMessage(new_event);
				break;
			}
		}
		else {
			//sf::Event key_event = ev;
			std::shared_ptr<struct Event> new_event = std::make_shared<Event>(SFML, ev, GLOBAL);
			eventBus->sendMessage(new_event);
		}
	}

	return true;
}