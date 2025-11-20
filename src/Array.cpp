#include "../include/Array.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Array::Array(int initialCapacity) {
    if (initialCapacity <= 0) initialCapacity = 10;
    capacity = initialCapacity;
    size = 0;
    data = new std::string[capacity];
}

Array::~Array() {
    delete[] data;
}

void Array::resize(int newCapacity) {
    std::string* newData = new std::string[newCapacity];
    for (int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void Array::pushBack(const std::string& value) {
    if (size == capacity) {
        resize(capacity * 2);
    }
    data[size++] = value;
}

void Array::insertAt(int index, const std::string& value) {
    if (index < 0 || index > size) {
        throw std::out_of_range("Index out of range");
    }
    if (size == capacity) {
        resize(capacity * 2);
    }
    for (int i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

std::string Array::get(int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

void Array::removeAt(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    size--;
}

void Array::set(int index, const std::string& value) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    data[index] = value;
}

int Array::length() const {
    return size;
}

void Array::print() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

void Array::clear() {
    delete[] data;
    capacity = 10;
    size = 0;
    data = new std::string[capacity];
}

void Array::serialize(const std::string& filename) const {
    json j = json::array();
    for (int i = 0; i < size; ++i) {
        j.push_back(data[i]);
    }
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file << j.dump(4);
    file.close();
}

void Array::deserialize(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    json j;
    file >> j;
    clear();
    for (const auto& item : j) {
        pushBack(item.get<std::string>());
    }
    file.close();
}

void Array::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (int i = 0; i < size; ++i) {
        int len = data[i].length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(data[i].c_str(), len);
    }
    file.close();
}

void Array::deserializeBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    clear();
    int newSize;
    file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
    for (int i = 0; i < newSize; ++i) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buffer = new char[len + 1];
        file.read(buffer, len);
        buffer[len] = '\0';
        pushBack(std::string(buffer));
        delete[] buffer;
    }
    file.close();
}
