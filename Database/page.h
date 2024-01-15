#ifndef PAGE_H
#define PAGE_H
// This is prevent defining multiple verisons of the header within the compiler
#include "../error.h"
#include <cstring>
#include <string>
using namespace std;

// This tuple will contain information about The passwords
// wname: The website name associated with the password
// password: The password associated with the website
struct Tuple
{
  private:
    char *wname;
    char *password;
    short size;
    bool isEnd; // Determine if we are at end of the entire page

  public:
    // Constructor with default values
    Tuple() : wname(nullptr), password(nullptr), size(0), isEnd(false) {}

    // Destructor
    ~Tuple()
    {
        delete[] wname;
        delete[] password;
    }

    char *getWname() { return wname; }
    char *getPassword() { return password; }
    short getSize() { return size; }
    short getisEnd() { return isEnd; }
    void setWname(const char *newName)
    {
        delete[] wname;                              // Free old name
        wname = new char[strlen(newName) + 1];       // Allocate new memory
        memcpy(wname, newName, strlen(newName) + 1); // Copy new name
        wname[strlen(newName)] = '\0';
    }

    void setPassword(const char *newPass)
    {
        delete[] password;                              // Free old password
        password = new char[strlen(newPass) + 1];       // Allocate new memory
        memcpy(password, newPass, strlen(newPass) + 1); // Copy new password
        password[strlen(newPass)] = '\0';
    }
    void setSize(short newSize) { size = newSize; }
    void setisEnd(short size, short freeSpace)
    {
        if (size == freeSpace)
            isEnd = true;
        else
            isEnd = false;
    }
};

// These are the slots inserted within a page to track each tuple and free spot
// t_address: The address of the tuple we are tracking
// size: The size of the tuple
struct Slot
{
    char *t_address;
    Slot *next;
    Slot *prev;
    short size;

  public:
    Slot() : t_address(nullptr), next(nullptr), prev(nullptr), size(0) {}
};

extern const unsigned PAGESIZE;
extern const unsigned PAGEDATASIZE;
extern const unsigned SIZELIMIT;

class Page
{
  private:
    Slot numTuplesSlot; // The amount of tuples within the pages
    Slot *currSlot;     // Pointer to the current Slot
    Slot *currTuple;    // Pointer to the current Tuple
    short freeSpace;    // The amount of space available in Page
    char *freePtr;      // Pointer to next free freeSpace
    char *data; // This is going to represent the block of memory for tuples

  public:
    Page();                     // Create a new Page
    const Status isFreeSpace(); // Return the amount of free space in page
    const Status
    getFreeAddress(char **ptr); // Return the first available free space address
    const Status insertTuple(const char *wname, const char *password,
                             Tuple *tuplePtr); // Insert the tuple in the page

    const Status deleteTuple(Slot *ptr);   // Delete the tuple in the page
    const Status modifyTuple(Tuple *data); // Modify the tuple
};
#endif
