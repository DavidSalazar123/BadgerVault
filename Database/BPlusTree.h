#include "page.h"
#ifndef BPlusTree_H
#define BPlusTree_H
using namespace std;

// Node Entries of Slot and name
struct NodeEntry
{
    pair<char *, Slot *> entry;

    NodeEntry(char *key = nullptr, Slot *slot = nullptr)
        : entry(make_pair(key, slot))
    {
    }

    // Ensure that all NodeEntry types are properly deleted
    ~NodeEntry()
    {
        delete entry.first;
        delete entry.second;
    }

    // Convenient accessors
    char *&key() { return entry.first; }
    Slot *&slot() { return entry.second; }
};

// Tree nodes
struct Node
{
  private:
    bool is_leaf;
    long size;      // The amount stored within the nodes
    NodeEntry *key; // Array of entries
    Node **childPtr;

  public:
    Node(long bucket) : is_leaf(false), size(0), key(nullptr), childPtr(nullptr)
    {
        key = new NodeEntry[bucket];
        childPtr = new Node *[bucket + 1];
    }

    ~Node()
    {
        delete[] key;
        for (long i = 0; i <= size; ++i)
        {
            delete childPtr[i];
        }
        delete[] childPtr;
    }

    // Getters
    bool isLeaf() const { return is_leaf; }
    NodeEntry *getKey() const { return key; }
    Node **getChildPtr() const { return childPtr; }
    long getSize() const { return size; }

    // Setters
    void setIsLeaf(bool val) { is_leaf = val; }
    void setSize(long val) { size = val; }
    const Status setKey(char *newName, short index, short bucket)
    {
        if (index < 0 || index >= bucket)
        {
            return BADINDEXPARM; // Invalid index
        }

        // Calculate the length of the new name
        size_t newNameLength = strlen(newName);

        // Free the old key if it exists
        if (key[index].key() != nullptr)
        {
            delete[] key[index].key(); // Free existing key memory
        }
        key[index].key() = new char[newNameLength + 1];

        // Copy the new name into the key at the specified index
        memcpy(key[index].key(), newName, strlen(newName) + 1);
        key[index].key()[strlen(key[index].key())] = '\0';

        return OK;
    }

    const Status setTuple(Slot *newTuple, short index)
    {
        if (index < 0 || index >= size)
        {
            return BADINDEXPARM; // Return an error status if index is out of
                                 // bounds
        }

        key[index].slot() = newTuple;

        return OK;
    }
};

class BPlusTree
{
  private:
    Node *root;      // Root of the tree
    long FIXEDSIZE;  // The fixed size of a B+ tree
    long BUCKETSIZE; // The size for each node
  public:
    BPlusTree(short size);

    // Setters
    void setBucketSize(int p, int x, int k = 4)
    {
        BUCKETSIZE = floor(static_cast<double>(x + k) / (k + p));
    }
    void setFixedSize(long size) { FIXEDSIZE = size; }

    // Getters
    long getBucketSize() { return BUCKETSIZE; }
    long getFixedSize() { return FIXEDSIZE; }
    const Status insertNode(char *wname, Slot *tuple);
    const Status recursiveShift(char *wname, Slot *tuple, Node *curr,
                                Node *child);
    void printTree();
    Node *findParent(Node *root, Node *curr);
    const Status deleteNode(char *wname);
    const Status redistribute(Node *node, Node *sibling, Node *parent,
                              bool isLeft);
    const Status handleUnderflow(Node *node);
    const Status mergeNodes(Node *node, Node *sibling, Node *parent,
                            bool isLeft);
    const Status searchTuple(char *wname, Node *foundNode, Slot *tupleAddress);
    const Status findSibilings(Node *curr, Node *&leftSibling,
                               Node *&rightSibling);
};

#endif
