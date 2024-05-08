#include "Camera.h"
#include <iostream>
#include "Events.h"

Camera::Camera(EventBus* eventBus, std::shared_ptr<sf::RenderWindow> rwindow) : BusNode(CAMERA,eventBus) {
	window = rwindow;

	pitch = 0.0f;
	yaw = -90.0f;

	pos = glm::vec3(5, 5, 5);
	float look_x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	float look_y = sin(glm::radians(pitch));
	float look_z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


	look = glm::vec3(look_x, look_y, look_z);
	up = glm::vec3(0, 1, 0);
	camera = glm::lookAt(pos, look, up);

	speed = 0.001f;
	movingUp = false;
	movingDown = false;
	movingLeft = false;
	movingRight = false;
	movingForward = false;
	movingBackward = false;

	locked = false;

	shared_pos = std::make_shared<glm::vec3>(pos);
	std::shared_ptr<struct Event> register_camera = std::make_shared<Event>(USER, CI_REGISTER, INTERACTOR, static_cast<void*>(&pos));
	eventBus->sendMessage(register_camera);



}

glm::mat4 Camera:: getCamera() {
	return glm::lookAt(pos, pos+look, up);
}

glm::vec3 Camera::getLocation() {
	return pos;
}

void Camera::update() {
	if (!locked) {
		updateLookAt();
		updatePosition();
	}
}

void Camera::updateLookAt() {
	float xoffset = sf::Mouse::getPosition(*window).x - 500;
	float yoffset = 500 - sf::Mouse::getPosition(*window).y;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	float look_x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	float look_y = sin(glm::radians(pitch));
	float look_z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	look = glm::normalize(glm::vec3(look_x, look_y, look_z));
}

void Camera::updatePosition() {
	if (movingForward) {
		moveForward();
	}
	if (movingBackward) {
		moveBackwards();
	}
	if (movingLeft) {
		moveLeft();
	}
	if (movingRight) {
		moveRight();
	}
	if (movingUp) {
		moveUp();
	}
	if (movingDown) {
		moveDown();
	}
}

void Camera::moveForward() {
	pos += speed * look;
	//std::cout << "Moving camera forward..." << std::endl;
}

void Camera::moveBackwards() {
	pos -= speed * look;
	//std::cout << "Moving camera backwards..." << std::endl;
}

void Camera::moveLeft() {
	pos -= glm::normalize(glm::cross(look, up)) * speed;
	//std::cout << "Moving camera left..." << std::endl;
}

void Camera::moveRight() {
	pos += glm::normalize(glm::cross(look, up)) * speed;
	//std::cout << "Moving camera right..." << std::endl;
}

void Camera::moveUp() {
	pos.y += speed;
}

void Camera::moveDown() {
	pos.y -= speed;
}

void Camera::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		//std::cout << "Camera received SFML event" << std::endl;
		sf::Event ev = get<sf::Event>(event.event);
		if (ev.type == sf::Event::KeyPressed) {
			if (ev.key.scancode == sf::Keyboard::Scan::W) {
				//std::cout << "CAMERA: W KEY PRESSED" << std::endl;
				movingForward = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::S) {
				//std::cout << "CAMERA: S KEY PRESSED" << std::endl;
				movingBackward = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::A) {
				//std::cout << "CAMERA: A KEY PRESSED" << std::endl;
				movingLeft = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::D) {
				//std::cout << "CAMERA: D KEY PRESSED" << std::endl;
				movingRight = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::Space) {
				//std::cout << "CAMERA: SPACE KEY PRESSED" << std::endl;
				movingUp = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::LControl) {
				//std::cout << "CAMERA: LEFT CONTROL PRESSED" << std::endl;
				movingDown = true;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::L) {
				//std::cout << "CAMERA: LEFT CONTROL PRESSED" << std::endl;
				locked = !locked;
			}
		}
		else if (ev.type == sf::Event::KeyReleased) {
			if (ev.key.scancode == sf::Keyboard::Scan::W) {
				//std::cout << "CAMERA: W KEY RELEASED" << std::endl;
				movingForward = false;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::S) {
				//std::cout << "CAMERA: S KEY RELEASED" << std::endl;
				movingBackward = false;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::A) {
				//std::cout << "CAMERA: A KEY RELEASED" << std::endl;
				movingLeft = false;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::D) {
				//std::cout << "CAMERA: D KEY RELEASED" << std::endl;
				movingRight = false;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::Space) {
				//std::cout << "CAMERA: SPACE KEY RELEASED" << std::endl;
				movingUp = false;
			}
			else if (ev.key.scancode == sf::Keyboard::Scan::LControl) {
				//std::cout << "CAMERA: LControl RELEASED" << std::endl;
				movingDown = false;
			}
		}
		break;
	case USER:
		EventEnums user_ev = get<EventEnums>(event.event);
		if (user_ev == UPDATE) {
			update();
		}
		break;
	}
}

