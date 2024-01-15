#include "BPlusTree.h"
#include "page.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
using namespace std;

/**
 * What to add to this code?
 *
 * Create a deconstructor along with delete statements
 * Check for any memory leaks pleaseeeee
 * Implement a search_range function
 *
 *
 */
BPlusTree::BPlusTree(short size)
{
    setBucketSize(sizeof(Node *), size);
    FIXEDSIZE = size;
    this->root = nullptr;
}

/**
 * 1) If the this->root is null, then simply modify it and return
 * 2) We are going to go down into the leaves until we find the tuple we are
 * looking for/ location to place
 * 3) Once we find the Node, we need to determine if we have space to insert
 *
 *
 */
const Status BPlusTree::insertNode(char *wname, Slot *tuple)
{
    long bucketSize = getBucketSize();
    Status status;
    Node *curr = this->root;
    Node *parent;
    char *keyString;
    NodeEntry *key; // Get the array of keys
    Node **childPtr;

    if (this->root == nullptr)
    {
        this->root = new Node(bucketSize);
        status = this->root->setKey(wname, 0, bucketSize);
        if (status != OK)
            return status;
        status = this->root->setTuple(tuple, 0);
        if (status != OK)
            return status;
        this->root->setIsLeaf(true);
        this->root->setSize(1);
    }
    else
    {
        while (curr->isLeaf() == false)
        {
            parent = curr; // Get the parent
            for (int i = 0; i < curr->getSize(); i++)
            {
                keyString = curr->getKey()[i].key();
                key = curr->getKey();
                childPtr = curr->getChildPtr();

                // The wname is less than keyString
                // This means we can go to the children for traveral
                if (strcmp(wname, keyString) < 0)
                {
                    curr = childPtr[i];
                    break;
                }

                // The wname is bigger than all the values
                // This means we can go to the children on the last one
                if (i == curr->getSize() - 1)
                {
                    curr = childPtr[i + 1];
                    break;
                }
            }
        }
    }

    // At this point, we are at a leaf and encountered the Node to insertNode

    // We have space to insert into the Node
    if (curr->getSize() < bucketSize)
    {
        int i = 0;

        // Traverse node to find where it needs to be located
        while (strcmp(wname, keyString) > 0 && i < curr->getSize())
            i++;

        // Shift everything over to make space for the tuple
        for (int j = curr->getSize(); j > i; j--)
            key[j] = key[j - 1];

        status = curr->setKey(wname, i, bucketSize); // Insert the tuple
        if (status != OK)
            return status;
        status = curr->setTuple(tuple, i);
        if (status != OK)
            return status;

        curr->setSize(curr->getSize() + 1);
        childPtr[curr->getSize()] =
            childPtr[curr->getSize() -
                     1]; // Shift the pointer to the child to left
        childPtr[curr->getSize() - 1] = NULL;
    }
    else
    {
        // We need to shift everything up!
        Node *newLeaf = new Node(bucketSize);
        Node **leafChildren = newLeaf->getChildPtr();
        NodeEntry tempNode[bucketSize + 1];

        // Store all values into the Node
        for (int i = 0; i < bucketSize; i++)
            tempNode[i] = key[i];

        int i = 0, j;

        // Find the position to insert Node
        while (strcmp(wname, tempNode[i].key()) > 0 && i < bucketSize)
            i++;

        for (int j = bucketSize + 1; j > i; j--)
            tempNode[j] = tempNode[j - 1];
        tempNode[i] = wname; // Insert the elemtn in right position

        newLeaf->setIsLeaf(true);
        // We are ensuring that each node is at least half full
        curr->setSize(floor((bucketSize + 1) / 2));
        newLeaf->setSize(floor((bucketSize + 1) - (bucketSize + 1) / 2));

        // Now we will push everything up a level
        childPtr[curr->getSize()] = newLeaf;
        leafChildren[newLeaf->getSize()] = childPtr[bucketSize];
        childPtr[newLeaf->getSize()] = childPtr[bucketSize];
        childPtr[bucketSize] = NULL;

        for (int i = 0; i < curr->getSize(); i++)
            key[i] = tempNode[i];

        NodeEntry *leafKey = newLeaf->getKey();
        j = curr->getSize();
        for (int i = 0; i < newLeaf->getSize(); i++)
        {
            leafKey[i] = tempNode[j].key();
            j++;
        }

        // Handles the case of spliting a this->root Node
        if (curr == this->root)
        {
            Node *newRoot = new Node(bucketSize);
            Node **newChildPtr = newRoot->getChildPtr();
            NodeEntry *key = newRoot->getKey();

            key[0] = leafKey[0];
            newChildPtr[0] = curr;
            newChildPtr[1] = newLeaf;
            newRoot->setIsLeaf(false);
            newRoot->setSize(1);
            this->root = newRoot;
        }
        else
        {
            // If we are splitting an internal Node, then we need to keep
            // spliting until we get to the this->root
            status = recursiveShift(leafKey[0].key(), tuple, parent, newLeaf);
            if (status != OK)
                return status;
        }
    }
    return OK;
}
const Status BPlusTree::recursiveShift(char *wname, Slot *tuple, Node *curr,
                                       Node *child)
{
    long bucketSize = getBucketSize();
    Status status;
    Node *parent;
    char *keyString;
    NodeEntry *currKey = curr->getKey(); // Get the array of keys
    Node **childPtr = curr->getChildPtr();

    // We can fit the password on this level!
    if (curr->getSize() < bucketSize)
    {
        int i = 0;

        while (strcmp(wname, currKey[i].key()) > 0 && i < curr->getSize())
            i++;
        for (int j = curr->getSize(); j > i; j--)
            currKey[j] = currKey[j - 1];
        for (int j = curr->getSize() + 1; j > i + 1; j--)
            childPtr[j] = childPtr[j - 1];

        status = curr->setKey(wname, i, bucketSize); // Insert the tuple
        if (status != OK)
            return status;
        status = curr->setTuple(tuple, i);
        if (status != OK)
            return status;
        curr->setSize(curr->getSize() + 1);
        childPtr[i + 1] = child;
    }
    else
    {
        Node *newInternal = new Node(bucketSize);
        NodeEntry *internalKey = newInternal->getKey();
        Node **internalChild = newInternal->getChildPtr();
        NodeEntry tempKey[bucketSize + 1];
        Node *tempPtr[bucketSize + 2];

        for (int i = 0; i < bucketSize; i++)
            tempKey[i] = currKey[i];

        for (int i = 0; i < bucketSize + 1; i++)
            tempPtr[i] = childPtr[i];

        int i = 0, j;
        while (strcmp(wname, tempKey[i].key()) > 0 && i < bucketSize)
            i++;

        for (int j = bucketSize + 1; j > i; j--)
            tempKey[j] = tempKey[j - 1];

        tempKey[i] = wname;
        for (int j = bucketSize + 2; j > i + 1; j--)
            tempPtr[j] = tempPtr[j - 1];

        tempPtr[i + 1] = child;
        newInternal->setIsLeaf(false);

        // We are ensuring that each node is at least half full
        curr->setSize(ceil((bucketSize + 1) / 2));
        newInternal->setSize(ceil(bucketSize - (bucketSize + 1) / 2));

        j = curr->getSize() + 1;
        for (int i = 0; i < newInternal->getSize(); i++)
        {
            internalKey[i] = tempKey[j];
            j++;
        }

        j = curr->getSize() + 1;
        for (int i = 0; i < newInternal->getSize() + 1; i++, j++)
        {
            internalKey[i] = tempKey[j];
            j++;
        }

        if (curr == this->root)
        {
            Node *newRoot = new Node(bucketSize);
            Node **newChildPtr = newRoot->getChildPtr();
            NodeEntry *key = newRoot->getKey();

            key[0] = currKey[curr->getSize()];
            newChildPtr[0] = curr;
            newChildPtr[1] = newInternal;
            newRoot->setIsLeaf(false);
            newRoot->setSize(1);
            this->root = newRoot;
        }
        else
        {
            status = recursiveShift(currKey[curr->getSize()].key(), tuple,
                                    findParent(this->root, curr), newInternal);
            if (status != OK)
                return status;
        }
    }
    return status;
}

Node *BPlusTree::findParent(Node *root, Node *curr)
{
    Node *parent;
    Node **newChildPtr = this->root->getChildPtr();

    if (this->root->isLeaf() || (newChildPtr[0])->isLeaf())
        return NULL;

    for (int i = 0; i < this->root->getSize() + 1; i++)
    {
        if (newChildPtr[i] == curr)
        {
            parent = this->root;
            return parent;
        }
        else
        {
            parent = findParent(newChildPtr[i], curr);
            if (parent != NULL)
                return parent;
        }
    }
    return parent;
}
const Status BPlusTree::findSibilings(Node *curr, Node *&leftSibling,
                                      Node *&rightSibling)
{
    Node *parent = findParent(this->root, curr);
    if (parent == NULL)
    {
        leftSibling = nullptr;
        rightSibling = nullptr;
        return NOSIBLINGS;
    }

    Node **parentsChildren = parent->getChildPtr();
    int index = -1;

    for (int i = 0; i < parent->getSize(); i++)
    {
        if (parentsChildren[i] == curr)
        {
            index = i;
            break;
        }
    }

    // Could not find the child in parent
    if (index == -1)
    {
        leftSibling = nullptr;
        rightSibling = nullptr;
        return NOSIBLINGS;
    }
    // Set left sibling (if it exists)
    leftSibling = (index > 0) ? parent->getChildPtr()[index - 1] : nullptr;

    // Set right sibling (if it exists)
    rightSibling = (index < parent->getSize() - 1 && index > 0)
                       ? parent->getChildPtr()[index + 1]
                       : nullptr;

    if (leftSibling != nullptr && rightSibling != nullptr)
        return NOSIBLINGS;

    return OK;
}

const Status BPlusTree::searchTuple(char *wname, Node *foundNode,
                                    Slot *tupleAddress)
{
    Status status;
    Node *curr = this->root;
    Node *parent;
    char *keyString;
    NodeEntry *key; // Get the array of keys
    Node **childPtr;

    if (this->root == nullptr)
        return NOTFOUND;
    else
    {
        while (curr->isLeaf() == false)
        {

            for (int i = 0; i < curr->getSize(); i++)
            {
                key = curr->getKey();
                keyString = key[i].key();
                childPtr = curr->getChildPtr();

                // The wname is less than keyString
                // This means we can go to the children for traveral
                if (strcmp(wname, keyString) < 0)
                {
                    curr = childPtr[i];
                    break;
                }

                // The wname is bigger than all the values
                // This means we can go to the children on the last one
                if (i == curr->getSize() - 1)
                {
                    curr = childPtr[i + 1];
                    break;
                }
            }
        }
    }

    for (int i = 0; i < curr->getSize(); i++)
    {
        // We found the tuple
        if (strcmp(wname, key[i].key()) == 0)
        {
            tupleAddress = key[i].slot();
            foundNode = curr;
            return OK;
        }
    }

    return NOTFOUND;
}

const Status BPlusTree::redistribute(Node *node, Node *sibling, Node *parent,
                                     bool isLeft)
{

    if (node == nullptr || parent == nullptr || sibling == nullptr)
        return BADINPUT;

    Status status;
    NodeEntry *nodeKeys = node->getKey();
    NodeEntry *siblingKey = sibling->getKey();
    NodeEntry *parentKeys = parent->getKey();
    Node **nodeChildren = node->getChildPtr();
    Node **siblingChildren = sibling->getChildPtr();
    int nodeIndex = -1;

    // We need to find the index of the parent's children of node
    Node **children = parent->getChildPtr();
    for (int i = 0; i < parent->getSize() + 1; ++i)
    {
        if (children[i] == node)
        {
            nodeIndex = i;
            break;
        }
    }

    if (nodeIndex == -1)
        return INDEXNOTFOUND;

    if (isLeft)
    {
        for (int i = node->getSize(); i > 0; i--)
        {
            nodeKeys[i] = nodeKeys[i - 1];
        }
        nodeKeys[0] = siblingKey[sibling->getSize() - 1];
        siblingKey[sibling->getSize() - 1] = NULL;
        sibling->setSize(sibling->getSize() - 1);

        parentKeys[nodeIndex] = nodeKeys[0];

        // Handle distribution of internal nodes
        if (!node->isLeaf())
        {
            Node *donorChild = siblingChildren[sibling->getSize()];

            for (int i = node->getSize(); i > 0; i--)
            {
                nodeChildren[i] = nodeChildren[i - 1];
            }
            nodeChildren[0] = donorChild;
            siblingChildren[sibling->getSize() - 1] = NULL;
        }
    }
    else
    {
        nodeKeys[node->getSize() - 1] = siblingKey[0];

        for (int i = 0; i < node->getSize() - 1; i++)
        {
            siblingKey[i] = siblingKey[i + 1];
        }
        siblingKey[sibling->getSize() - 1] = NULL;
        sibling->setSize(sibling->getSize() - 1);
        parentKeys[nodeIndex] = siblingKey[0];

        if (!node->isLeaf())
        {
            Node *donorChild = siblingChildren[0];

            for (int i = 0; i < node->getSize() - 1; i++)
            {
                nodeChildren[i] = nodeChildren[i + 1];
            }
            nodeChildren[node->getSize() - 1] = donorChild;
        }
    }
    return status;
}

const Status BPlusTree::mergeNodes(Node *node, Node *sibling, Node *parent,
                                   bool isLeft)
{
    if (node == nullptr || sibling == nullptr || parent == nullptr)
    {
        return NULLPTR;
    }

    Status status;
    NodeEntry *nodeKeys = node->getKey();
    NodeEntry *siblingKey = sibling->getKey();
    NodeEntry *parentKeys = parent->getKey();
    Node **nodeChildren = node->getChildPtr();
    Node **siblingChildren = sibling->getChildPtr();
    int nodeIndex = -1;

    Node **children = parent->getChildPtr();
    for (int i = 0; i < parent->getSize() + 1; ++i)
    {
        if (children[i] == node)
        {
            nodeIndex = i;
            break;
        }
    }

    if (nodeIndex == -1)
        return INDEXNOTFOUND;

    if (isLeft)
    {
        siblingKey[sibling->getSize()] = parentKeys[nodeIndex];
        sibling->setSize(sibling->getSize() + 1);

        for (int i = 0; i < node->getSize(); ++i)
        {
            siblingKey[sibling->getSize()] = nodeKeys[i];
            sibling->setSize(sibling->getSize() + 1);
        }

        // Handle internal nodes
        if (!node->isLeaf())
        {
            int siblingSize = sibling->getSize();
            for (int i = 0; i < node->getSize() + 1; ++i)
            {
                siblingChildren[siblingSize + i + 1] = nodeChildren[i];
            }
            sibling->setSize(siblingSize +
                             node->getSize()); // Update size for sibling
        }

        // Remove the the node from the parent
        int parentIndex = -1;
        for (int i = 0; i < parent->getSize(); ++i)
        {
            if (children[i] == node)
            {
                parentIndex = i;
                break;
            }
        }

        if (parentIndex == -1)
        {
            return INDEXNOTFOUND;
        }

        for (int i = parentIndex; i < parent->getSize() - 1; ++i)
        {
            children[i] = children[i + 1];
        }

        children[parent->getSize() - 1] = nullptr;
        parent->setSize(parent->getSize() - 1);
        delete node;
    }
    else
    {
        nodeKeys[node->getSize()] = parentKeys[nodeIndex];
        node->setSize(node->getSize() + 1);
        for (int i = 0; i < sibling->getSize(); ++i)
        {
            nodeKeys[node->getSize()] = siblingKey[i];
            node->setSize(node->getSize() + 1);
        }

        // Handle children for internal nodes
        if (!node->isLeaf())
        {
            int nodeSize = node->getSize();
            for (int i = 0; i < sibling->getSize() + 1; ++i)
            {
                nodeChildren[nodeSize + i + 1] = siblingChildren[i];
            }
            node->setSize(nodeSize + sibling->getSize());
        }

        // Remove the the node from the parent
        int parentIndex = -1;
        for (int i = 0; i < parent->getSize(); ++i)
        {
            if (children[i] == sibling)
            {
                parentIndex = i;
                break;
            }
        }

        if (parentIndex == -1)
        {
            return INDEXNOTFOUND;
        }

        for (int i = parentIndex; i < parent->getSize() - 1; ++i)
        {
            children[i] = children[i + 1];
        }

        children[parent->getSize() - 1] = nullptr;
        parent->setSize(parent->getSize() - 1);
        delete sibling;
    }
    return status;
}
/**
 * For each internal node, they must have size >= [bucketSize/2]
 * For the this->root, it just needs to be size of 1
 *
 *
 */
const Status BPlusTree::handleUnderflow(Node *node)
{
    if (node == nullptr || node == this->root && node->getSize() >= 1)
    {
        return OK;
    }

    Status status;
    size_t minKeys = ceil(getBucketSize() / 2);

    Node *parent = findParent(this->root, node);
    Node *leftSibling = nullptr, *rightSibling = nullptr;
    status = findSibilings(node, leftSibling, rightSibling);

    if (status != OK)
        return status;

    // Try Redistribution First
    if (leftSibling && leftSibling->getSize() >= minKeys)
    {
        redistribute(node, leftSibling, parent, true);
    }
    else if (rightSibling && rightSibling->getSize() >= minKeys)
    {
        redistribute(node, rightSibling, parent, false);
    }
    else
    {
        // No sibling can spare a key, try merging
        Status status =
            mergeNodes(node, leftSibling ? leftSibling : rightSibling, parent,
                       leftSibling ? true : false);
        if (parent && parent->getSize() < minKeys)
        {
            handleUnderflow(parent); // Handle underflow recursively up the tree
        }
    }

    return OK;
}

const Status BPlusTree::deleteNode(char *wname)
{
    Status status;
    Node *foundNode;
    Slot *foundSlot;

    if (this->root == nullptr)
    {
        return NOTFOUND; // Tree is empty
    }

    // Step 1: Locate the key
    status = searchTuple(wname, foundNode, foundSlot);
    if (status != OK)
        return status;

    NodeEntry *key = foundNode->getKey();
    bool found = false;

    // Step 2: Actually delete the NodeEntry
    for (int i = 0; i < foundNode->getSize(); i++)
    {
        if (!found && strcmp(wname, key[i].key()) == 0)
        {
            found = true;
            foundSlot = key[i].slot();
        }

        if (found == true && i < foundNode->getSize() - 1)
        {
            key[i] = key[i + 1];
        }
    }

    key[getBucketSize() - 1] = NULL; // Ensure the last element is nothing

    if (!found)
        return NOTFOUND;
    else
        foundNode->setSize(foundNode->getSize() - 1);

    // Step 3: Adjust the tree if needed
    if (foundNode->getSize() < ceil(getBucketSize() / 2))
    {
    }
    return OK;
}

void BPlusTree::printTree()
{
    if (this->root == nullptr)
    {
        cout << "The tree is empty." << endl;
        return;
    }

    queue<std::pair<Node *, std::string>> q;
    q.push(make_pair(this->root, "")); // this->root doesn't have a branch yet

    while (!q.empty())
    {
        pair<Node *, std::string> front = q.front();
        Node *node = front.first;
        string branch = front.second;
        q.pop();

        // Print the branch and node's keys
        cout << branch << "[";
        for (int i = 0; i < node->getSize(); ++i)
        {
            if (i > 0)
                cout << ", ";
            cout
                << node->getKey()[i].key(); // Assuming key is a string or char*
        }
        std::cout << "]" << endl;

        // If the node is not a leaf, enqueue its children with updated branches
        if (!node->isLeaf())
        {
            for (int i = 0; i <= node->getSize(); ++i)
            {
                string newBranch = branch + (i == 0 ? " \\" : " /");
                q.push(make_pair(node->getChildPtr()[i], newBranch));
            }
        }
    }
}
