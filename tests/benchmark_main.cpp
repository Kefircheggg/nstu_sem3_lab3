#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include "../include/HashTable.h"

using namespace std;

void benchmark_hashtable_put(int num_operations) {
    HashTable ht;
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_operations; ++i) {
        ht.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "HashTable Put (" << num_operations << " ops): " << duration.count() << " ms" << endl;
}

void benchmark_hashtable_get(int num_operations) {
    HashTable ht;
    for (int i = 0; i < num_operations; ++i) {
        ht.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_operations; ++i) {
        ht.get("key" + to_string(i));
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "HashTable Get (" << num_operations << " ops): " << duration.count() << " ms" << endl;
}

int main() {
    cout << "Running Benchmarks..." << endl;
    benchmark_hashtable_put(10000);
    benchmark_hashtable_get(10000);
    return 0;
}
