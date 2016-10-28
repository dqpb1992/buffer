#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer_mgr.h"
#include "replacement_strategy.h"
#include "storage_mgr.h"
#define MAX 100000
#define totalNumPages 10000


/*Author: Hanqiao Lu
 *Date: 23th Oct
 *CWID: A20324072
 */
/* Modifications by Uday Tak (CWID : A20381053) and Vaibhav Hongal (CWID : A20378220) Date : 24th Oct 2015 */

doubleLinkedList *initializeStrategy(int capacity) {
    doubleLinkedList *list = (doubleLinkedList*)malloc(sizeof(doubleLinkedList)); //Uday/Vaibhav review: for each frame we need to create separate memory
    //How to do something like this??
    //------------------------------
    //frames is an array used as a hashmap from pageNum to actual page struct->
    
    pageFrame *frames[totalNumPages];
    //test:
    int i;
    for(i = 0; i < totalNumPages; i++) {
        frames[i] = NULL; /*Hanq's Code -- we need to comment this line - Uday / Vaibhav Review*/
    }
    list->pageNumToFrame = frames;
    //----------------------------
    list->head = NULL;
    list->tail = NULL;
    list->capacity = capacity;
    list->minFreq = MAX;
    list->read_counts = 0;
    list->write_counts = 0;
    return list;
}

pageFrame *createFrame(BM_PageHandle *page) {
    pageFrame *node = (pageFrame *) malloc(sizeof(pageFrame));
  //  node->pageContent = page;
    node->pageContent = page;
    node->freq = 0;
    node->fixCount = 0;
    node->isDirty = false;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

int fifo_put(doubleLinkedList *list, BM_PageHandle *page, SM_FileHandle *fHandle) {
    //case1: empty list
    if (list->tail == NULL) {
        list->tail = createFrame(page);
        list->head = list->tail;
      
      // former code:  
      //  list->pageNumToFrame[page->pageNum] = list->tail;
        
        //test change:
        //list->pageNumToFrame-> pointerPNTF =  &(list->pageNum8ToFrame[page->pageNum]);
        //list->pageNumToFrame-> pointerPNTF = list->tail;
        // test done:
        list->pageNumToFrame[page->pageNum] = list->tail;
        list->currSize += 1;
        return 1;
    }
    //case2: the frame is already there
    
    
    if (list->pageNumToFrame[page->pageNum] != NULL) {
        printf("This page has already been in the memory->");
        return 0;
    }
    // test 2 done;
    
    //case3: the capacity is full
    if (list->currSize >= list->capacity) {
        fifo_eviction(list, fHandle);
    }
    //case4: not empty, have not been inserted, not out of capacity->
    pageFrame *newFrame = createFrame(page);
    list->tail->next = newFrame;
    newFrame->prev = list->tail;
    list->tail = newFrame;
    list->pageNumToFrame[page->pageNum] = newFrame;
 //    list->pageNumToFrame[page->pageNum] = newFrame;
    
    
    list->currSize += 1;
    printf("Added a new Frame %d successfully!", page->pageNum);
    return 1;
}

//Eviction strategy for fifo and lru
int fifo_eviction(doubleLinkedList *list, SM_FileHandle *fHandle) {
    //Find the Least Recent frame that fixCount is 0->
        pageFrame *curr = list->head;
        while (curr != NULL && curr->fixCount != 0) {
            curr = curr->next;
        }

        //All frames are pinned by client
        if (curr == NULL) {
            printf("No frames can be freed because all of them are in use!");
            return 0;
        }
        printf("Eviction of the frame %d", curr->pageContent->pageNum);
        /*Uday/Vaibhav -- we did not understand the significance of above if sstatement in FIFO
                          we dont think that we have to chck if the page is Pinned or not in FIFO, 
                          anyhow we have remove the page*/
    
    
    
        //If it is dirty, write it back to disk->
        if (curr->isDirty) {
            writeBlock(curr->pageContent->pageNum, fHandle, curr->pageContent->data);
            curr->isDirty = false; 
            printf("Dirty Data, write it into file!");
        }

        //Remove the frame from memory
        if (list->head == curr) {
            list->head = list->head->next;
            list->head->prev = NULL;
        } else if (list->tail == curr) {
            list->tail = list->tail->prev;
            list->tail->next = NULL;
        } else {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
        }
    
    // former 2:
    list->pageNumToFrame[curr->pageContent->pageNum] = NULL;
    free(curr);
    list->currSize -= 1;
    return 1;
}


pageFrame *fifo_get(doubleLinkedList *list, int pageNumber) {
    //If in the memory return it->
    
    // former code:
//    if (list->pageNumToFrame[pageNumber] != NULL) {
//       return list->pageNumToFrame[pageNumber];
//    }
    
    // test change : 
    //list->pageNumToFrame-> pointerPNTF =  &(list->pageNumToFrame[page->pageNum]);
    
    if (list->pageNumToFrame[pageNumber] != NULL) {
        // check the retuen value, it is a pointer or a value !
        return list->pageNumToFrame[pageNumber];
        printf("fifo success!");
    }
   // test done 
    
    return NULL;
}


int lru_put(doubleLinkedList *list, BM_PageHandle *page, SM_FileHandle *fHandle) {
    return fifo_put(list, page, fHandle);
}


pageFrame *lru_get(doubleLinkedList *list, int pageNumber) {
    
   
    if (list->pageNumToFrame[pageNumber] != NULL) {
        // former code:
        pageFrame *myFrame = list->pageNumToFrame[pageNumber];
        
   // test code done:
        if (myFrame->prev == NULL) {
            myFrame->next->prev = NULL;
            list->head = myFrame->next;
            list->tail->next = myFrame;
            myFrame->prev = list->tail;
            myFrame->next = NULL;
            list->tail = myFrame;
        } else if (myFrame->next == NULL) {
            //Do Nothing
        } else {
            myFrame->prev->next = myFrame->next;
            myFrame->next->prev = myFrame->prev;
            myFrame->prev = list->tail;
            list->tail->next = myFrame;
            myFrame->next = NULL;
            list->tail = list->tail->next; 
        }
        return myFrame;
    }
    return NULL;
}

int lfu_put(doubleLinkedList *list, BM_PageHandle *page, SM_FileHandle *fHandle) {
    //case1: empty list
    if (list->tail == NULL) {
        list->tail = createFrame(page);
        list->head = list->tail;
        list->pageNumToFrame[page->pageNum] = list->tail;
        list->currSize += 1;
        list->minFreq = list->tail->freq < list->minFreq ? list->tail->freq : list->minFreq;
        return 1;
    }
    //case2: the frame is already there
    // former code;
   // if (list->pageNumToFrame[page->pageNum] != NULL) {
   //   printf("This page has already been in the memory->");
   //  return 0;
   // }
    
    if (list->pageNumToFrame[page->pageNum] != NULL){
        printf("This page has already been in the memory\n");
        return 0;
    }
    
    // test done:
    //case3: the capacity is full
    if (list->currSize >= list->capacity) {
        lfu_eviction(list, fHandle);
    }
    //case4: not empty, have not been inserted, not out of capacity->
    pageFrame *newFrame = createFrame(page);
    list->tail->next = newFrame;
    newFrame->prev = list->tail;
    list->tail = newFrame;
    list->pageNumToFrame[page->pageNum] = newFrame;
    list->currSize += 1;
    list->minFreq = list->tail->freq < list->minFreq ? list->tail->freq : list->minFreq;
    printf("Added a new Frame %d successfully!", page->pageNum);
    return 1;
}

//eviction strategy for lfu
int lfu_eviction(doubleLinkedList *list, SM_FileHandle *fHandle) {
        //Find the Least Recent frame that fixCount is 0->
        pageFrame *curr = list->head;
        pageFrame *target = NULL;
        while (curr != NULL) {
            if (curr->fixCount == 0 && curr->freq <= list->minFreq) {
                target = curr;
                break;
            } 
            curr = curr->next;
        }
        //All frames are pinned by client
        if (target == NULL) {
            printf("No frames can be freed because all of them are in use!");
            return 0;
        }
        printf("Eviction of the frame %d", target->pageContent->pageNum);

        //If it is dirty, write it back to disk->
        if (target->isDirty) {
            writeBlock(target->pageContent->pageNum, fHandle, target->pageContent->data);
            target->isDirty = false; 
            printf("Dirty Data, write it into file!");
        }

        //Remove the frame from memory
        if (list->head == target) {
            list->head = list->head->next;
            list->head->prev = NULL;
        } else if (list->tail == target) {
            list->tail = list->tail->prev;
            list->tail->next = NULL;
        } else {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
        }
 
        list->pageNumToFrame[target->pageContent->pageNum] = NULL;
        free(target);
        list->currSize -= 1;
        return 1;
}


pageFrame *lfu_get(doubleLinkedList *list, int pageNumber) {
    
    //former code:
    if (list->pageNumToFrame[pageNumber] != NULL) {
        pageFrame *myFrame = list->pageNumToFrame[pageNumber];
        if (myFrame->prev == NULL) {
            myFrame->next->prev = NULL;
            list->head = myFrame->next;
            list->tail->next = myFrame;
            myFrame->prev = list->tail;
            myFrame->next = NULL;
            list->tail = myFrame;
        } else if (myFrame->next == NULL) {
            //Do Nothing
        } else {
            myFrame->prev->next = myFrame->next;
            myFrame->next->prev = myFrame->prev;
            myFrame->prev = list->tail;
            list->tail->next = myFrame;
            myFrame->next = NULL;
            list->tail = list->tail->next; 
        }
        //Update frequency
        myFrame->freq += 1;
        list->minFreq = list->tail->freq < list->minFreq ? list->tail->freq : list->minFreq;
        return myFrame;
    }
    //If not found return NULL->
    return NULL;
}

/*Integration each put strategy*/
int put(doubleLinkedList *list, BM_PageHandle *page, SM_FileHandle *fHandle, const ReplacementStrategy algorithm) {
    int state = 0;
    switch(algorithm) {
        case 0:
            state = fifo_put(list, page, fHandle);
            break;
        case 1:
            state = lru_put(list, page, fHandle);
            break;
        case 3:
            state = lfu_put(list, page, fHandle);
            break;
        default:
            printf("This strategy not implemented or not in the range->");
    }
    return state;
}

/*Get a page from the data structure as per strategy*/
pageFrame *get(doubleLinkedList *list, int pageNumber, const ReplacementStrategy algorithm) {
    pageFrame *frame = NULL;
    switch(algorithm) {
        case 0:
            frame = fifo_get(list, pageNumber);
            break;
        case 1:
            frame = lru_get(list, pageNumber);
            break;
        case 3:
            frame = lfu_get(list, pageNumber);
            break;
        default:
            printf("This strategy not implemented or not in the range->");
    }
    return frame;
}

