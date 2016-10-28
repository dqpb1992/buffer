#ifndef REPLACEMENT_STRATEGY_H
#define REPLACEMENT_STRATEGY_H
#include <stdbool.h>
#include "storage_mgr.h"
#include "buffer_mgr.h"

/*The structure of a frame in memory-> Used to 
implement an double linkedlist based eviction strategy->*/
typedef struct pageFrame {
    int freq;
    int fixCount;
   // int isDirty; // is this dirty ---Uday/Vaibhav review comment : don't use as an int isDirty, make it bool   */
    bool isDirty;   /* Uday/Vaibhav */
    BM_PageHandle *pageContent;
    struct pageFrame *prev;
    struct pageFrame *next;
    
    
} pageFrame;

/*The data structure used to hold frames in memory*/
typedef struct doubleLinkedList {
    pageFrame *head;
    pageFrame *tail;
    int capacity;
    int currSize;
    int minFreq;
    int write_counts;
    int read_counts;
    struct pageFrame **pageNumToFrame;
    
} doubleLinkedList;

// The strategy for management of buffer pool

/*Initialize the linkedHashMap data structure*/
doubleLinkedList *initializeStrategy(int capacity);

/*Put a page into the data structure as per strategy*/
RC put(doubleLinkedList *list, BM_PageHandle *page, SM_FileHandle *fHandle, const ReplacementStrategy algorithm);

/*Get a page from the data structure as per strategy*/
pageFrame *get(doubleLinkedList *list, int pageNumber, const ReplacementStrategy algorithm);

#endif
