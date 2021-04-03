
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0

DESCRIPTION:
    This object calls implements two data circular queue, aka ring buffer, for holding the messages that will appear on the scrolling display.
    is a linear data structure in which the operations are performed based on FIFO (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called "Ring Buffer".

REFERENCE MATERIALS:
    * https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
    * https://stackoverflow.com/questions/1403150/how-do-you-dynamically-allocate-a-matrix

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/


#pragma once                        // compiler to skip subsequent includes of this file

#define STORE_SIZE  5               // default size of message store
#define QUEUE_SIZE  5               // default size of message queue
#define BUF_SIZE    80              // default max number of characters of a message


class MessageStore {
  private:
    int store_size, store_top;      // size of and index into top of simple store
    int queue_size, queue_top;      // size of and index into top of queue
    int queue_rear, queue_front;    // size of and indexes for front & rear of the circular queue
    int total_size;                 // total size of the message array (store + queue)
    int buffer_size;                // size of the buffer for a message
    char **array = NULL;            // memory array used to store massages

    // private methods for Simple Store
    int indexStore(void);
    int countStore(void);

    // private methods for Queue
    int indexQueue(void);
    int countQueue(void);
    bool enQueue(char *);
    char *deQueue(void);

  public:
    // constructors & destructors for the class
    MessageStore(void);
    MessageStore(int store_size, int queue_size, int buf_size);
    ~MessageStore(void);

    // public methods for Circular Queue
    int topQueue();
    int sizeQueue();
    void clearQueue();
    void printQueue();
    char *getQueue(int);
    bool addQueue(char *);

    // public methods for Simple Store
    int topStore();
    int sizeStore();
    void clearStore();
    void printStore();
    char *getStore(int);
    bool addStore(char *);
    bool addStore(char *, int);
    bool deleteStore(int);

    // public methods for MessageStore
    int top();
    int size();
    void clear();
    char *get(int);
    void print();
};



