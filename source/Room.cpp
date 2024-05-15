#include "Room.h"
#include "Mesh3D.h"
#include "Texture.h"
#include "AssimpImport.h"

Room::Room(EventBus* eventBus) : BusNode(ROOM,eventBus) {
	// This block of code is to create a cube to act as a room
	/*
	sf::Image image;
	image.loadFromFile("models/wall.jpg");
	texture = texture.loadImage(image, "baseTexture");
	std::vector<Texture> textures;
	textures.push_back(texture);
	
	//std::shared_ptr <Mesh3D> mesh = std::make_shared<Mesh3D>(Mesh3D::cube(texture));
	Mesh3D my_mesh = Mesh3D::cube(std::move(texture));
	std::vector<Mesh3D> meshes;
	meshes.push_back(my_mesh);

	//Object3D myObject(std::move(meshes));
	model = std::make_shared<Object3D>(std::move(meshes));
	model->move(glm::vec3(0, 0, -3));

	model->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);
	*/

	// This block of code uses assimp to load a pre-defined mesh
	model = assimpLoad("models/sibenik/sibenik.obj", false);
	model->move(glm::vec3(0, 15, -3));
	std::shared_ptr<struct Event>  register_object = std::make_shared<Event>(USER, R_REGISTER, RENDERER, static_cast<void*>(model.get()));
	eventBus->sendMessage(register_object);


}

void Room::onNotify(Event event) {}
void Room::update() {}
