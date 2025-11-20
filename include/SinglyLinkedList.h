#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <string>
#include <iostream>
#include <fstream>

struct SNode {
    std::string data;
    SNode* next;
    SNode(std::string val) : data(val), next(nullptr) {}
};

class SinglyLinkedList {
private:
    SNode* head;
    int size;

public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    void pushBack(const std::string& value);
    void pushFront(const std::string& value);
    void popBack();
    void popFront();
    void remove(const std::string& value);
    bool search(const std::string& value) const;
    int length() const;
    void print() const;
    void clear();

    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
    void serializeBinary(const std::string& filename) const;
    void deserializeBinary(const std::string& filename);
};

#endif
