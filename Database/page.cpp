#include "page.h"
#include "../error.h"
#include <cstring>
using namespace std;

const unsigned PAGESIZE = 1000;
const unsigned PAGEDATASIZE =
    PAGESIZE - (sizeof(Slot) *
                2); // The actual amount of data available not including slots
const unsigned SIZELIMIT = 65; // Max size of passswords and wname

/** Constructor of a page
 *
 * Create a slot that contains the amount of the tuples
 * Create a free pointer to first available spot
 * Let the free space be the amount we can fit
 */
Page::Page()
{
    // Create the next slot for the amount of tuples in Page
    numTuplesSlot.prev = nullptr;
    numTuplesSlot.next = nullptr;
    numTuplesSlot.size = 0;

    // Initialize the curPage Pointer
    currSlot = &numTuplesSlot;
    freePtr = data;
    freeSpace = PAGEDATASIZE;
}

/** Check whether or not there is free space within the tuple
 * @return Status on if there is space or not
 */
const Status Page::isFreeSpace()
{
    if (freeSpace > 0)
        return OK;

    return NOSPACE;
}

/** Return the address of the free space
 * @param ptr: Holds the address of the free space
 * @return status: Status on if there is free space or not
 */
const Status Page::getFreeAddress(char **ptr)
{
    if (freePtr == nullptr)
        return BADPAGEPTR;

    if (isFreeSpace() != OK)
        return ENDOFPAGE;

    *ptr = freePtr;
    return OK;
}

/** Insert a tuple into the page
 * @param wname: The website name assoicated with the password
 * @param password: The password
 * @param tupelPtr: The pointer to the newly inserted tuple
 * @return status: Return the status if we have inserted tuple or not
 **/
const Status Page::insertTuple(const char *wname, const char *password,
                               Tuple *tuplePtr)
{
    // Ensure the inputs are valid
    if (!wname || !password)
        return BADINPUT;

    // We need ot check that the wname and password follow the restirctions of
    // 64 bytes
    if (strlen(wname) > SIZELIMIT || strlen(password) > SIZELIMIT)
        return SIZERROR;

    short wnameLength = strlen(wname);
    short passwordLength = strlen(password);
    // Size of Tuple (password & username) + Size of Slot
    short spaceNeeded = wnameLength + passwordLength + sizeof(Slot);

    if (spaceNeeded > freeSpace)
        return NOSPACE; // There is no space within the page
    else
    {
        Slot *newSlot = new Slot();
        Tuple *newTuple = new Tuple();
        short tupleSize = wnameLength + passwordLength;

        // Initialize the new slot
        currSlot->next = newSlot;
        newSlot->t_address = freePtr;
        newSlot->next = nullptr;
        newSlot->prev = currSlot;
        newSlot->size = wnameLength + passwordLength;
        currSlot = newSlot;

        // Initialize the new tuple
        newTuple->setWname(wname);
        newTuple->setPassword(password);
        newTuple->setSize(tupleSize);
        newTuple->setisEnd(spaceNeeded, freeSpace);

        tuplePtr = newTuple;

        // Copy the tuple into the block of memory
        memcpy(data + tupleSize, &newTuple, tupleSize);
        freePtr = (freePtr + tupleSize); // We find the next tuple
        freeSpace -= spaceNeeded;
    }

    // Later we need a method to insert into the B+ tree
    return OK;
}

/** Delete the tuple located within the slot
 * @param slotPtr: Pointer to the slot that contains the tuple
 * @return status: The status code of the deletion
 **/
const Status Page::deleteTuple(Slot *slotPtr)
{
    // Two situations: It's the last tuple meaning we don't need to compact

    bool isEnd = reinterpret_cast<Tuple *>(slotPtr->t_address)
                     ->getisEnd(); // Determine if it's the last tuple
    short sizeTuple = slotPtr->size;
    Slot *runPtr = slotPtr->next;
    Slot *prevPtr = slotPtr->prev;
    char *tempTupleAddress = slotPtr->t_address;

    slotPtr->size = -1; // Show that we need to delete
    if (isEnd)
    {
        // SImply delete the slot and tuple
        Slot *prevSlot = slotPtr->prev;
        delete slotPtr->t_address;
        slotPtr->t_address = nullptr;

        delete slotPtr;
        slotPtr = nullptr;

        prevSlot->next = nullptr;
        currSlot = prevSlot;
    }
    else
    {
        delete slotPtr->t_address; // Delete the tuple
        slotPtr->t_address = nullptr;
        prevPtr->next = runPtr; // Set the previous to next
        delete slotPtr;

        char *dest = tempTupleAddress; // Get the address

        while (runPtr != nullptr)
        {
            char *source = reinterpret_cast<char *>(runPtr->t_address);
            memmove(dest, source, runPtr->size);
            runPtr->t_address = dest;
            dest += runPtr->size;

            // Update the runner slot
            runPtr = runPtr->next;
        }
    }

    freePtr = reinterpret_cast<char *>(runPtr->size + runPtr);
    currSlot = runPtr;
    freeSpace += sizeTuple;
    numTuplesSlot.size--;

    return OK;
}
