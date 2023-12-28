#include "../Database/page.h"
#include "../error.h"
#include <gtest/gtest.h>

// Test for isFreeSpace Method
TEST(PageTest, IsFreeSpace)
{
    Page page;
    // Assuming a new page has the full amount of PAGEDATASIZE of free space
    EXPECT_EQ(page.isFreeSpace(), OK);
    // Consider adding more cases where you add data to the page and check if
    // the free space decreases accordingly
}

// Test for getFreeAddress Method
TEST(PageTest, GetFreeAddress)
{
    Page page;
    char *ptr = nullptr;
    // Assuming a new page has a valid free address
    EXPECT_EQ(page.getFreeAddress(&ptr), OK);
    EXPECT_NE(ptr, nullptr); // Ensure the pointer is not null after getting the
                             // free address
    // You can extend by filling the page and then ensuring it returns the
    // correct status or pointer
}

// Test for insertTuple Method
TEST(PageTest, InsertTuple)
{
    Page page;
    const char *wname = "example.com";
    const char *password = "password123";
    Tuple *tupelPtr = nullptr; // Initialize as necessary for your use case

    Status status = page.insertTuple(wname, password, tupelPtr);
    EXPECT_EQ(status, OK); // Assuming the initial insert should be successful

    // Add more tests to cover the full range of functionality:
    // - Inserting until the page is full
    // - Inserting with oversized wname or password
}

/*
// Test for deleteTuple Method
TEST(PageTest, DeleteTuple) {
    Page page;
    // Assuming you have a way to add a tuple to the page first, then get a
pointer to its slot Slot *slotPtr = nullptr; // Initialize to point to an actual
slot on the page

    Status status = page.deleteTuple(slotPtr);
    EXPECT_EQ(status, OK);

    // Consider testing for:
    // - Deleting the last tuple on the page
    // - Deleting when there are multiple tuples
    // - Deleting from an empty page or a null slot pointer
}
*/
