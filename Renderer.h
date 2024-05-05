#pragma once
#include "BusNode.h"
#include "Object3D.h"
#include "Camera.h"
#include <vector>

class Renderer : public BusNode {
public:
	Renderer() = delete;
	Renderer(EventBus* eventBus);
	void update();
	bool pollEvents();
	void render();
protected:
	void onNotify(Event event);
private:
	std::vector<std::shared_ptr<Object3D>> render_objects;
	std::vector<ShaderProgram> shaders;
	std::shared_ptr<sf::RenderWindow> window;
	sf::ContextSettings Settings;
	std::unique_ptr<Camera> camera;
	glm::mat4 perspective;
	bool mouseLock;

};