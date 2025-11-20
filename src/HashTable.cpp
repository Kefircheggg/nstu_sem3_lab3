#include "../include/HashTable.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

HashTable::HashTable(size_t initialCapacity) : size(0), capacity(initialCapacity) {
    table.resize(capacity, nullptr);
}

HashTable::~HashTable() {
    clear();
}

size_t HashTable::hashFunction(const std::string& key) const {
    std::hash<std::string> hasher;
    return hasher(key) % capacity;
}

void HashTable::insert(const std::string& key, const std::string& value) {
    size_t index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current) {
        if (current->key == key) {
            current->value = value; // Update value
            return;
        }
        current = current->next;
    }
    
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    size++;
}

std::string HashTable::get(const std::string& key) const {
    size_t index = hashFunction(key);
    HashNode* current = table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    throw std::runtime_error("Key not found");
}

void HashTable::remove(const std::string& key) {
    size_t index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

bool HashTable::contains(const std::string& key) const {
    size_t index = hashFunction(key);
    HashNode* current = table[index];
    while (current) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

size_t HashTable::length() const {
    return size;
}

void HashTable::print() const {
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i]) {
            std::cout << "[" << i << "]: ";
            HashNode* current = table[i];
            while (current) {
                std::cout << "{" << current->key << ": " << current->value << "} -> ";
                current = current->next;
            }
            std::cout << "NULL" << std::endl;
        }
    }
}

void HashTable::clear() {
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = table[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    size = 0;
}

void HashTable::serialize(const std::string& filename) const {
    json j = json::array();
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = table[i];
        while (current) {
            j.push_back({{"key", current->key}, {"value", current->value}});
            current = current->next;
        }
    }
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file << j.dump(4);
    file.close();
}

void HashTable::deserialize(const std::string& filename) {
    clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    json j;
    file >> j;
    for (const auto& item : j) {
        insert(item["key"], item["value"]);
    }
    file.close();
}

void HashTable::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = table[i];
        while (current) {
            int kLen = current->key.length();
            file.write(reinterpret_cast<const char*>(&kLen), sizeof(kLen));
            file.write(current->key.c_str(), kLen);
            
            int vLen = current->value.length();
            file.write(reinterpret_cast<const char*>(&vLen), sizeof(vLen));
            file.write(current->value.c_str(), vLen);
            
            current = current->next;
        }
    }
    file.close();
}

void HashTable::deserializeBinary(const std::string& filename) {
    clear();
    std::ifstream file(filename, std::ios::binary);
    while (file.peek() != EOF) {
        int kLen;
        file.read(reinterpret_cast<char*>(&kLen), sizeof(kLen));
        char* kBuf = new char[kLen + 1];
        file.read(kBuf, kLen);
        kBuf[kLen] = '\0';
        std::string key(kBuf);
        delete[] kBuf;
        
        int vLen;
        file.read(reinterpret_cast<char*>(&vLen), sizeof(vLen));
        char* vBuf = new char[vLen + 1];
        file.read(vBuf, vLen);
        vBuf[vLen] = '\0';
        std::string value(vBuf);
        delete[] vBuf;
        
        insert(key, value);
    }
    file.close();
}
