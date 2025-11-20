#include "../include/Queue.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Queue::Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

Queue::~Queue() {
    clear();
}

void Queue::enqueue(const std::string& value) {
    QueueNode* newNode = new QueueNode(value);
    if (isEmpty()) {
        frontNode = rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

void Queue::dequeue() {
    if (isEmpty()) return;
    QueueNode* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    delete temp;
    count--;
}

std::string Queue::front() const {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return frontNode->data;
}

bool Queue::isEmpty() const {
    return frontNode == nullptr;
}

int Queue::size() const {
    return count;
}

void Queue::print() const {
    QueueNode* temp = frontNode;
    while (temp) {
        std::cout << temp->data << " -> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

void Queue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}

void Queue::serialize(const std::string& filename) const {
    json j = json::array();
    QueueNode* temp = frontNode;
    while (temp) {
        j.push_back(temp->data);
        temp = temp->next;
    }
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file << j.dump(4);
    file.close();
}

void Queue::deserialize(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    json j;
    file >> j;
    clear();
    for (const auto& item : j) {
        enqueue(item.get<std::string>());
    }
    file.close();
}

void Queue::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    QueueNode* temp = frontNode;
    while (temp) {
        int len = temp->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(temp->data.c_str(), len);
        temp = temp->next;
    }
    file.close();
}

void Queue::deserializeBinary(const std::string& filename) {
    clear();
    std::ifstream file(filename, std::ios::binary);
    while (file.peek() != EOF) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buffer = new char[len + 1];
        file.read(buffer, len);
        buffer[len] = '\0';
        enqueue(std::string(buffer));
        delete[] buffer;
    }
    file.close();
}
