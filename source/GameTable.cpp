#include "GameTable.h"
#include "AssimpImport.h"
#include "Bunny.h"
#include <iostream>
#include <random>
#include <glm/glm.hpp>

GameTable::GameTable(EventBus* eventBus) : BusNode(GAMETABLE, eventBus) {
	// initialize variables
	is_interactable = false;
	model = assimpLoad("models/simple-office-table/table_new/table.obj", true);
	model->setOrientation(glm::vec3(0, 1.6, 0));
	model->move(glm::vec3(-6.4, -0.2, -7.6));
	model->setNorm(false);
	//model->setScale(glm::vec3(0.01, 0.01, 0.01));
	time = c.getElapsedTime();
	elapsedTime = 0;
	// is Animated false until interactable is toggled
	isAnimated = false;

	// register with renderer
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);

	// register with interactor
	glm::vec3 test_pos = model->getPosition();
	interactor = std::make_shared<Interactable>(
		&model->getPosition(),
		[this](bool value) {
			return this->toggleInteractable(value);
		}
	);
	std::shared_ptr<struct Event>  register_interactor = std::make_shared<Event>(USER, I_REGISTER, INTERACTOR, static_cast<void*>(interactor.get()));
	eventBus->sendMessage(register_interactor);

	// load bunny and set starting values
	bunny1 = assimpLoad("models/bunny/bunny_textured.obj", true);
	glm::vec3 initialPositionBunny = glm::vec3(-6.41122, 0.6, -7.57902);
	bunny1->setPosition(initialPositionBunny);
	bunny1->setOrientation(glm::vec3(0, 0, 0));
	bunny1->setScale(glm::vec3(0.8));

	// Add Animator pointer to bunny1
	std::shared_ptr<Object3D> obj = bunny1;

	// load cups and set starting values
	// MIDDLE START
	cup1 = assimpLoad("models/old-metal-cup/untitled.obj", true);
	glm::vec3 initialPositionCup1 = glm::vec3(-6.36122, 1, -7.57902);
	cup1->setPosition(initialPositionCup1);
	cup1->rotate(glm::vec3(M_PI, M_PI / 2, 0));
	cup1->setScale(glm::vec3(0.5));
	// RIGHT START
	cup2 = assimpLoad("models/old-metal-cup/untitled.obj", true);
	glm::vec3 initialPositionCup2 = glm::vec3(-5.66122, 1, -7.57902);
	cup2->setPosition(initialPositionCup2);
	cup2->rotate(glm::vec3(M_PI, M_PI / 2, 0));
	cup2->setScale(glm::vec3(0.5));
	// LEFT START
	cup3 = assimpLoad("models/old-metal-cup/untitled.obj", true);
	glm::vec3 initialPositionCup3 = glm::vec3(-7.06122, 1, -7.57902);
	cup3->setPosition(initialPositionCup3);
	cup3->rotate(glm::vec3(M_PI, M_PI / 2, 0));
	cup3->setScale(glm::vec3(0.5));

	// Add Animator pointers
	std::shared_ptr<Object3D> animCup1 = cup1;
	std::shared_ptr<Object3D> animCup2 = cup2;
	std::shared_ptr<Object3D> animCup3 = cup3;

	// register objects with renderer
	std::shared_ptr<struct Event>  reg_bunny1 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(bunny1.get()));
	eventBus->sendMessage(reg_bunny1);
	std::shared_ptr<struct Event>  reg_cup1 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(cup1.get()));
	eventBus->sendMessage(reg_cup1);
	std::shared_ptr<struct Event>  reg_cup2 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(cup2.get()));
	eventBus->sendMessage(reg_cup2);
	std::shared_ptr<struct Event>  reg_cup3 = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(cup3.get()));
	eventBus->sendMessage(reg_cup3);

	// set bunny physical properties
	//bunny1_properties = PhysicsStructure(5, glm::vec3(0), glm::vec3(0));
	//bunny2_properties = PhysicsStructure(2, glm::vec3(0), glm::vec3(0));
	
	// For combined animations
	std::vector<std::shared_ptr<Animation>> anims;

	///////////////////// Animation Loading ///////////////////////
	
	// have cup come down, pause while move bunny down, bring cup up
	// PAUSE
	animator.addAnimation(
		[animCup1]() {
			return std::make_shared<PauseAnimation>(animCup1, 2);
		}
	);
	
	// MOVE ALL CUPS DOWN TO START
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, 1, glm::vec3(0, -0.28, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, 1, glm::vec3(0, -0.28, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, 1, glm::vec3(0, -0.28, 0)));
	auto combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// PAUSE
	animator.addAnimation(
		[animCup1]() {
			return std::make_shared<PauseAnimation>(animCup1, 1);
		}
	);
	animator.addAnimation(
		[obj]() {
			return std::make_shared<ScaleAnimation>(obj, 0.0f, glm::vec3(0.0f));
		}
	);
	// Load animations into vector
	anims.push_back(std::make_shared<PauseAnimation>(animCup1, 1));


	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);

	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	float gameSpeed = 0.3;
	////////// GAME START //////////
	// Move cup1 forward, cup2 and cup3 back
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// Move cup1 to the left, cup2 to the right, and cup3 forward
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// Additional animations
	// 1. Move cup1 back, cup2 forward, and cup3 to the left
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(-0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 2. Move cup1 to the right, cup2 to the left, and cup3 forward
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, 0.3)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 3. Move cup1 forward, cup2 back, and cup3 to the right
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 4. Move cup1 to the left, cup2 to the right, and cup3 back
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, -0.3)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 5. Move cup1 back, cup2 forward, and cup3 to the left
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(-0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 6. Move cup1 to the right, cup2 to the left, and cup3 forward
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, 0.3)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 7. Move cup1 forward, cup2 back, and cup3 to the right
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 8. Move cup1 to the left, cup2 to the right, and cup3 back
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, -0.3)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 9. Move cup1 back, cup2 forward, and cup3 to the left
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0, 0, -0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(0, 0, 0.3)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(-0.3, 0, 0)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	// 10. Move cup1 to the right, cup2 to the left, and cup3 forward
	anims.push_back(std::make_shared<TranslationAnimation>(animCup1, gameSpeed, glm::vec3(0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup2, gameSpeed, glm::vec3(-0.3, 0, 0)));
	anims.push_back(std::make_shared<TranslationAnimation>(animCup3, gameSpeed, glm::vec3(0, 0, 0.3)));
	combinedAnim = std::make_shared<CombinedAnimation>(obj, anims);
	animator.addAnimation(
		[combinedAnim]() {
			return combinedAnim;
		}
	);
	anims.clear();

	/////////// GAME END ///////////

	// PAUSE
	animator.addAnimation(
		[animCup1]() {
			return std::make_shared<PauseAnimation>(animCup1, 1);
		}
	);

	animator.addAnimation(
		[obj]() {
			return std::make_shared<ScaleAnimation>(obj, 0.0f, glm::vec3(0.8f));
		}
	);


	animator.addAnimation(
		[animCup1, initialPositionCup1]() {
			glm::vec3 moveBack = initialPositionCup1 - animCup1->getPosition();
			return std::make_shared<TranslationAnimation>(animCup1, 3, moveBack);
		}
	);

	// Reset position for cup2 and cup3
	animator.addAnimation(
		[animCup2, initialPositionCup2]() {
			glm::vec3 moveBack = initialPositionCup2 - animCup2->getPosition();
			return std::make_shared<TranslationAnimation>(animCup2, 1, moveBack);
		}
	);
	animator.addAnimation(
		[animCup3, initialPositionCup3]() {
			glm::vec3 moveBack = initialPositionCup3 - animCup3->getPosition();
			return std::make_shared<TranslationAnimation>(animCup3, 1, moveBack);
		}
	);


	// Wait for interactor start
	//animator.start();
}

void GameTable::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	model->render(window, shaderProgram);
}

void GameTable::update() {
	auto now = c.getElapsedTime();
	auto diff = now - time;
	animator.tick(diff.asSeconds());
	time = now;
}

void GameTable::onNotify(Event event) {
	switch (event.event_type) {
	case SFML:
		if (is_interactable) {
			sf::Event ev = get<sf::Event>(event.event);
			if (ev.type == sf::Event::KeyPressed) {
				if (ev.key.scancode == sf::Keyboard::Scan::C) {
					model->setTBN(!model->hasTBN());
					if (model->hasTBN()) {
						std::cout << "Table is now using TBN with Normal Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using TBN with Normal Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::V) {
					model->setSpec(!model->hasSpec());
					if (model->hasSpec()) {
						std::cout << "Table is now using Spec Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using Spec Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::B) {
					model->setNorm(!model->hasNorm());
					if (model->hasNorm()) {
						std::cout << "Table is now using Normal Map" << std::endl;
					}
					else {
						std::cout << "Table is NOT using Normal Map" << std::endl;
					}
				}
				else if (ev.key.scancode == sf::Keyboard::Scan::Up) {
					bunny1->move(glm::vec3(0, .01, 0));
					std::cout << bunny1->getPosition().x << ", " << bunny1->getPosition().y << ", " << bunny1->getPosition().z << std::endl;
				}
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

void GameTable::toggleInteractable(bool toggle) {

	/*
	* Statements for debugging, each time interactable toggles from one state
	* to another for the first time, print it
	*/
	if (toggle == true) {
		if (!is_interactable) {
			std::cout << "GameTable is now interactable" << std::endl;
		}
		if (!isAnimated) {
			playAnimation();
		}
	}
	// toggle is false, but interactable is true
	else {
		if (is_interactable) {
			std::cout << "GameTable is now NOT interactable" << std::endl;
		}
	}

	is_interactable = toggle;
}

void GameTable::playAnimation() {
	animator.start();
	isAnimated = true;
}