#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <string>
#include <iostream>
#include <fstream>

struct DNode {
    std::string data;
    DNode* next;
    DNode* prev;
    DNode(std::string val) : data(val), next(nullptr), prev(nullptr) {}
};

class DoublyLinkedList {
private:
    DNode* head;
    DNode* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

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
