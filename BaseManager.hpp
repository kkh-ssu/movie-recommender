#pragma once

class BaseManager {
public:    
    virtual void display() const = 0;
    virtual int size() const = 0;
    virtual void loadFromfile() = 0;
    virtual void saveToFile() = 0;
};