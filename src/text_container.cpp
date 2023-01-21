#include "text_container.hpp"

TextContainer::TextContainer(unsigned maximumCapacity)
{
    this->storageCounter = 0;
    this->maximumCapacity = maximumCapacity;
    this->container = std::make_unique<std::deque<std::string>>();
}

void TextContainer::store(const std::string text)
{
    if (this->storageCounter == this->maximumCapacity) {
        this->container->pop_front();
        --this->storageCounter;
    }
    this->container->push_back(text);
    ++this->storageCounter;
}

void TextContainer::removeLast()
{
    if (this->storageCounter > 0) {
        this->container->pop_front();
        --this->storageCounter;
    }
}

unsigned TextContainer::getCount()
{
    return this->storageCounter;
}

std::deque<std::string>::reverse_iterator TextContainer::getIteratorToLatest()
{
    return this->container->rbegin();
}

std::deque<std::string>::reverse_iterator TextContainer::getIteratorToOldest()
{
    return this->container->rend();
}