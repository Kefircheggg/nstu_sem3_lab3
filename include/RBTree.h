#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <iostream>
#include <fstream>

enum Color {
    RED,
    BLACK
};

struct Node {
    std::string data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(std::string val, Color col, Node* l, Node* r, Node* p)
        : data(val), color(col), left(l), right(r), parent(p) {}
};

class RBTree {
private:
    Node* root;
    Node* nil;

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node) const;
    void inorderPrint(Node* node) const;
    void clearHelper(Node* node);
    
    // Serialization helpers
    void serializeBinaryHelper(Node* node, std::ofstream& file) const;

public:
    RBTree();
    ~RBTree();

    void insert(const std::string& value);
    bool search(const std::string& value) const;
    void remove(const std::string& value);
    int size() const; // Note: Lab 1 had 'count' in struct, I'll compute or maintain it. Lab 1 struct had 'count'.
    void clear();
    void print() const;

    // Serialization
    void serialize(const std::string& filename) const;
    void deserialize(const std::string& filename);

    void serializeBinary(const std::string& filename) const;
    void deserializeBinary(const std::string& filename);
};

#endif
