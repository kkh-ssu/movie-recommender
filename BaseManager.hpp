#pragma once

class BaseManager {
public:    
    virtual void display() const = 0;
    virtual int size() const = 0;
};