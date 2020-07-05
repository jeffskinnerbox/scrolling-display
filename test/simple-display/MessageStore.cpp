
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0

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

#include <Arduino.h>

#define QUEUE_SIZE  5         // number of messages stored in a queue
#define STORE_SIZE  5         // number of messages stored in a queue
#define BUF_SIZE    512       // max number of characters in a message

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "debug.h"


class MessageStore {
  private:
    // Circular Queue
    int cir_size;                                   // size of the circular queue
    int cir_rear, cir_front;                        // indexes to front and rear of the circular queue
    //char cir_array[QUEUE_SIZE][BUF_SIZE];
    char **cir_array = NULL;

    // Simple Store
    char array[STORE_SIZE][BUF_SIZE];
    int indexStore(void);
    int countStore(void);

  public:
    // constructors & destructors for the class
    MessageStore(void);
    ~MessageStore(void);
    //MessageStore(int queue_size, int store_size, int buf_size);

    // Circular Queue
    void clearQueue();
    void printQueue();
    void addQueue(char *);
    char *getQueue(int);

    // Simple Store
    void clearStore();
    void printStore();
    bool addStore(char *);
    bool addStore(char *, int);
    char *getStore(int);
    bool deleteStore(int);
};



// ------------------------ Constructors & Destructors -------------------------

/*
// Constructor to create Circular queue
MessageStore::MessageStore(void) {

    cir_front = cir_rear = -1;
    cir_size = 0;

    cir_array = new char*[QUEUE_SIZE];
    for (int i = 0; i < QUEUE_SIZE; i++)
        cir_array[i] = new char[BUF_SIZE];

}


// Destructor to create Circular queue
MessageStore::~MessageStore(void) {

    for (int i = 0; i < QUEUE_SIZE; i++)
        delete [] cir_array[i];
    delete [] cir_array;
}
*/


// Constructor to create Circular queue
MessageStore::MessageStore(void) {

    int rows = QUEUE_SIZE;
    int cols = BUF_SIZE;

    cir_front = cir_rear = -1;
    cir_size = 0;

    char** cir_array = new char*[rows];
    if (rows) {
        cir_array[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            cir_array[i] = cir_array[0] + i * cols;
    }

}


// Destructor to create Circular queue
MessageStore::~MessageStore(void) {

    int rows = QUEUE_SIZE;
    int cols = BUF_SIZE;

    if (rows) delete [] cir_array[0];
    delete [] cir_array;

}


// ------------------------ Methods for Circular Queue -------------------------

// Function to clear the contents of the circular queue
void MessageStore::clearQueue(void) {
    for (int i = 0; i < QUEUE_SIZE; i++)
        cir_array[i][0] = '\0';

    cir_size = 0;
    cir_front = cir_rear = -1;
}


// Function prints the elements of Circular Queue
void MessageStore::printQueue(void) {

    // print headings
    if (cir_front == -1) {
        PRINTD("No elements in Circular Queue: ", cir_size);
    } else
        PRINTD("Elements in Circular Queue are: ", cir_size);

    // print controlling parameters
    PRINTD("\tcir_size = ", cir_size);
    PRINTD("\tcir_front = ", cir_front);
    PRINTD("\tcir_rear = ", cir_rear);

    // print the circular queue contents
    if (cir_front == -1)                               // circular queue is empty
        ;
    else if (cir_rear >= cir_front) {                  // circular queue doesn't loop
        for (int i = cir_front; i <= cir_rear; i++)
            PRINTS("\t",cir_array[i]);
    } else {                                           // circular queue loop around
        for (int i = cir_front; i < cir_size; i++)
            PRINTS("\t", cir_array[i]);
        for (int i = 0; i <= cir_rear; i++)
            PRINTS("\t", cir_array[i]);
    }
}


// Function to add element to Circular Queue
void MessageStore::addQueue(char *value) {

    if ((cir_front == 0 && cir_rear == cir_size-1) || (cir_rear == (cir_front-1)%(cir_size-1))) {
        PRINT("Circular Queue is full. Removing element from end of message queue.\n\r");
        return;
    } else if (cir_front == -1) {     // (room in queue) queue is empty, so insert into first element
        cir_front = cir_rear = 0;
        sprintf(cir_array[cir_front], value);
    } else if (cir_rear == cir_size-1 && cir_front != 0) {   // (room in queue) rear is at bottom but front is > 0
        cir_rear = 0;
        sprintf(cir_array[cir_rear], value);
    } else {
        cir_rear++;
        sprintf(cir_array[cir_rear], value);
    }

}


char *MessageStore::getQueue(int index) {

    if (index < 0 || index >= QUEUE_SIZE)
        return NULL;

    return cir_array[index];
}


// -------------------------- Methods for Simple Store -------------------------

// Function to clear the contents of the simple store
void MessageStore::clearStore(void) {

    for (int i = 0; i < STORE_SIZE; i++)
        array[i][0] = '\0';

}


// Function prints the elements of simple store
void MessageStore::printStore(void) {
    int i, cnt;

    //count non-null elements
    cnt = 0;
    for (i = 0; i < STORE_SIZE; i++)
        if (array[i][0] != '\0')
                cnt++;

    // print headings
    PRINTD("Elements in Simple Store are: ", cnt);

    // print the simple store contents
    for (int i = 0; i < STORE_SIZE; i++)
        if (array[i][0] != '\0')
            PRINTS("\t", array[i]);
}


// Function to add element to simple store
bool MessageStore::addStore(char *str) {

    int index = indexStore();

    if (index < 0) {
        PRINT("\nFailed to add message to Store. Queue is full.\n\r");
        return false;
    } else {
        PRINT("\nSuccessfully adding message to Store.\n\r");
        sprintf(array[index], str);
        return true;
    }
}


// Function to add element to simple store
bool MessageStore::addStore(char *str, int index) {

    if (index < 0 || index > STORE_SIZE) {
        PRINT("\nFailed to add message to Store. Bad index.\n\r");
        return false;
    } else {
        PRINT("\nSuccessfully adding message to Store.\n\r");
        sprintf(array[index], str);
        return true;
    }
}


// Function to delete element from simple store
bool MessageStore::deleteStore(int index) {

    if (index < 0 || index > STORE_SIZE) {
        PRINT("\nFailed to delete message from Store. Bad index.\n\r");
        return false;
    } else {
        PRINT("\nSuccessfully deleted message from Store.\n\r");
        array[index][0] = '\0';
        return true;
    }
}


// return index of next empty message in store, return -1 if simple store is full
int MessageStore::indexStore() {

    for (int i = 0; i < STORE_SIZE; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return -1;
}


// return the number of messages in simple store
int MessageStore::countStore() {
    int i = 0;

    for (i = 0; i < STORE_SIZE; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return STORE_SIZE;
}


char *MessageStore::getStore(int index) {

    if (index < 0 || index >= STORE_SIZE)
        return NULL;

    return array[index];
}

