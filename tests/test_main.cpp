#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "../include/Array.h"
#include "../include/DoublyLinkedList.h"
#include "../include/HashTable.h"
#include "../include/Queue.h"
#include "../include/RBTree.h"
#include "../include/SinglyLinkedList.h"
#include "../include/Stack.h"

// тесты массива

TEST(ArrayTest, BasicOperations) {
    Array arr;
    EXPECT_EQ(arr.length(), 0);
    arr.pushBack("val1");
    EXPECT_EQ(arr.length(), 1);
    EXPECT_EQ(arr.get(0), "val1");
    arr.pushBack("val2");
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr.get(1), "val2");
}

TEST(ArrayTest, InsertAt) {
    Array arr;
    arr.pushBack("A");
    arr.pushBack("C");
    arr.insertAt(1, "B");
    EXPECT_EQ(arr.length(), 3);
    EXPECT_EQ(arr.get(0), "A");
    EXPECT_EQ(arr.get(1), "B");
    EXPECT_EQ(arr.get(2), "C");

    arr.insertAt(0, "Start");
    EXPECT_EQ(arr.get(0), "Start");

    arr.insertAt(arr.length(), "End");
    EXPECT_EQ(arr.get(arr.length() - 1), "End");
}

TEST(ArrayTest, RemoveAt) {
    Array arr;
    arr.pushBack("A");
    arr.pushBack("B");
    arr.pushBack("C");
    arr.removeAt(1);
    EXPECT_EQ(arr.length(), 2);
    EXPECT_EQ(arr.get(0), "A");
    EXPECT_EQ(arr.get(1), "C");

    arr.removeAt(0);
    EXPECT_EQ(arr.get(0), "C");

    arr.removeAt(0);
    EXPECT_EQ(arr.length(), 0);
}

TEST(ArrayTest, Set) {
    Array arr;
    arr.pushBack("Old");
    arr.set(0, "New");
    EXPECT_EQ(arr.get(0), "New");
}

TEST(ArrayTest, Exceptions) {
    Array arr;
    EXPECT_THROW(arr.get(0), std::out_of_range);
    EXPECT_THROW(arr.removeAt(0), std::out_of_range);
    EXPECT_THROW(arr.set(0, "val"), std::out_of_range);

    arr.pushBack("val");
    EXPECT_THROW(arr.get(1), std::out_of_range);
    EXPECT_THROW(arr.insertAt(5, "val"), std::out_of_range);
}

TEST(ArrayTest, Clear) {
    Array arr;
    arr.pushBack("val");
    arr.clear();
    EXPECT_EQ(arr.length(), 0);
}

TEST(ArrayTest, Serialization) {
    Array arr;
    arr.pushBack("test1");
    arr.pushBack("test2");
    arr.serialize("test_array.txt");

    Array arr2;
    arr2.deserialize("test_array.txt");
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.get(0), "test1");
    EXPECT_EQ(arr2.get(1), "test2");
}

TEST(ArrayTest, SerializationBinary) {
    Array arr;
    arr.pushBack("bin1");
    arr.pushBack("bin2");
    arr.serializeBinary("test_array.bin");

    Array arr2;
    arr2.deserializeBinary("test_array.bin");
    EXPECT_EQ(arr2.length(), 2);
    EXPECT_EQ(arr2.get(0), "bin1");
    EXPECT_EQ(arr2.get(1), "bin2");
}

TEST(ArrayTest, Print) {
    Array arr;
    arr.pushBack("A");
    arr.pushBack("B");
    testing::internal::CaptureStdout();
    arr.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("A"), std::string::npos);
    EXPECT_NE(output.find("B"), std::string::npos);
}

TEST(ArrayTest, ResizeStress) {
    Array arr;
    for (int i = 0; i < 100; ++i) {
        arr.pushBack(std::to_string(i));
    }
    EXPECT_EQ(arr.length(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(arr.get(i), std::to_string(i));
    }
}

// тесты односвязного списка

TEST(SinglyLinkedListTest, PushBackFront) {
    SinglyLinkedList list;
    list.pushBack("Back");
    list.pushFront("Front");
    EXPECT_EQ(list.length(), 2);
    EXPECT_TRUE(list.search("Front"));
    EXPECT_TRUE(list.search("Back"));
}

TEST(SinglyLinkedListTest, PopBack) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.popBack();
    EXPECT_EQ(list.length(), 1);
    EXPECT_TRUE(list.search("A"));
    EXPECT_FALSE(list.search("B"));

    list.popBack();
    EXPECT_EQ(list.length(), 0);

    list.popBack();  // не должно крашнуться
}

TEST(SinglyLinkedListTest, PopFront) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.popFront();
    EXPECT_EQ(list.length(), 1);
    EXPECT_FALSE(list.search("A"));
    EXPECT_TRUE(list.search("B"));

    list.popFront();
    EXPECT_EQ(list.length(), 0);

    list.popFront();  // не должно крашнуться
}

TEST(SinglyLinkedListTest, Remove) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    list.remove("B");  // серединка
    EXPECT_EQ(list.length(), 2);
    EXPECT_FALSE(list.search("B"));

    list.remove("A");  // голова
    EXPECT_EQ(list.length(), 1);
    EXPECT_FALSE(list.search("A"));

    list.remove("C");  // хвост
    EXPECT_EQ(list.length(), 0);

    list.remove("NonExistent");  // не должно крашнуться
}

TEST(SinglyLinkedListTest, Clear) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.clear();
    EXPECT_EQ(list.length(), 0);
}

TEST(SinglyLinkedListTest, Serialization) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.serialize("test_sll.txt");

    SinglyLinkedList list2;
    list2.deserialize("test_sll.txt");
    EXPECT_EQ(list2.length(), 2);
    EXPECT_TRUE(list2.search("A"));
    EXPECT_TRUE(list2.search("B"));
}

TEST(SinglyLinkedListTest, SerializationBinary) {
    SinglyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.serializeBinary("test_sll.bin");

    SinglyLinkedList list2;
    list2.deserializeBinary("test_sll.bin");
    EXPECT_EQ(list2.length(), 2);
    EXPECT_TRUE(list2.search("A"));
    EXPECT_TRUE(list2.search("B"));
}

// тесты двусвязного списка

TEST(DoublyLinkedListTest, PushBackFront) {
    DoublyLinkedList list;
    list.pushBack("Back");
    list.pushFront("Front");
    EXPECT_EQ(list.length(), 2);
    EXPECT_TRUE(list.search("Front"));
    EXPECT_TRUE(list.search("Back"));
}

TEST(DoublyLinkedListTest, PopBack) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.popBack();
    EXPECT_EQ(list.length(), 1);
    EXPECT_TRUE(list.search("A"));
    EXPECT_FALSE(list.search("B"));

    list.popBack();
    EXPECT_EQ(list.length(), 0);

    list.popBack();  
}

TEST(DoublyLinkedListTest, PopFront) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.popFront();
    EXPECT_EQ(list.length(), 1);
    EXPECT_FALSE(list.search("A"));
    EXPECT_TRUE(list.search("B"));

    list.popFront();
    EXPECT_EQ(list.length(), 0);

    list.popFront();  
}

TEST(DoublyLinkedListTest, Remove) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    list.remove("B");  // серединка
    EXPECT_EQ(list.length(), 2);
    EXPECT_FALSE(list.search("B"));

    list.remove("A");  // голова
    EXPECT_EQ(list.length(), 1);
    EXPECT_FALSE(list.search("A"));

    list.remove("C");  // хвосты
    EXPECT_EQ(list.length(), 0);

    list.remove("NonExistent");  
}

TEST(DoublyLinkedListTest, Clear) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.clear();
    EXPECT_EQ(list.length(), 0);
}

TEST(DoublyLinkedListTest, Serialization) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.serialize("test_dll.txt");

    DoublyLinkedList list2;
    list2.deserialize("test_dll.txt");
    EXPECT_EQ(list2.length(), 2);
    EXPECT_TRUE(list2.search("A"));
    EXPECT_TRUE(list2.search("B"));
}

TEST(DoublyLinkedListTest, SerializationBinary) {
    DoublyLinkedList list;
    list.pushBack("A");
    list.pushBack("B");
    list.serializeBinary("test_dll.bin");

    DoublyLinkedList list2;
    list2.deserializeBinary("test_dll.bin");
    EXPECT_EQ(list2.length(), 2);
    EXPECT_TRUE(list2.search("A"));
    EXPECT_TRUE(list2.search("B"));
}

// тесты стека

TEST(StackTest, PushPop) {
    Stack stack;
    stack.push("A");
    stack.push("B");
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), "B");

    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), "A");

    stack.pop();
    EXPECT_TRUE(stack.isEmpty());

    stack.pop();  
}

TEST(StackTest, Exceptions) {
    Stack stack;
    EXPECT_THROW(stack.top(), std::runtime_error);
}

TEST(StackTest, Clear) {
    Stack stack;
    stack.push("A");
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
}

TEST(StackTest, Serialization) {
    Stack stack;
    stack.push("A");
    stack.push("B");
    stack.serialize("test_stack.txt");

    Stack stack2;
    stack2.deserialize("test_stack.txt");
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), "B");
    stack2.pop();
    EXPECT_EQ(stack2.top(), "A");
}

TEST(StackTest, SerializationBinary) {
    Stack stack;
    stack.push("A");
    stack.push("B");
    stack.serializeBinary("test_stack.bin");

    Stack stack2;
    stack2.deserializeBinary("test_stack.bin");
    EXPECT_EQ(stack2.size(), 2);
    EXPECT_EQ(stack2.top(), "B");
    stack2.pop();
    EXPECT_EQ(stack2.top(), "A");
}

// тесты очереди

TEST(QueueTest, EnqueueDequeue) {
    Queue q;
    q.enqueue("A");
    q.enqueue("B");
    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.front(), "A");

    q.dequeue();
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.front(), "B");

    q.dequeue();
    EXPECT_TRUE(q.isEmpty());

    q.dequeue();  
}

TEST(QueueTest, Exceptions) {
    Queue q;
    EXPECT_THROW(q.front(), std::runtime_error);
}

TEST(QueueTest, Clear) {
    Queue q;
    q.enqueue("A");
    q.clear();
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, Serialization) {
    Queue q;
    q.enqueue("A");
    q.enqueue("B");
    q.serialize("test_queue.txt");

    Queue q2;
    q2.deserialize("test_queue.txt");
    EXPECT_EQ(q2.size(), 2);
    EXPECT_EQ(q2.front(), "A");
    q2.dequeue();
    EXPECT_EQ(q2.front(), "B");
}

TEST(QueueTest, SerializationBinary) {
    Queue q;
    q.enqueue("A");
    q.enqueue("B");
    q.serializeBinary("test_queue.bin");

    Queue q2;
    q2.deserializeBinary("test_queue.bin");
    EXPECT_EQ(q2.size(), 2);
    EXPECT_EQ(q2.front(), "A");
    q2.dequeue();
    EXPECT_EQ(q2.front(), "B");
}

// тесты хэш таблиц 

TEST(HashTableTest, BasicOperations) {
    HashTable ht;
    ht.insert("key1", "val1");
    EXPECT_TRUE(ht.contains("key1"));
    EXPECT_EQ(ht.get("key1"), "val1");

    ht.insert("key1", "val2");  
    EXPECT_EQ(ht.get("key1"), "val2");

    ht.remove("key1");
    EXPECT_FALSE(ht.contains("key1"));
}

TEST(HashTableTest, Collisions) {
    HashTable ht;
    for (int i = 0; i < 50; ++i) {
        ht.insert("key" + std::to_string(i), "val" + std::to_string(i));
    }
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(ht.get("key" + std::to_string(i)), "val" + std::to_string(i));
    }
}

TEST(HashTableTest, Exceptions) {
    HashTable ht;
    EXPECT_THROW(ht.get("missing"), std::runtime_error);
}

TEST(HashTableTest, Clear) {
    HashTable ht;
    ht.insert("A", "B");
    ht.clear();
    EXPECT_EQ(ht.length(), 0);
    EXPECT_FALSE(ht.contains("A"));
}

TEST(HashTableTest, Serialization) {
    HashTable ht;
    ht.insert("k1", "v1");
    ht.insert("k2", "v2");
    ht.serialize("test_ht.txt");

    HashTable ht2;
    ht2.deserialize("test_ht.txt");
    EXPECT_EQ(ht2.get("k1"), "v1");
    EXPECT_EQ(ht2.get("k2"), "v2");
}

TEST(HashTableTest, SerializationBinary) {
    HashTable ht;
    ht.insert("k1", "v1");
    ht.insert("k2", "v2");
    ht.serializeBinary("test_ht.bin");

    HashTable ht2;
    ht2.deserializeBinary("test_ht.bin");
    EXPECT_EQ(ht2.get("k1"), "v1");
    EXPECT_EQ(ht2.get("k2"), "v2");
}

// КЧД тесты

TEST(RBTreeTest, InsertSearch) {
    RBTree tree;
    tree.insert("50");
    tree.insert("30");
    tree.insert("70");
    EXPECT_TRUE(tree.search("50"));
    EXPECT_TRUE(tree.search("30"));
    EXPECT_TRUE(tree.search("70"));
    EXPECT_FALSE(tree.search("90"));
}

TEST(RBTreeTest, Remove) {
    RBTree tree;
    tree.insert("50");
    tree.insert("30");
    tree.insert("70");
    tree.insert("20");
    tree.insert("40");

    tree.remove("30");
    EXPECT_FALSE(tree.search("30"));
    EXPECT_TRUE(tree.search("20"));
    EXPECT_TRUE(tree.search("40"));

    tree.remove("50");  // Root
    EXPECT_FALSE(tree.search("50"));
    EXPECT_TRUE(tree.search("70"));
}

TEST(RBTreeTest, Size) {
    RBTree tree;
    EXPECT_EQ(tree.size(), 0);
    tree.insert("A");
    EXPECT_EQ(tree.size(), 1);
    tree.insert("B");
    EXPECT_EQ(tree.size(), 2);
    tree.remove("A");
    EXPECT_EQ(tree.size(), 1);
    tree.clear();
    EXPECT_EQ(tree.size(), 0);
}

TEST(RBTreeTest, Serialization) {
    RBTree tree;
    tree.insert("M");
    tree.insert("A");
    tree.insert("Z");
    tree.serialize("test_rbtree.txt");

    RBTree tree2;
    tree2.deserialize("test_rbtree.txt");
    EXPECT_TRUE(tree2.search("M"));
    EXPECT_TRUE(tree2.search("A"));
    EXPECT_TRUE(tree2.search("Z"));
}

TEST(RBTreeTest, SerializationBinary) {
    RBTree tree;
    tree.insert("M");
    tree.insert("A");
    tree.insert("Z");
    tree.serializeBinary("test_rbtree.bin");

    RBTree tree2;
    tree2.deserializeBinary("test_rbtree.bin");
    EXPECT_TRUE(tree2.search("M"));
    EXPECT_TRUE(tree2.search("A"));
    EXPECT_TRUE(tree2.search("Z"));
}

TEST(RBTreeTest, Print) {
    RBTree tree;
    tree.insert("B");
    tree.insert("A");
    tree.insert("C");
    testing::internal::CaptureStdout();
    tree.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("A"), std::string::npos);
    EXPECT_NE(output.find("B"), std::string::npos);
    EXPECT_NE(output.find("C"), std::string::npos);
}

TEST(RBTreeTest, StressTest) {
    RBTree tree;
    
    for (int i = 0; i < 50; ++i) {
        tree.insert(std::to_string(i));
    }
    EXPECT_EQ(tree.size(), 50);

    // удаляем часть
    for (int i = 0; i < 20; ++i) {
        tree.remove(std::to_string(i));
    }
    EXPECT_EQ(tree.size(), 30);

    // проверка что осталось
    for (int i = 20; i < 50; ++i) {
        EXPECT_TRUE(tree.search(std::to_string(i)));
    }
}
