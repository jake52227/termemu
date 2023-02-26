#include "text_container.hpp"
#include "utility.hpp"


std::unique_ptr<TextContainer> TextContainer::create(unsigned maximumCapacity)
{
    std::unique_ptr<TextContainer> textContainer = std::make_unique<TextContainer>(TextContainer());
    textContainer->storageCounter = 0;
    textContainer->maximumCapacity = maximumCapacity;
    textContainer->container = std::make_unique<std::deque<std::string>>();
    return textContainer;
}


void TextContainer::clearStorage()
{
    this->container->clear();
}

// store the string in front of the deque.
void TextContainer::store(const std::string text)
{
    this->container->push_front(text);
}

void TextContainer::removeLast()
{
    if (this->storageCounter > 0)
    {
        this->container->pop_back();
        --this->storageCounter;
    }
}

unsigned TextContainer::getCount()
{
    return this->storageCounter;
}

std::deque<std::string>::iterator TextContainer::getIteratorToLatest()
{
    return this->container->begin();
}

std::deque<std::string>::iterator TextContainer::getIteratorToOldest()
{
    return this->container->end();
}