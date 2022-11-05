
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:
    This structure is called a linked list, or more specifically, a “Singly linked list”.
    A linked list is a type of data structure that is made up of nodes that
    are created using self referential structures. Each of these nodes contain
    two parts, namely the data and the reference to the next list node.
    Only the reference to the first list node is required to access the whole linked list.
    This is known as the head. The last node in the list points to nothing so it stores NULL in that part.

REFERENCE MATERIALS:
    https://www.geeksforgeeks.org/data-structures/linked-list/
    https://www.geeksforgeeks.org/difference-c-structures-c-structures/

SOURCES:
    Code adapted from https://www.softwaretestinghelp.com/linked-list/
    Code adapted from https://www.bitdegree.org/learn/linked-list-c-plus-plus

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#pragma once                // compiler to skip subsequent includes of this file

#define DATASIZE 30         // max characters in Node data

//#include <iostream>
//#include <cstddef>

using namespace std;


/*// A node within the linked list*/
//class Node {
  //private:
    ////-------------- private variables -------------
   //char data[DATASIZE];
   //Node *next;

    ////--------------- private methods --------------

  //public:
    ////-- constructors & destructors for the class --
    //Node(void);
    //Node(char *);
    //~Node(void);

    ////--------------- public methods ---------------
/*};*/


// A node within the linked list
struct Node {
   char data[DATASIZE];
   Node *next;
};



// the singly linked list
class LinkedList {
  private:
    //-------------- private variables -------------
    int count;
    Node *head;
    Node *tail;

    //--------------- private methods --------------
    int printList(void);
    bool deleteList(void);
    bool pushList(char*);
    bool appendList(char*);
    bool insertList(char*, int);
    bool insertList(char*, char*);
    bool insertList(char*, Node*);

  public:
    //-- constructors & destructors for the class --
    LinkedList(void);
    ~LinkedList(void);

    //--------------- public methods ---------------
    void testcases(void);
};
