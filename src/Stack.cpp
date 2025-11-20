#include "../include/Stack.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Stack::Stack() : topNode(nullptr), count(0) {}

Stack::~Stack() {
    clear();
}

void Stack::push(const std::string& value) {
    StackNode* newNode = new StackNode(value);
    newNode->next = topNode;
    topNode = newNode;
    count++;
}

void Stack::pop() {
    if (isEmpty()) return;
    StackNode* temp = topNode;
    topNode = topNode->next;
    delete temp;
    count--;
}

std::string Stack::top() const {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty");
    }
    return topNode->data;
}

bool Stack::isEmpty() const {
    return topNode == nullptr;
}

int Stack::size() const {
    return count;
}

void Stack::print() const {
    StackNode* temp = topNode;
    while (temp) {
        std::cout << temp->data << " -> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

void Stack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

void Stack::serialize(const std::string& filename) const {
    json j = json::array();
    StackNode* temp = topNode;
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

void Stack::deserialize(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    json j;
    file >> j;
    clear();
    // Iterate in reverse to push bottom elements first, restoring the stack order
    for (auto it = j.rbegin(); it != j.rend(); ++it) {
        push(it->get<std::string>());
    }
    file.close();
}

void Stack::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    StackNode* temp = topNode;
    while (temp) {
        int len = temp->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(temp->data.c_str(), len);
        temp = temp->next;
    }
    file.close();
}

void Stack::deserializeBinary(const std::string& filename) {
    clear();
    std::ifstream file(filename, std::ios::binary);
    Stack tempStack;
    while (file.peek() != EOF) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buffer = new char[len + 1];
        file.read(buffer, len);
        buffer[len] = '\0';
        tempStack.push(std::string(buffer));
        delete[] buffer;
    }
    file.close();
    
    while (!tempStack.isEmpty()) {
        push(tempStack.top());
        tempStack.pop();
    }
}
