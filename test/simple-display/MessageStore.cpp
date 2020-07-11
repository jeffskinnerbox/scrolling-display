
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    This object calls implements two data circular queue, aka ring buffer, for holding the messages that will appear on the scrolling display.
    is a linear data structure in which the operations are performed based on FIFO (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called "Ring Buffer".

REFERENCE MATERIALS:
    * https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
    * https://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#pragma once                 // compiler to skip subsequent includes of this file

// ESP8266 libraries (~/.arduino15/packages/esp8266)

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"
#include "MessageStore.h"

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)


#define QUEUE_SIZE  5         // defult size of message queue
#define STORE_SIZE  5         // defult size of message store
#define BUF_SIZE    80        // defult max number of characters of a message



// ------------------------ Constructors & Destructors -------------------------


// Constructor to create MessageStore
MessageStore::MessageStore(void) {

    int rows = STORE_SIZE + QUEUE_SIZE;
    int cols = BUF_SIZE + 1;               // adding 1 to include null character at end of string

    msg_size = BUF_SIZE;
    store_size = STORE_SIZE;
    store_top = 0;           // simple store indexes from 0 to store_size

    queue_size = QUEUE_SIZE;
    queue_top = STORE_SIZE;           // queue indexes from queue_top to store_size + queue_size
    queue_front = queue_rear = -1;

    // create array used to store and queue messages
    array = new char*[rows];
    if (rows) {
        array[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            array[i] = array[0] + i * cols;
    }

}


// Destructor to delete MessageStore
MessageStore::~MessageStore(void) {

    int rows = store_size + queue_size;

    // delete array used to store and queue messages
    if (rows) delete [] array[0];
    delete [] array;

}



// ------------------------ Methods for Circular Queue -------------------------

// Function to clear the contents of the circular queue
void MessageStore::clearQueue(void) {
    for (int i = queue_top; i < queue_top + queue_size; i++)
        array[i][0] = '\0';

    queue_front = queue_rear = -1;
}


// return index of next empty message in queue, return -1 if queue is full
int MessageStore::indexQueue() {

    for (int i = queue_top; i < queue_top + queue_size; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return -1;
}


// Function prints the elements of Circular Queue
void MessageStore::printQueue(void) {

    int cnt;

    // count non-null elements
    cnt = 0;
    for (int i = queue_top; i < queue_top + queue_size; i++)
        if (array[i][0] != '\0')
                cnt++;

    // print controlling parameters
    INFOD("Number of elements in Queue are: ", cnt);
    INFOD("\tqueue_top = ", queue_top);
    INFOD("\tqueue_size = ", queue_size);
    INFOD("\tqueue_front = ", queue_front);
    INFOD("\tqueue_rear = ", queue_rear);

    // print the queue contents
    for (int i = queue_top; i < queue_top + queue_size; i++)
        if (array[i][0] != '\0')
            INFOS("\t", array[i]);
}


// Function to add element to Circular Queue
bool MessageStore::addQueue(char *str) {

    // check size of message
    if (int size = strlen(str) > msg_size) {
        ERRORD("Attempting to add message longer than message buffer. size = ", size);
        INFOD("Message buffer size: msg_size = ", msg_size);
        return false;
    }

    if ((queue_front == queue_top && queue_rear == queue_size - 1) || (queue_rear == (queue_front - 1)%(queue_size - 1))) {
        INFO("Circular Queue is full. Removing element from end of message queue.\n\r");
    } else if (queue_front == -1) {     // (room in queue) queue is empty, so insert into first element
        queue_front = queue_rear = queue_top;
        sprintf(array[queue_front], str);
    } else if (queue_rear == queue_size - 1 && queue_front != queue_top) {   // (room in queue) rear is at bottom but front is > 0
        queue_rear = queue_top;
        sprintf(array[queue_rear], str);
    } else {
        queue_rear++;
        sprintf(array[queue_rear], str);
    }

    return true;

}


// return the number of messages in queue
int MessageStore::countQueue() {

    int cnt = 0;

    for (int i = queue_top; i < queue_top + queue_size; i++)
        if (array[i][0] != '\0') {
            cnt++;
        }

    return cnt;
}


int MessageStore::sizeQueue() {

    return queue_size;
}



// -------------------------- Methods for Simple Store -------------------------

// Function to clear the contents of the simple store
void MessageStore::clearStore(void) {

    for (int i = store_top; i < store_top + store_size; i++)
        array[i][0] = '\0';

}


// return index of next empty message in store, return -1 if simple store is full
int MessageStore::indexStore() {

    for (int i = store_top; i < store_top + store_size; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return -1;
}


// Function prints the elements of simple store
void MessageStore::printStore(void) {
    int i, cnt;

    //count non-null elements
    cnt = 0;
    for (i = store_top; i < store_top + store_size; i++)
        if (array[i][0] != '\0')
                cnt++;

    // print headings
    INFOD("Number of elements in Simple Store are: ", cnt);
    INFOD("\tstore_top = ", store_top);
    INFOD("\tstore_size = ", store_size);

    // print the simple store contents
    for (int i = store_top; i < store_top + store_size; i++)
        if (array[i][0] != '\0')
            INFOS("\t", array[i]);
}


// Function to add element anywhere into simple store
bool MessageStore::addStore(char *str) {

    int index = indexStore();

    // check size of message
    if (int size = strlen(str) > msg_size) {
        ERRORD("Attempting to add message longer than message buffer. size = ", size);
        INFOD("Message buffer size: msg_size = ", msg_size);
        return false;
    }

    if (index < store_top || index > store_top + store_size - 1) {
        ERRORD("Failed to add message to Store. Bad index.  index = ", index);
        return false;
    } else if (index < 0) {
        INFO("Failed to add message to Store. Store is full.\n\r");
        return false;
    } else {
        INFO("Successfully adding message to Store.\n\r");
        sprintf(array[index], str);
    }

    //EXEC(printStore())
    return true;
}


// Function to add element to specific index of simple store
bool MessageStore::addStore(char *str, int index) {

    // check size of message
    if (int size = strlen(str) > msg_size) {
        ERRORD("Attempting to add message longer than message buffer. size = ", size);
        INFOD("Message buffer size: msg_size = ", msg_size);
        return false;
    }

    if (index < store_top || index > store_top + store_size - 1) {
        ERRORD("Failed to add message to Store. Bad index.  index = ", index);
        return false;
    } else {
        INFO("Successfully adding message to Store.\n\r");
        sprintf(array[index], str);
    }

    //EXEC(printStore())
    return true;

}


// Function to delete element from simple store
bool MessageStore::deleteStore(int index) {

    if (index < store_top || index > store_top + store_size) {
        ERRORD("Failed to delete message from Store. Bad index. index = ", index);
        return false;
    } else {
        INFO("Successfully deleted message from Store.\n\r");
        array[index][0] = '\0';
        return true;
    }
}



// return the number of messages in simple store
int MessageStore::countStore() {

    int cnt = 0;

    for (int i = store_top; i < store_top + store_size; i++)
        if (array[i][0] != '\0') {
            cnt++;
        }

    return cnt;
}


char *MessageStore::getStore(int index) {

    if (index < store_top || index >= store_top + store_size) {
        ERRORD("Failed to get message from Store. Bad index. index = ", index);
        return NULL;
    }

    return array[index];
}


int MessageStore::sizeStore() {

    return store_size;
}



// --------------------- Methods for Simple Store and Queue --------------------

int MessageStore::size(void) {
    return store_size + queue_size;
}


void MessageStore::clear(void) {
    for (int i = store_top; i < store_size + queue_size; i++)
        array[i][0] = '\0';

    queue_front = queue_rear = -1;
}


void MessageStore::print(void) {

    int cnt;

    // count non-null elements
    cnt = 0;
    for (int i = store_top; i < store_size + queue_size; i++)
        if (array[i][0] != '\0')
                cnt++;

    // print controlling parameters
    INFOD("Number of elements in MessageStore are: ", cnt);
    INFOD("\tstore_top = ", store_top);
    INFOD("\tstore_size = ", store_size);
    INFOD("\tqueue_top = ", queue_top);
    INFOD("\tqueue_size = ", queue_size);
    INFOD("\tqueue_front = ", queue_front);
    INFOD("\tqueue_rear = ", queue_rear);

    // print the queue contents
    for (int i = store_top; i < store_size + queue_size; i++)
        if (array[i][0] != '\0')
            INFOS("\t", array[i]);
}

