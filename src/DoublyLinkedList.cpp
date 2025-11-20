#include "../include/DoublyLinkedList.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

void DoublyLinkedList::pushBack(const std::string& value) {
    DNode* newNode = new DNode(value);
    if (!tail) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

void DoublyLinkedList::pushFront(const std::string& value) {
    DNode* newNode = new DNode(value);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
}

void DoublyLinkedList::popBack() {
    if (!tail) return;
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        DNode* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
    }
    size--;
}

void DoublyLinkedList::popFront() {
    if (!head) return;
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        DNode* temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
    }
    size--;
}

void DoublyLinkedList::remove(const std::string& value) {
    DNode* temp = head;
    while (temp) {
        if (temp->data == value) {
            if (temp == head) {
                popFront();
            } else if (temp == tail) {
                popBack();
            } else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                size--;
            }
            return;
        }
        temp = temp->next;
    }
}

bool DoublyLinkedList::search(const std::string& value) const {
    DNode* temp = head;
    while (temp) {
        if (temp->data == value) return true;
        temp = temp->next;
    }
    return false;
}

int DoublyLinkedList::length() const {
    return size;
}

void DoublyLinkedList::print() const {
    DNode* temp = head;
    while (temp) {
        std::cout << temp->data << " <-> ";
        temp = temp->next;
    }
    std::cout << "NULL" << std::endl;
}

void DoublyLinkedList::clear() {
    while (head) {
        popFront();
    }
}

void DoublyLinkedList::serialize(const std::string& filename) const {
    json j = json::array();
    DNode* temp = head;
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

void DoublyLinkedList::deserialize(const std::string& filename) {
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

void DoublyLinkedList::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    DNode* temp = head;
    while (temp) {
        int len = temp->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(temp->data.c_str(), len);
        temp = temp->next;
    }
    file.close();
}

void DoublyLinkedList::deserializeBinary(const std::string& filename) {
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
