#include "Renderer.h"
#include <iostream>

struct DirLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

Renderer::Renderer(EventBus* eventBus) : BusNode(RENDERER,eventBus) {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	window = std::make_shared<sf::RenderWindow>(sf::VideoMode{ 1000, 1000 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	camera = std::make_unique<Camera>(eventBus, window);
	gladLoadGL();

	// Load shaders and bind them for use.

	// shared parameters
	glm::vec4 material = glm::vec4(0.1, 0.0, 0.0, 3); // TODO, change for each model
	glm::vec3 ambientColor = glm::vec3(0.8, 0.8, 1); // ambient light color
	glm::vec3 directionalLight = glm::vec3(0, -1, 0); // direction of light
	glm::vec3 directionalColor = glm::vec3(1, 1, 1); // directional light color
	perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window->getSize().x) / window->getSize().y, 0.1, 100.0);

	/**********************************************************************************
	*	DEFAULT SHADER
	**********************************************************************************/
	ShaderProgram defaultShader;
	shaders.push_back(defaultShader);
	try {
		shaders[0].load("shaders/texture_perspective.vert", "shaders/texturing.frag");
		//shaders[0].load("shaders/texture_perspective.vert", "shaders/all_green.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}

	// activate and uniforms
	/*
	shaders[0].activate();
	shaders[0].setUniform("view", camera->getCamera());
	shaders[0].setUniform("projection", perspective);
	shaders[0].setUniform("viewPos", camera->getLocation());
	shaders[0].setUniform("material", material);
	shaders[0].setUniform("ambientColor", ambientColor);
	shaders[0].setUniform("directionalLight", directionalLight);
	shaders[0].setUniform("directionalColor", directionalColor);
	*/

	/**********************************************************************************
	*	Multiple Light Shader
	**********************************************************************************/

	// point light params
	glm::vec4 point_material = glm::vec4(0.0, 1, 0.0, 16);
	glm::vec3 position1 = glm::vec3(-0.255263, 2.41485, -0.898758);
	lightPos = glm::vec3(-4.7, 0, -0.898758); // point light 2

	glm::vec3 p_color1 = glm::vec3(1,0,0);
	glm::vec3 p_color2 = glm::vec3(0,0,1);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;



	ShaderProgram lightShader;
	shaders.push_back(lightShader);
	try {
		shaders[1].load("shaders/texture_perspective.vert", "shaders/lighting.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	// activate and set uniforms
	
	shaders[1].activate();
	shaders[1].setUniform("view", camera->getCamera());
	shaders[1].setUniform("projection", perspective);
	shaders[1].setUniform("viewPos", camera->getLocation());
	shaders[1].setUniform("material", material);
	shaders[1].setUniform("ambientColor", ambientColor);
	shaders[1].setUniform("directionalLight", directionalLight);
	shaders[1].setUniform("directionalColor", directionalColor);
	shaders[1].setUniform("position2", position1);
	shaders[1].setUniform("position2", lightPos);
	shaders[1].setUniform("p_color1", p_color1);
	shaders[1].setUniform("p_color2", p_color2);
	shaders[1].setUniform("constant", constant);
	shaders[1].setUniform("linear", linear);
	shaders[1].setUniform("quadratic", quadratic);
	shaders[1].setUniform("point_material", point_material);
	

	// OpenGL setup
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	mouseLock = true;
	window->setMouseCursorVisible(false);

	forward = true;

}

void Renderer::update() {
	if (mouseLock) {
		sf::Mouse::setPosition(sf::Vector2i(500, 500), *window);
	}
	/*
	if (lightPos.z > 0.466991) {
		forward = false;
	}
	else if (lightPos.z < -5.77539) {
		forward = true;
	}

	if (forward) {
		lightPos.z += 0.001;
	}
	else {
		lightPos.z -= 0.001;
	}
	*/
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
	//window->clear();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaders[1].setUniform("view", camera->getCamera());
	shaders[1].setUniform("viewPos", camera->getLocation());
	shaders[1].setUniform("position2", lightPos);
	//shaders[0].setUniform("view", camera->getCamera());
	//shaders[0].setUniform("viewPos", camera->getLocation());

	for(std::shared_ptr<Object3D> object : render_objects) {
		//object->render(*window, shaders[0]);
		object->render(*window, shaders[1]);
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
			case sf::Keyboard::Up:
				lightPos.y += 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
				break;
			case sf::Keyboard::Down:
				lightPos.y -= 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
				break;
			case sf::Keyboard::Left:
				lightPos.x += 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
				break;
			case sf::Keyboard::Right:
				lightPos.x -= 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
				break;
			case sf::Keyboard::M:
				lightPos.z += 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
				break;
			case sf::Keyboard::N:
				lightPos.z -= 0.1;
				std::cout << "Light Position: " << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << std::endl;
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