#include <iostream>
#include "include/Array.h"
#include "include/SinglyLinkedList.h"
#include "include/DoublyLinkedList.h"
#include "include/Stack.h"
#include "include/Queue.h"
#include "include/HashTable.h"
#include "include/RBTree.h"
#include <chrono>

void benchmark();

int main() {
    std::cout << "Lab 3 Demo" << std::endl;

    // Array Demo
    std::cout << "\n--- Array ---" << std::endl;
    Array arr;
    arr.pushBack("Hello");
    arr.pushBack("World");
    arr.print();
    arr.serialize("array.txt");
    arr.clear();
    arr.deserialize("array.txt");
    std::cout << "Deserialized: ";
    arr.print();

    // HashTable Demo
    std::cout << "\n--- HashTable ---" << std::endl;
    HashTable ht;
    ht.insert("key1", "value1");
    ht.insert("key2", "value2");
    ht.insert("key3", "value3");
    std::cout << "Get key2: " << ht.get("key2") << std::endl;
    ht.print();
    ht.remove("key2");
    std::cout << "After removing key2:" << std::endl;
    ht.print();

    // RBTree Demo
    std::cout << "\n--- RBTree ---" << std::endl;
    RBTree tree;
    tree.insert("Apple");
    tree.insert("Banana");
    tree.insert("Cherry");
    tree.print();
    tree.serializeBinary("tree.bin");
    tree.clear();
    tree.deserializeBinary("tree.bin");
    std::cout << "Deserialized Binary: ";
    tree.print();

    benchmark();

    return 0;
}

void benchmark() {
    std::cout << "\n--- Benchmarks ---" << std::endl;
    Array arr;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        arr.pushBack("test");
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Array pushBack 10000 items: " << elapsed.count() << "s" << std::endl;

    RBTree tree;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i) {
        tree.insert(std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "RBTree insert 10000 items: " << elapsed.count() << "s" << std::endl;
}
