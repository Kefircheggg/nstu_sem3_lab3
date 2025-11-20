#include <iostream>
#include <cassert>
#include <string>
#include "../include/Array.h"
#include "../include/SinglyLinkedList.h"
#include "../include/DoublyLinkedList.h"
#include "../include/Stack.h"
#include "../include/Queue.h"
#include "../include/HashTable.h"
#include "../include/RBTree.h"

void testArray() {
    std::cout << "Testing Array..." << std::endl;
    Array arr;
    arr.pushBack("test1");
    arr.pushBack("test2");
    assert(arr.length() == 2);
    assert(arr.get(0) == "test1");
    assert(arr.get(1) == "test2");
    
    arr.serialize("test_array.txt");
    Array arr2;
    arr2.deserialize("test_array.txt");
    assert(arr2.length() == 2);
    assert(arr2.get(0) == "test1");
    assert(arr2.get(1) == "test2");
    std::cout << "Array tests passed!" << std::endl;
}

void testSinglyLinkedList() {
    std::cout << "Testing SinglyLinkedList..." << std::endl;
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    assert(list.length() == 2);
    list.popFront();
    assert(list.length() == 1);
    assert(list.search("B") == true);
    assert(list.search("A") == false);
    std::cout << "SinglyLinkedList tests passed!" << std::endl;
}

void testStack() {
    std::cout << "Testing Stack..." << std::endl;
    Stack stack;
    stack.push("1");
    stack.push("2");
    assert(stack.top() == "2");
    stack.pop();
    assert(stack.top() == "1");
    std::cout << "Stack tests passed!" << std::endl;
}

void testQueue() {
    std::cout << "Testing Queue..." << std::endl;
    Queue q;
    q.enqueue("1");
    q.enqueue("2");
    assert(q.front() == "1");
    q.dequeue();
    assert(q.front() == "2");
    std::cout << "Queue tests passed!" << std::endl;
}

void testHashTable() {
    std::cout << "Testing HashTable..." << std::endl;
    HashTable ht;
    ht.insert("key1", "val1");
    ht.insert("key2", "val2");
    assert(ht.get("key1") == "val1");
    assert(ht.get("key2") == "val2");
    assert(ht.contains("key1") == true);
    std::cout << "HashTable tests passed!" << std::endl;
}

void testRBTree() {
    std::cout << "Testing RBTree..." << std::endl;
    RBTree tree;
    tree.insert("50");
    tree.insert("30");
    tree.insert("70");
    assert(tree.search("50") == true);
    assert(tree.search("30") == true);
    assert(tree.search("70") == true);
    assert(tree.search("90") == false);

    tree.serializeBinary("test_tree.bin");
    RBTree tree2;
    tree2.deserializeBinary("test_tree.bin");
    assert(tree2.search("50") == true);
    assert(tree2.search("30") == true);
    std::cout << "RBTree tests passed!" << std::endl;
}

int main() {
    testArray();
    testSinglyLinkedList();
    testStack();
    testQueue();
    testHashTable();
    testRBTree();
    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}
