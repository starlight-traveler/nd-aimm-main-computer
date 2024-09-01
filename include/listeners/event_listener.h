#pragma once
#include <string>

class EventListener
{
public:
    virtual bool shouldListen(const std::string &event) const = 0;
    virtual void onEvent(const std::string &event) = 0;
    virtual ~EventListener() {}
};