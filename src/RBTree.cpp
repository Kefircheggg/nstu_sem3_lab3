#include "../include/RBTree.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

RBTree::RBTree() {
    nil = new Node("", BLACK, nullptr, nullptr, nullptr);
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
}

RBTree::~RBTree() {
    clear();
    delete nil;
}

void RBTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right != nil) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == nil) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void RBTree::insertFixup(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RBTree::transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node* RBTree::minimum(Node* node) const {
    while (node->left != nil) {
        node = node->left;
    }
    return node;
}

void RBTree::deleteFixup(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void RBTree::insert(const std::string& value) {
    Node* z = new Node(value, RED, nil, nil, nil);
    Node* y = nil;
    Node* x = root;

    while (x != nil) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == nil) {
        root = z;
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFixup(z);
}

bool RBTree::search(const std::string& value) const {
    Node* x = root;
    while (x != nil && x->data != value) {
        if (value < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return x != nil;
}

void RBTree::remove(const std::string& value) {
    Node* z = root;
    while (z != nil && z->data != value) {
        if (value < z->data) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == nil) return;  // Not found

    Node* y = z;
    Color yOriginalColor = y->color;
    Node* x;

    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK) {
        deleteFixup(x);
    }
}

int sizeHelper(Node* node, Node* nil) {
    if (node == nil) return 0;
    return 1 + sizeHelper(node->left, nil) + sizeHelper(node->right, nil);
}

int RBTree::size() const { return sizeHelper(root, nil); }

void RBTree::clearHelper(Node* node) {
    if (node != nil) {
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}

void RBTree::clear() {
    clearHelper(root);
    root = nil;
}

void RBTree::inorderPrint(Node* node) const {
    if (node != nil) {
        inorderPrint(node->left);
        std::cout << node->data << " ";
        inorderPrint(node->right);
    }
}

void RBTree::print() const {
    inorderPrint(root);
    std::cout << std::endl;
}

// serializeHelper removed


void RBTree::serialize(const std::string& filename) const {
    json j = json::array();
    std::function<void(Node*)> helper = [&](Node* node) {
        if (node != nil) {
            j.push_back(node->data);
            helper(node->left);
            helper(node->right);
        }
    };
    helper(root);

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file << j.dump(4);
    file.close();
}

void RBTree::deserialize(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    json j;
    file >> j;
    clear();
    for (const auto& item : j) {
        insert(item.get<std::string>());
    }
    file.close();
}

void RBTree::serializeBinaryHelper(Node* node, std::ofstream& file) const {
    if (node != nil) {
        int len = node->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(node->data.c_str(), len);
        serializeBinaryHelper(node->left, file);
        serializeBinaryHelper(node->right, file);
    } else {
        int len = -1;  // Marker for nil
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    }
}

void RBTree::serializeBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    serializeBinaryHelper(root, file);
    file.close();
}

void RBTree::deserializeBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    clear();
    // This is tricky because insert balances the tree, so simple insertion might not restore exact
    // structure if we just insert. But for BST/RBT, inserting elements in preorder (which serialize
    // does) should restore the tree content. However, RBT structure (colors) might change if we
    // just insert. But the task asks for serialization/deserialization of DATA. So restoring the
    // content is enough.

    while (file.peek() != EOF) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (len == -1) continue;

        char* buffer = new char[len + 1];
        file.read(buffer, len);
        buffer[len] = '\0';
        insert(std::string(buffer));
        delete[] buffer;
    }
    file.close();
}
