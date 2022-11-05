
#define TDEBUG  true       // activate trace message printing for debugging


// found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <Arduino.h>
//#include <ESP8266WiFi.h>

// found in Arduino libraries (~/Arduino/libraries)
//#include <TelnetStream.h>

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// for trace messages/debugging, found in this directory
#include "DeBug.h"
#include "LinkedList.h"

//---------------------------- Static Scoped Macros ----------------------------

//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug

// ------------------------------- Public Methods ------------------------------

// ------------------------ Constructors & Destructors -------------------------

/*// default constructors for the Node class*/
//Node::Node(void) {
   //data[0] = '\0';
   //next = NULL;
//}


//// default constructors for the Node class
//Node::Node(char *data) {
   //data[0] = '\0';
   //next = NULL;
//}


//// default destructor for the Node class
//Node::~Node(void) {
/*}*/


// default constructors for the class
LinkedList::LinkedList(void) {

    DEBUGTRACE(INFO, "Entered LinkedList constructor");

    count = 0;
    head = NULL;
    tail = NULL;

};


// destructors for the class
LinkedList::~LinkedList(void) {

    DEBUGTRACE(INFO, "Entered LinkedList destructor");

    deleteList();

};


void LinkedList::testcases(void) {

    printList();

    pushList("very first entry");
    printList();

    appendList("2nd entry");
    printList();

    appendList("very long entry...........................................");
    printList();

    appendList("make this last entry");
    pushList("make this the new first entry");
    printList();

    insertList("put this after the 3rd entry", 3);
    insertList("put after the entry where data is \"2nd entry\"", "2nd entry");
    printList();

    deleteList();

};


// ------------------------------ Private Methods ------------------------------

// delete entire list
bool LinkedList::deleteList(void) {
    Node *node_idx;

    if (head == NULL) {
        DEBUGTRACE(INFO, "Nothing to delete in linked list");
        return false;
    };

    node_idx = head;

    while(node_idx != NULL) {
        node_idx = head->next;
        delete head;
    };

    // null head & tail pointers and count
    count = 0;
    head = NULL;
    tail = NULL;

    return true;

};


//insert a new node at the top of the linked list
bool LinkedList::pushList(char *new_data) {
    int ctn;
    bool rtn = true;

    DEBUGTRACE(INFO, "Entered LinkedList::pushList");

    // create and allocate memory for new node
    Node *newNode = new Node;

    // assign data to new node
    ctn = snprintf(newNode->data, DATASIZE, "%s", new_data);

    // check for errors in assigning data to new node
    if (ctn < 0) {
        DEBUGTRACE(ERROR, "Could not push data properly to linked list");
        delete newNode;
        return false;
    };
    if (ctn >= DATASIZE) {
        DEBUGTRACE(WARN, "Could not push the data entirely to the linked list");
        rtn = false;
    };

    // increment node count
    ++count;

    // set the next of new node as head of current linked list
    newNode->next = head;

    // move the head to point to the new node
    head = newNode;

    // if this is the very first node, update tail
    if (count == 1) {
        newNode->next = NULL;
        tail = newNode;
    };

    return rtn;

};


//insert a new node at the end of the linked list
bool LinkedList::appendList(char *new_data) {
    int ctn;
    bool rtn = true;

    DEBUGTRACE(INFO, "Entered LinkedList::appendList");

    // create and allocate memory for new node
    Node *newNode = new Node;

    // assign data to new node
    ctn = snprintf(newNode->data, DATASIZE, "%s", new_data);

    // check for errors in assigning data to new node
    if (ctn < 0) {
        DEBUGTRACE(ERROR, "Could not append data properly to linked list");
        delete newNode;
        return false;
    };
    if (ctn >= DATASIZE) {
        DEBUGTRACE(WARN, "Could not push the data entirely to the linked list");
        rtn = false;
    };

    // increment node count
    ++count;

    // update pointers for linked list
    if (count == 1) {         // if this is the very first node, update head and tail
        head = newNode;
        tail = newNode;
        newNode->next = NULL;
    } else {
        tail->next = newNode;
        tail = newNode;
        newNode->next = NULL;
    };

    return rtn;

};


//insert a new node after a specified index of the linked list
bool LinkedList::insertList(char *new_data, int index) {
    int ctn, i = 0;
    bool rtn = true;
    Node* target;

    DEBUGTRACE(INFO, "Entered LinkedList::insertList(char *new_data, int index)");

    //check for errors in index
    if (index < 1 || index > count) {
        DEBUGTRACE(ERROR, "Could not insert data after index location.");
        DEBUGTRACE(ERROR, "index = ", index);
        return false;
    };

    // create and allocate memory for new node
    Node *newNode = new Node;

    // assign data to new node
    ctn = snprintf(newNode->data, DATASIZE, "%s", new_data);

    // check for errors in assigning data to new node
    if (ctn < 0) {
        DEBUGTRACE(ERROR, "Could not insert data properly to linked list");
        delete newNode;
        return false;
    };
    if (ctn >= DATASIZE) {
        DEBUGTRACE(WARN, "Could not push the data entirely to the linked list");
        rtn = false;
    };

    // increment node count
    ++count;

    // find the target index
    target = head;
    while (++i != index) {
        target = target->next;
    };
    //DEBUGTRACE(INFO, "index = ", index);
    //DEBUGTRACE(INFO, "Ths value should equal index: i = ", i);
    //DEBUGTRACE(INFO, "target->data = ", target->data);
    //DEBUGTRACE(INFO, "newNode->data = ", newNode->data);

    // move pointers so the new node to follow the target
    target->next = newNode;
    newNode->next = target->next;

/*    // if index is 1 or count, must change the head or tail*/
    //if ((index == 1 && count == 1) || index == count) {
        //newNode->next = NULL;
        //tail = newNode;
    /*}*/

    return rtn;

};


//insert a new node after a specified target of the linked list
bool LinkedList::insertList(char *new_data, Node *target) {
    int ctn, i = 0;
    bool rtn = true;

    DEBUGTRACE(INFO, "Entered LinkedList::insertList(char *new_data, Node *target)");

    // create and allocate memory for new node
    Node *newNode = new Node;

    // assign data to new node
    ctn = snprintf(newNode->data, DATASIZE, "%s", new_data);

    // check for errors in assigning data to new node
    if (ctn < 0) {
        DEBUGTRACE(ERROR, "Could not insert data properly to linked list");
        delete newNode;
        return false;
    };
    if (ctn >= DATASIZE) {
        DEBUGTRACE(WARN, "Could not push the data entirely to the linked list");
        rtn = false;
    };

    //// move the next of new node to point to the the targets current next
    //newNode->next = target->next;

    //// set the next of target to point to the new node
    //target->next = newNode;

    //// if index is 1 or count, must change the head or tail
    //if ((count == 0) || target == tail) {
        //newNode->next = NULL;
        //tail = newNode;
    /*}*/

    // increment node count
    ++count;

    DEBUGTRACE(INFO, "index = ", index);
    DEBUGTRACE(INFO, "Ths value should equal index: i = ", i);
    //DEBUGTRACE(INFO, "target->data = ", target->data);
    //DEBUGTRACE(INFO, "newNode->data = ", newNode->data);

    // move pointers so the new node to follow the target
    target->next = newNode;
    newNode->next = target->next;

    return rtn;

};


//insert a new node after a specified data entry of the linked list
bool LinkedList::insertList(char *new_data, char *match_data) {
    int ctn, index = 0;
    bool rtn = true;
    Node* target;

    DEBUGTRACE(INFO, "Entered LinkedList::insertList(char *new_data, char *match_data)");

    // find the target index
    target = head;
    while (++index <= count) {
        if (strcmp(target->data, match_data) == 0)
            index = count + 2;  // break out of the while loop
        else
            target = target->next;
    };

    // check if you ever found a match
    if (index != count + 2) {
        DEBUGTRACE(ERROR, "Could not find a match to insert data in link list");
        return false;
    };

    // insert a node after the target you found
    rtn = insertList(new_data, target);

    return rtn;

};


// print content of linked list
int LinkedList::printList(void) {
    Node *node_idx;
    int cnt = 0;

    DEBUGTRACE(INFO, "Entered LinkedList::printList");

    node_idx = head;

    if (node_idx == NULL) {
        DEBUGTRACE(INFO, "Linked list is empty");
        return cnt;
    } else {
        DEBUGTRACE(INFO, "Linked list contains ", count);
    };

    while(node_idx != NULL) {
        ++cnt;
        DEBUGTRACE(INFO, node_idx->data);
        node_idx = node_idx->next;
    };

    DEBUGTRACE(INFO, "Exiting LinkedList::printList");

    return cnt;

};







// ------------------ Explicitly Declare All Needed Functions ------------------




// ----------------- Templatize Additionally Needed Functions ------------------


