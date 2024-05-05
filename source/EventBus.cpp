#include "EventBus.h"
#include <iostream>


EventBus::EventBus(): event_queue() {}
EventBus::~EventBus(){}

void EventBus::addReceiver(int tag, std::function<void(Event)> eventReceiver)
{
    receivers.insert(std::pair<int, std::function<void(Event)>>(tag, eventReceiver));
}

void EventBus::removeReceiver(int tag)
{
    receivers.erase(tag);
}

void EventBus::sendMessage(std::shared_ptr<Event> passedEvent)
{
    event_queue.push(passedEvent);
}

void EventBus::notify()
{
    //std::cout << "Number of events queued: " << event_queue.size() << std::endl;
    while (!event_queue.empty())
    {
        std::shared_ptr<Event> ev = event_queue.front();
        for (auto it = receivers.begin(); it != receivers.end(); it++)
        {
            it->second(*ev);
        }
        event_queue.pop();
    }
    //std::cout << "Number of events queued after loop: " << event_queue.size() << std::endl;
}