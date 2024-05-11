#pragma once
#include "EventBus.h"

class BusNode {
public:
    BusNode() = delete;
    BusNode(ObjectID tag, EventBus* eventBus);
    virtual void update();
    EventBus* getBus();
    void setBus(EventBus* bus);
    void sendEvent(EventType type, std::variant<sf::Event, EventEnums> passed_event, int receiver);
protected:
    EventBus* eventBus;
    std::function<void(Event)> getNotifyFunc();
    void send(std::shared_ptr<Event> message);
    virtual void onNotify(Event event);
    uint32_t userEvent1;
    ObjectID id;
private:

};