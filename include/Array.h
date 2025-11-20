#ifndef ARRAY_H
#define ARRAY_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

class Array {
private:
    std::string* data;
    int size;
    int capacity;

    void resize(int newCapacity);

public:
    Array(int initialCapacity = 10);
    ~Array();

    void pushBack(const std::string& value);
    void insertAt(int index, const std::string& value);
    std::string get(int index) const;
    void removeAt(int index);
    void set(int index, const std::string& value);
    int length() const;
    void print() const;
    void clear();

    // Serialization
    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
    
    void serializeBinary(const std::string& filename) const;
    void deserializeBinary(const std::string& filename);
};

#endif
