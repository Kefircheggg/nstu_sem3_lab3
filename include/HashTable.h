#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>

struct HashNode {
    std::string key;
    std::string value;
    HashNode* next;
    HashNode(std::string k, std::string v) : key(k), value(v), next(nullptr) {}
};

class HashTable {
private:
    std::vector<HashNode*> table;
    size_t size;
    size_t capacity;

    size_t hashFunction(const std::string& key) const;

public:
    HashTable(size_t initialCapacity = 100);
    ~HashTable();

    void insert(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    void remove(const std::string& key);
    bool contains(const std::string& key) const;
    size_t length() const;
    void print() const;
    void clear();

    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
    void serializeBinary(const std::string& filename) const;
    void deserializeBinary(const std::string& filename);
};

#endif
