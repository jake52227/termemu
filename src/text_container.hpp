#ifndef TEXT_CONTAINER_HPP
#define TEXT_CONTAINER_HPP

#include <string>
#include <deque>
#include <memory>

// a container class which stores text for rendering
class TextContainer
{
public:
    static std::unique_ptr<TextContainer> create(unsigned maximumCapacity = 200);
    void store(const std::string text);
    void removeLast();
    unsigned getCount();
    void clearStorage();
    std::deque<std::string>::iterator getIteratorToLatest();
    std::deque<std::string>::iterator getIteratorToOldest();
private:
    unsigned storageCounter;
    unsigned maximumCapacity;
    std::unique_ptr<std::deque<std::string>> container;
};


#endif