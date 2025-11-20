#include "../include/SinglyLinkedList.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SinglyLinkedList::SinglyLinkedList() : head(nullptr), size(0) {}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::pushBack(const std::string& value) {
    SNode* newNode = new SNode(value);
    if (!head) {
        head = newNode;
    } else {
        SNode* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    size++;
}

void SinglyLinkedList::pushFront(const std::string& value) {
    SNode* newNode = new SNode(value);
    newNode->next = head;
    head = newNode;
    size++;
}

void SinglyLinkedList::popBack() {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = nullptr;
    } else {
        SNode* temp = head;
        while (temp->next->next) {
            temp = temp->next;
        }
        delete temp->next;
        temp->next = nullptr;
    }
    size--;
}

void SinglyLinkedList::popFront() {
    if (!head) return;
    SNode* temp = head;
    head = head->next;
    delete temp;
    size--;
}

void SinglyLinkedList::remove(const std::string& value) {
    if (!head) return;
    if (head->data == value) {
        popFront();
        return;
    }
    SNode* temp = head;
    while (temp->next && temp->next->data != value) {
        temp = temp->next;
    }
    if (temp->next) {
        SNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        size--;
    }
}

bool SinglyLinkedList::search(const std::string& value) const {
    SNode* temp = head;
    while (temp) {
        if (temp->data == value) return true;
        temp = temp->next;
    }
    return false;
}

int SinglyLinkedList::length() const {
    return size;
}

void SinglyLinkedList::print() const {
    SNode* temp = head;
    while (temp) {
        std::cout << temp->data << " -> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

void SinglyLinkedList::clear() {
    while (head) {
        popFront();
    }
}

void SinglyLinkedList::serialize(const std::string& filename) const {
    json j = json::array();
    SNode* temp = head;
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

void SinglyLinkedList::deserialize(const std::string& filename) {
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

void SinglyLinkedList::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    SNode* temp = head;
    while (temp) {
        int len = temp->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(temp->data.c_str(), len);
        temp = temp->next;
    }
    file.close();
}

void SinglyLinkedList::deserializeBinary(const std::string& filename) {
    clear();
    std::ifstream file(filename, std::ios::binary);
    while (file.peek() != EOF) {
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
