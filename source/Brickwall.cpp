#include "Brickwall.h"
#include "Mesh3D.h"
#include "Texture.h"
#include "AssimpImport.h"
#include <iostream>

Brickwall::Brickwall(EventBus* eventBus) : BusNode(BRICKWALL, eventBus) {
    // This block of code uses assimp to load a pre-defined mesh
    model = assimpLoad("models/brick-wall-segment-02-tilable/model.obj", true);
    if (model) {
        std::cout << "Brickwall model loaded successfully." << std::endl;
        model->move(glm::vec3(0, 0, -5));
		model->setScale(glm::vec3(0.02f, 0.02f, 0.02f));
        std::shared_ptr<struct Event> register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
        eventBus->sendMessage(register_object);
    }
    else {
        std::cerr << "Failed to load brickwall model." << std::endl;
    }
}

void Brickwall::onNotify(Event event) {}
void Brickwall::update() {}
