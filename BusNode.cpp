#include "BusNode.h"

BusNode::BusNode(ObjectID tag, EventBus* eventBus)
{
    this->eventBus = eventBus;
    this->eventBus->addReceiver(tag, this->getNotifyFunc());
    this->id = tag;
}

std::function<void(Event)> BusNode::getNotifyFunc()
{
    auto eventListener = [=](Event event) -> void
    {
        this->onNotify(event);
    };
    return eventListener;
}

void BusNode::send(std::shared_ptr<Event> event) {
    eventBus->sendMessage(event);
}

void BusNode::onNotify(Event event) {
    printf("Forgot to implement onNotify\n");
}

void BusNode::update() {
    printf("Forgot to implement update\n");
}

EventBus* BusNode::getBus() {
    return eventBus;
}

void BusNode::setBus(EventBus* bus) {
    eventBus = bus;
}

void BusNode::initializeEvents() {
    
}

void BusNode::sendEvent(EventType type, std::variant<sf::Event, EventEnums> passed_event, int receiver) {
    std::shared_ptr<struct Event> new_event = std::make_shared<Event>(type, passed_event, receiver);
    eventBus->sendMessage(new_event);
}
