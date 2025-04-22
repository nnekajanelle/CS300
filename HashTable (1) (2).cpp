//============================================================================
// Name        : HashTable.cpp
// Author      : Nneka Hamilton
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <vector>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

const unsigned int DEFAULT_SIZE = 179;

double strToDouble(string str, char ch);

struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;

    Bid() {
        amount = 0.0;
    }
};

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
}

class HashTable {

private:
    struct Node {
        Bid bid;
        unsigned int key;
        Node* next;

        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;
    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

HashTable::HashTable() {
    nodes.resize(tableSize);
}

HashTable::HashTable(unsigned int size) {
    tableSize = size;
    nodes.resize(tableSize);
}

HashTable::~HashTable() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* currentNode = nodes[i].next;
        while (currentNode != nullptr) {
            Node* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
    nodes.clear();
}

unsigned int HashTable::hash(int key) {
    return key % tableSize;
}

void HashTable::Insert(Bid bid) {
    unsigned int key = hash(stoi(bid.bidId));
    Node* currentNode = &nodes[key];

    if (currentNode->key == UINT_MAX) {
        currentNode->key = key;
        currentNode->bid = bid;
        return;
    }

    // Traverse to the end and add the new node
    while (currentNode->next != nullptr) {
        currentNode = currentNode->next;
    }
    currentNode->next = new Node(bid, key);
}

void HashTable::PrintAll() {
    for (unsigned int i = 0; i < tableSize; i++) {
        Node* currentNode = &nodes[i];
        if (currentNode->key != UINT_MAX) {
            while (currentNode != nullptr) {
                displayBid(currentNode->bid);
                currentNode = currentNode->next;
            }
        }
    }
}

void HashTable::Remove(string bidId) {
    unsigned int key = hash(stoi(bidId));
    Node* currentNode = &nodes[key];
    Node* previousNode = nullptr;

    while (currentNode != nullptr && currentNode->bid.bidId != bidId) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (currentNode != nullptr && currentNode->bid.bidId == bidId) {
        if (previousNode == nullptr) {
            // First node in chain
            if (currentNode->next != nullptr) {
                Node* temp = currentNode->next;
                *currentNode = *temp;
                delete temp;
            }
            else {
                currentNode->key = UINT_MAX;
                currentNode->bid = Bid();
            }
        }
        else {
            previousNode->next = currentNode->next;
            delete currentNode;
        }
    }
}

Bid HashTable::Search(string bidId) {
    Bid bid;
    unsigned int key = hash(stoi(bidId));
    Node* currentNode = &nodes[key];

    while (currentNode != nullptr) {
        if (currentNode->bid.bidId == bidId) {
            return currentNode->bid;
        }
        currentNode = currentNode->next;
    }

    return bid;
}

void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);
    vector<string> header = file.getHeader();

    try {
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            hashTable->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    clock_t ticks;
    HashTable* bidTable = new HashTable();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ticks = clock();
            loadBids(csvPath, bidTable);
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();
            bid = bidTable->Search(bidKey);
            ticks = clock() - ticks;
            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;
    delete bidTable;
    return 0;
}
