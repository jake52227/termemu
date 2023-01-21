#ifndef TEXT_CONTAINER_HPP
#define TEXT_CONTAINER_HPP

#include <string>
#include <deque>
#include <memory>

// a container class which stores text for rendering
class TextContainer
{
public:
    TextContainer(unsigned maximumCapacity);
    void store(const std::string text);
    void removeLast();
    unsigned getCount();
    std::deque<std::string>::reverse_iterator getIteratorToLatest();
    std::deque<std::string>::reverse_iterator getIteratorToOldest();
private:
    unsigned storageCounter;
    unsigned maximumCapacity;
    std::unique_ptr<std::deque<std::string>> container;
};


#endif