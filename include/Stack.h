#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

struct StackNode {
    std::string data;
    StackNode* next;
    StackNode(std::string val) : data(val), next(nullptr) {}
};

class Stack {
private:
    StackNode* topNode;
    int count;

public:
    Stack();
    ~Stack();

    void push(const std::string& value);
    void pop();
    std::string top() const;
    bool isEmpty() const;
    int size() const;
    void print() const;
    void clear();

    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);
    void serializeBinary(const std::string& filename) const;
    void deserializeBinary(const std::string& filename);
};

#endif
