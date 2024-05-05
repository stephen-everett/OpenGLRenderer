#pragma once
#include <glm/ext.hpp>
#include "BusNode.h"


class Camera : public BusNode {
public:
	Camera() = delete;
	Camera(EventBus* eventBus, std::shared_ptr<sf::RenderWindow> rwindow);
	glm::mat4 getCamera();
	glm::vec3 getLocation();
	void update();
protected:
	void onNotify(Event event);
private:
	std::shared_ptr<sf::RenderWindow> window;
	glm::mat4 camera;
	glm::vec3 pos;
	glm::vec3 look;
	glm::vec3 up;

	float pitch;
	float yaw;

	bool movingUp;
	bool movingDown;
	bool movingRight;
	bool movingLeft;
	bool movingForward;
	bool movingBackward;

	float speed;

	void updatePosition();
	void updateLookAt();


	void moveForward();
	void moveBackwards();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};