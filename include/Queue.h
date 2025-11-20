#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

struct QueueNode {
    std::string data;
    QueueNode* next;
    QueueNode(std::string val) : data(val), next(nullptr) {}
};

class Queue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
    int count;

public:
    Queue();
    ~Queue();

    void enqueue(const std::string& value);
    void dequeue();
    std::string front() const;
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
