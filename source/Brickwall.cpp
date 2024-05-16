#include "Brickwall.h"
#include "Mesh3D.h"
#include "Texture.h"
#include "AssimpImport.h"
#include <iostream>

Brickwall::Brickwall(EventBus* eventBus) : BusNode(BRICKWALL, eventBus) {
    
    // This block of code uses assimp to load a pre-defined mesh
    /*
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
    */

    // manually create plane and use textures provides by learnopngl
	sf::Image image;
	image.loadFromFile("models/logl_brickwall/brickwall.jpg");
	texture = texture.loadImage(image, "baseTexture");

	sf::Image normal_map;
	normal_map.loadFromFile("models/logl_brickwall/brickwall_normal.jpg");
	normalMap = normalMap.loadImage(normal_map, "normalMap");

	std::vector<Texture> textures;
	textures.push_back(texture);
	textures.push_back(normalMap);

	//std::shared_ptr <Mesh3D> mesh = std::make_shared<Mesh3D>(Mesh3D::cube(texture));
	Mesh3D my_mesh = Mesh3D::square(std::move(textures));
	std::vector<Mesh3D> meshes;
	meshes.push_back(my_mesh);

	//Object3D myObject(std::move(meshes));
	model = std::make_shared<Object3D>(std::move(meshes));
	model->move(glm::vec3(0, 0, -3));
	model->setNorm(false);
	model->setSpec(false);

	model->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
}

void Brickwall::onNotify(Event event) {}
void Brickwall::update() {}
