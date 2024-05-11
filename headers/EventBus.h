#pragma once

#include <functional>
#include <queue>
#include <map>
#include "Events.h"
#include "ObjectID.h"

class EventBus
{
public:
    EventBus();
    ~EventBus();
    void addReceiver(int tag, std::function<void(Event)> eventReceiver);
    void removeReceiver(int tag);
    void sendMessage(std::shared_ptr<Event> event);
    void notify();
private:
    std::queue<std::shared_ptr<Event> > event_queue;
    std::map<int, std::function<void(Event)>> receivers;


};
