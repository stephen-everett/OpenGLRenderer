#include "EventBus.h"
#include <iostream>


EventBus::EventBus(): event_queue() {}
EventBus::~EventBus(){}

void EventBus::addReceiver(int tag, std::function<void(Event)> eventReceiver)
{
    //receivers.insert(std::pair<int, std::function<void(Event)>>(tag, eventReceiver));
    receivers.push_back(eventReceiver);
}

void EventBus::addReceiver(std::function<void(Event)> eventReceiver)
{
    //receivers.insert(std::pair<int, std::function<void(Event)>>(tag, eventReceiver));
    receivers.push_back(eventReceiver);
}

void EventBus::removeReceiver(int tag)
{
    //receivers.erase(tag); // if receivers were a hashmap
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
            //it.
            // call function here
            (*it)(*ev);
        }
        event_queue.pop();
    }
    //std::cout << "Number of events queued after loop: " << event_queue.size() << std::endl;
}