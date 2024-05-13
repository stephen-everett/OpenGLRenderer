#include "Bunny1.h"
#include "AssimpImport.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

Bunny1::Bunny1(EventBus* eventBus) : BusNode(BUNNY1, eventBus) {
	model = assimpLoad("models/bunny_textured.obj", true);
	std::shared_ptr<Object3D> obj = model;
	model->move(glm::vec3(0, 0, 0));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
	time = c.getElapsedTime();
	elapsedTime = 0;
	isAnimated = false;


	// For sequential animation loading
	/*animator.addAnimation(
		[obj]() {
			return std::make_shared<TranslationAnimation>(obj, 2, glm::vec3(0, 3, 0));
		}
        
	);

	// then moves exactly to(-1, -1, -1)
	animator.addAnimation(
		[obj]() {
			return std::make_shared<TranslationAnimation>(obj, 1, glm::vec3(-1, -1, -1) * obj->getPosition());
		}
	);*/

	
	// For combined animation loading
	// 
	// Make vector of animation objects
	/*std::vector<std::shared_ptr<Animation>> anims;

	// Load animations into vector
	anims.push_back(std::make_unique<TranslationAnimation>(obj, 5, glm::vec3(0, 3, 0)));
	anims.push_back(std::make_unique<RotationAnimation>(obj, 5, glm::vec3(0, 2 * M_PI, 0)));

	auto combinedAnim = std::make_unique<CombinedAnimation>(obj, anims);

	animator.addAnimation(
		[&combinedAnim]() {
			return std::move(combinedAnim);
		}
	);*/
	/*
	//// ALL MOVEMENT TESTS
	//// MOVE
	animator.addAnimation(
		[obj]() {
			return std::make_shared<TranslationAnimation>(obj, 2, glm::vec3(0, 3, 0));
		}
	);
	// PAUSE
	animator.addAnimation(
		[obj]() {
			return std::make_shared<PauseAnimation>(obj, 2);
		}
	);
	// ROTATE
	animator.addAnimation(
		[obj]() {
			return std::make_shared<RotationAnimation>(obj, 2, glm::vec3(0, 2 * M_PI, 0));
		}
	);
	// RESET POSITION
	animator.addAnimation(
		[obj]() {
			return std::make_shared<TranslationAnimation>(obj, 1, glm::vec3(-1, -1, -1) * obj->getPosition());
		}
	);
	*/
	//// BEZIER CURVE
	//animator.addAnimation(
	//	[obj]() {
	//		return std::make_unique<BezierTranslationAnimation>(obj, 3,
	//		glm::vec3(0, 0, 0),
	//		glm::vec3(0, 2, 0),
	//		glm::vec3(2, 2, 0),
	//		glm::vec3(2, 0, 0));
	//	}
	//);

	// PAUSE
	//animator.addAnimation(
	//	[obj]() {
	//		return std::make_shared<PauseAnimation>(obj, 2);
	//	}
	//);

	// MOVE forward and rotate 90 degrees
	/*animator.addAnimation(
		[obj]() {
			return std::make_shared<TranslationAnimation>(obj, 2, glm::vec3(0, 3, 0));
		}
	);
	animator.addAnimation(
		[obj]() {
			return std::make_shared<RotationAnimation>(obj, 2, glm::vec3(0, M_PI / 2, 0));
		}
	);*/

	// For combined animation loading
	// 
	// Make vector of animation objects
	//std::vector<std::shared_ptr<Animation>> anims;

	//// MOVE forward and rotate 90 degrees
	//anims.push_back(std::make_unique<TranslationAnimation>(obj, 2, glm::vec3(3, 0, 0)));
	//anims.push_back(std::make_unique<RotationAnimation>(obj, 2, glm::vec3(0, M_PI / 2, 0)));

	//auto combinedAnim = std::make_unique<CombinedAnimation>(obj, anims);

	//animator.addAnimation(
	//	[&combinedAnim]() {
	//		return std::move(combinedAnim);
	//	}
	//);



	//////////////// Creating Animation Vectors //////////////////////
	// Make vector of animation objects
	std::vector<std::shared_ptr<Animation>> anims;
	
	//// Load animations into vectors
	// STEP LEFT (move forward AND rotate 45 degrees CW)
	anims.push_back(std::make_shared<TranslationAnimation>(obj, 0.15, glm::vec3(-0.16, 0, 0)));
	anims.push_back(std::make_shared<RotationAnimation>(obj, 0.1, glm::vec3(0, M_PI / 8, 0)));
	auto animLeftStep = std::make_shared<CombinedAnimation>(obj, anims);
	anims.clear();

	// STEP RIGHT
	anims.push_back(std::make_shared<TranslationAnimation>(obj, 0.15, glm::vec3(-0.16, 0, 0)));
	anims.push_back(std::make_shared<RotationAnimation>(obj, 0.1, glm::vec3(0, -M_PI / 8, 0)));
	auto animRightStep = std::make_shared<CombinedAnimation>(obj, anims);
	anims.clear();

	// Rotate back to original position
	//// NEED TROUBLESHOOTING
	anims.push_back(std::make_shared<TranslationAnimation>(obj, 0.25, glm::vec3(-0.08, 0, 0)));
	anims.push_back(std::make_shared<RotationAnimation>(obj, 0.25, glm::vec3(-1, -1, -1) * obj->getOrientation()));
	auto animRotateBack = std::make_shared<CombinedAnimation>(obj, anims);
	anims.clear();
	

	// WIGGLE left and right
	anims.push_back(std::make_shared<RotationAnimation>(obj, 0.05, glm::vec3(0, M_PI / 8, 0)));
	auto animTwitchLeft = std::make_shared<CombinedAnimation>(obj, anims);
	anims.clear();

	anims.push_back(std::make_shared<RotationAnimation>(obj, 0.05, glm::vec3(0, -M_PI / 8, 0)));
	auto animTwitchRight = std::make_shared<CombinedAnimation>(obj, anims);
	anims.clear();

	
	//////////////////////////// Loading Animation Vectors / CREATING THE SCENE ////////////////////////////
	// Creating a "bouncy" bunny
	//// BEZIER CURVE JUMPS
	// Loop to create a bunny that bounces, with smaller bounce each time
	// Bounces left because of the negative x values
	for (double i = 1; i > 0.25; i -= 0.25) {
		animator.addAnimation(
			[obj, i]() {
				return std::make_unique<BezierTranslationAnimation>(obj, i,
				glm::vec3(0, 0, 0),
				glm::vec3(0, i, 0),
				glm::vec3(-i, i, 0),
				glm::vec3(-i, 0, 0));
			}
		);
	}
	//PAUSE
	animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 1); });
	// Creating life-like bunny with twitches
	for (int i = 0; i < 2; i++) {
		// Wiggle and return to center
		animator.addAnimation([animTwitchLeft]() { return animTwitchLeft; });
		animator.addAnimation([animTwitchRight]() { return animTwitchRight; });
		animator.addAnimation([animTwitchRight]() { return animTwitchRight; });
		animator.addAnimation([animTwitchLeft]() { return animTwitchLeft; });
		animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 1); });
	}
	
	// Bunny walks left
	for (int i = 0; i < 4; i++) {
		animator.addAnimation([animLeftStep]() { return animLeftStep; });
		//animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 0.5); });
		animator.addAnimation([animRightStep]() { return animRightStep; });
		//animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 0.5); });
		animator.addAnimation([animRightStep]() { return animRightStep; });
		//animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 0.5); });
		animator.addAnimation([animLeftStep]() { return animLeftStep; });
		//animator.addAnimation([obj]() { return std::make_shared<PauseAnimation>(obj, 0.5); });
	}

}

void Bunny1::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void Bunny1::update() {
	auto now = c.getElapsedTime();
	auto diff = now - time;
	
	animator.tick(diff.asSeconds());
	time = now;
	if (isAnimated) {
		elapsedTime += diff.asSeconds();
		if (elapsedTime >= 1 && elapsedTime < 2) {
			std::shared_ptr<struct Event>  play_sound = std::make_shared<Event>(USER, PLAY_SOUND, SOUND);
			eventBus->sendMessage(play_sound);
		}
		else if (elapsedTime >= 2 && elapsedTime <= 3) {
			std::shared_ptr<struct Event>  play_sound = std::make_shared<Event>(USER, PLAY_SOUND, SOUND);
			eventBus->sendMessage(play_sound);
		}
		else {
			elapsedTime = 0;
			isAnimated = false;
		}
	}
}

void Bunny1::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		sf::Event sf_event = get<sf::Event>(event.event);
		if (sf_event.type == sf::Event::KeyPressed) {
			if (sf_event.key.scancode == sf::Keyboard::Scan::T) {
				playAnimation();
			}
		}
		break;
	case USER:
		EventEnums ev = get<EventEnums>(event.event);
		if (ev == UPDATE) {
			update();
		}
		break;
	}
}

void Bunny1::playAnimation() {
	animator.start();
	isAnimated = true;
	std::shared_ptr<struct Event>  play_sound = std::make_shared<Event>(USER, PLAY_SOUND, SOUND);
	eventBus->sendMessage(play_sound);
}
