
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.2.0

DESCRIPTION:
    This object implements two data structure in a single array.
    Part of this array is a LILO circular queue (aka ring buffer) for holding the messages that will appear on the scrolling display.
    The remaining prosion of the array is a simplely a

REFERENCE MATERIALS:
    * https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
    * https://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#define DEBUG true    // activate debugging routings (print trace messages on serial port)

// ESP8266 libraries (~/.arduino15/packages/esp8266)

// Arduino libraries (~/src/arduino/libraries)
#include <Arduino.h>

// Arduino libraries (~/Arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// simple-display project's include files (~/src/scrolling-display/test/scrolling-display)
#include "debug.h"
#include "MessageStore.h"



// ------------------------ Constructors & Destructors -------------------------


// Constructor to create MessageStore
MessageStore::MessageStore(int str_size, int que_size, int buf_size) {

    int rows = str_size + que_size;
    int cols = buf_size + 1;          // adding 1 to include null character at end of string

    store_size = str_size;
    store_top = 0;                    // simple store indexes from 0 to store_size

    buffer_size = buf_size;
    queue_size = que_size;
    queue_top = str_size;             // queue indexes from queue_top to store_size + queue_size
    queue_front = queue_rear = -1;

    total_size = store_size + queue_size;

    // create array used to store and queue messages
    array = new char*[rows];
    if (rows) {
        array[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            array[i] = array[0] + i * cols;
    }

    INFOD("store_size = ", store_size);
    INFOD("queue_size = ", queue_size);
    INFOD("total_size = ", total_size);
    INFOD("buffer_size = ", buffer_size);
    INFOD("store_top = ", store_top);
    INFOD("queue_top = ", queue_top);
    INFOD("queue_front = ", queue_front);
    INFOD("queue_rear = ", queue_rear);

}


// Constructor to create MessageStore
MessageStore::MessageStore(void) {

    int rows = STORE_SIZE + QUEUE_SIZE;
    int cols = BUF_SIZE + 1;          // adding 1 to include null character at end of string

    store_size = STORE_SIZE;
    store_top = 0;                    // simple store indexes from 0 to store_size

    buffer_size = BUF_SIZE;
    queue_size = QUEUE_SIZE;
    queue_top = STORE_SIZE;           // queue indexes from queue_top to store_size + queue_size
    queue_front = queue_rear = -1;

    total_size = store_size + queue_size;

    // create array used to store and queue messages
    array = new char*[rows];
    if (rows) {
        array[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            array[i] = array[0] + i * cols;
    }

    INFOD("store_size = ", store_size);
    INFOD("queue_size = ", queue_size);
    INFOD("total_size = ", total_size);
    INFOD("buffer_size = ", buffer_size);
    INFOD("store_top = ", store_top);
    INFOD("queue_top = ", queue_top);
    INFOD("queue_front = ", queue_front);
    INFOD("queue_rear = ", queue_rear);

}


// Destructor to delete MessageStore
MessageStore::~MessageStore(void) {

    int rows = store_size + queue_size;

    // delete array used to store and queue messages
    if (rows) delete [] array[0];
    delete [] array;

}



// --------------------- Private Methods for Simple Store ----------------------

// return index of next empty message in store, return -1 if simple store is full
int MessageStore::indexStore() {

    for (int i = store_top; i < store_top + store_size; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return -1;
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



// -------------------- Private Methods for Circular Queue ---------------------

// return index of next empty message in queue, return -1 if queue is full
int MessageStore::indexQueue() {

    for (int i = queue_top; i < queue_top + queue_size; i++)
        if (array[i][0] == '\0') {
            return i;
        }

    return -1;
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


// function inserts an element into the circular queue at the rear position
bool  MessageStore::enQueue(char *str) {

    if ((queue_front == queue_top && queue_rear == queue_top + queue_size - 1)
            || (queue_rear == (queue_top + queue_front - 1)%(queue_size - 1))) {
        INFO("Failed to add message to Queue. Queue is full.");
        return false;
    }

    else if (queue_front == -1) {                // insert first element
        queue_front = queue_rear = queue_top;
        sprintf(array[queue_rear], str);
    }

    else if (queue_rear == queue_top + queue_size - 1 && queue_front != queue_top) {
        queue_rear = queue_top;
        sprintf(array[queue_rear], str);
    }

    else {
        queue_rear++;
        sprintf(array[queue_rear], str);
    }

    return true;

}

// function deletes an element from the circular queue at the front position
char *MessageStore::deQueue(void) {

    char *data;

    if (queue_front == -1) {
        INFO("Queue is empty.");
        return NULL;
    }

    data = array[queue_front];
    array[queue_front][0] = '\0';
    if (queue_front == queue_rear) {
        queue_front = -1;
        queue_rear = -1;
    }
    else if (queue_front == queue_top + queue_size - 1)
        queue_front = queue_top;
    else
        queue_front++;

    return data;

}



// --------------------- Public Methods for Simple Store -----------------------

// Function to clear the contents of the simple store
void MessageStore::clearStore(void) {

    for (int i = store_top; i < store_top + store_size; i++)
        array[i][0] = '\0';

    INFO("Simple store has been cleared.");

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

    int size = strlen(str);
    int index = indexStore();

    // check size of message
    if (size > buffer_size) {
        ERRORD("Attempting to add message longer then buffer in simple store. size = ", size);
        INFOD("Message buffer size: buffer_size = ", buffer_size);
        return false;
    }

    if (index < store_top || index > store_top + store_size - 1) {
        ERRORD("Failed to add message to Store. Bad index.  index = ", index);
        return false;
    } else if (index < 0) {
        INFO("Failed to add message to Store. Store is full.");
        return false;
    } else {
        INFO("Successfully adding message to Store.");
        sprintf(array[index], str);
    }

    //EXEC(printStore())
    return true;
}


// Function to add element to specific index of simple store
bool MessageStore::addStore(char *str, int index) {

    int size = strlen(str);

    // check size of message
    if (size > buffer_size) {
        ERRORD("Attempting to add message longer then buffer in simple store. size = ", size);
        INFOD("Message buffer size: buffer_size = ", buffer_size);
        return false;
    }

    if (index < store_top || index > store_top + store_size - 1) {
        ERRORD("Failed to add message to Store. Bad index.  index = ", index);
        return false;
    } else {
        INFO("Successfully adding message to Store.");
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
        INFO("Successfully deleted message from Store.");
        array[index][0] = '\0';
        return true;
    }
}


char *MessageStore::getStore(int index) {

    if (index < store_top || index >= store_top + store_size) {
        ERRORD("Failed to get message from Store. Bad index. index = ", index);
        return NULL;
    }

    return array[index];
}


int MessageStore::topStore() {

    return store_top;
}


int MessageStore::sizeStore() {

    return store_size;
}



// -------------------- Public Methods for Circular Queue ----------------------

// Function to clear the contents of the circular queue
void MessageStore::clearQueue(void) {

    for (int i = queue_top; i < queue_top + queue_size; i++)
        array[i][0] = '\0';

    queue_front = queue_rear = -1;

    INFO("Circular queue has been cleared.");

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

    int size = strlen(str);

    // check size of message
    if (size > buffer_size) {
        ERRORD("Attempting to add message longer then buffer in circular queue. size = ", size);
        INFOD("Message buffer size: buffer_size = ", buffer_size);
        return false;
    }

/*
    if ((queue_front == queue_top && queue_rear == queue_size - 1) || (queue_rear == (queue_front - 1)%(queue_size - 1))) {
        INFO("Circular Queue is full. Removing element from end of message queue.");
    } else if (queue_front == -1) {     // (room in queue) queue is empty, so insert into first element
        queue_front = queue_rear = queue_top;
        sprintf(array[queue_front], str);
    } else if (queue_rear == queue_size - 1 && queue_front != queue_top) {
        queue_rear = queue_top;
        sprintf(array[queue_rear], str);
    } else {
        queue_rear++;
        sprintf(array[queue_rear], str);
    }
*/

/*
    if (queue_front == queue_top && queue_rear == queue_size - 1 || queue_rear - queue_front == 1) {
        INFO("Circular Queue is full. Removing element from end of message queue.");
    }

    if (queue_front == -1) {        // (room in queue) queue is empty, so insert into first element
        queue_front = queue_top;
        queue_rear = queue_top;
        sprintf(array[queue_rear], str);
        return true;
    } else if (queue_rear >= queue_front && queue_front == queue_top) {
        queue_rear++;
        sprintf(array[queue_rear], str);
        return true;
    } else if (queue_rear == queue_top + queue_size - 1 && queue_front == queue_top) {
        queue_front = queue_top + 1;
        queue_rear = queue_top;
        sprintf(array[queue_rear], str);
        return true;
    } else if (queue_front - queue_rear == 1 && queue_front != queue_top + queue_size - 1) {
        queue_front++;
        queue_rear++;
        sprintf(array[queue_rear], str);
        return true;
    } else if (queue_front - queue_rear == 1 && queue_front != queue_top + queue_size - 1) {
        queue_front = queue_top;
        queue_rear++;
        sprintf(array[queue_rear], str);
        return true;
    } else if (queue_front - queue_rear == 1 && queue_front == queue_top + queue_size - 1) {
        queue_front = queue_top;
        queue_rear++;
        sprintf(array[queue_rear], str);
        return true;
    }

    ERROR("You should never reach the point in MessageStore::addQueue.");
    printQueue();
    delay(50000);
    return false;
*/

    if ((queue_front == queue_top && queue_rear == queue_top + queue_size - 1) || (queue_rear == (queue_top + queue_front - 1)%(queue_size - 1))) {
        char *string = deQueue();
        INFOS("Queue is full. Removing element from circular queue = ", string);
        return enQueue(str);
    } else {
        INFOS("Element added to circular queue = ", str);
        return enQueue(str);
    }

}


char *MessageStore::getQueue(int index) {

    if (index < queue_top || index >= queue_top + queue_size) {
        ERRORD("Failed to get message from Queue. Bad index. index = ", index);
        return NULL;
    }

    return array[index];

}


int MessageStore::topQueue() {

    return queue_top;
}


int MessageStore::sizeQueue() {

    return queue_size;
}



// -------------------------- Methods for MessageStore -------------------------

int MessageStore::top() {

    return store_top;
}


int MessageStore::size(void) {
    return store_size + queue_size;
}


void MessageStore::clear(void) {
    for (int i = store_top; i < store_size + queue_size; i++)
        array[i][0] = '\0';

    queue_front = queue_rear = -1;
}


char *MessageStore::get(int index) {

    if (index < store_top || index >= store_top + store_size + queue_size) {
        ERRORD("Failed to get message from Queue. Bad index. index = ", index);
        return NULL;
    }

    return array[index];

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
    INFOD("store_size = ", store_size);
    INFOD("queue_size = ", queue_size);
    INFOD("total_size = ", total_size);
    INFOD("buffer_size = ", buffer_size);
    INFOD("store_top = ", store_top);
    INFOD("queue_top = ", queue_top);
    INFOD("queue_front = ", queue_front);
    INFOD("queue_rear = ", queue_rear);

    // print the queue contents
    for (int i = store_top; i < store_size + queue_size; i++)
        if (array[i][0] != '\0')
            INFOS("\t", array[i]);
}

