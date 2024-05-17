/**
This application renders a textured mesh that was loaded with Assimp.
*/
#include "EventBus.h"
#include "Bunny.h"
#include "Interactor.h"
#include "Renderer.h"
#include "Room.h"
#include "AnimBunny.h"
#include "Gman.h"
#include "Bunny1.h"
#include "Sound.h"
#include "Table.h"
#include "Brickwall.h"


int main() {
	// event bus and renderer must be declared first before other objects
	EventBus event_bus;
	Renderer renderer(&event_bus);
	Interactor interactor(&event_bus);
	Sound sound(&event_bus);

	// other objects
	//Bunny bunny(&event_bus);
	//Room room(&event_bus);
	//AnimBunny animbunny(&event_bus);
	//Gman gman(&event_bus);

	// Bunny Scene
	//Bunny1 bunny1(&event_bus);


	Bunny bunny(&event_bus);
	Room room(&event_bus);
	//AnimBunny animbunny(&event_bus);

	Table table(&event_bus);
	//Bunny1 bunny1(&event_bus);
	Brickwall brickwall(&event_bus);


	// main loop
	bool running = true;
	while (running) {

		// renderer owns the window. Tell it to poll for any SFML events
		running = renderer.pollEvents();

		// after any SFML events, tell all BusNodes to update. Update should be the last message
		// received after handling any other events
		std::shared_ptr<struct Event> update = std::make_shared<Event>(USER, UPDATE, GLOBAL);
		event_bus.sendMessage(update);

		// tell event bus to notify all nodes of the events
		event_bus.notify();

		// tell renderer to render
		renderer.render();
	}
	return 0;
}


