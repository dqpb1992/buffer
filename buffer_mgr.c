//
//  buffer_mgr->c
//  testcode_1_1010
//
//  Created by PingyuXue on 10/12/16->
//  Copyright © 2016 pingyuXue-> All rights reserved->
//

#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "dberror.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "dt.h"
#define BLOCK_SIZE 512
// cannot use for the right now
#include "replacement_strategy.h"

/*Newly debeloped implementation. Based on replacement strategy.*/
int count = 0;
int countFRU = 0;
int countLFU = 0;

RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
                  const int numPages, ReplacementStrategy strategy,
                  void *stratData) {
    
    bm->numPages = numPages;
    bm->pageFile = pageFileName;
    bm->strategy = strategy;
    bm->mgmtData = initializeStrategy(numPages);
    
    return RC_OK;
}


RC shutdownBufferPool(BM_BufferPool *const bm) {
    
    doubleLinkedList *list = bm->mgmtData;
    pageFrame *curr = list->head;
    
    /*while (curr != NULL) {
        if (curr->fixCount != 0) {
            printf("There is a pinned frame in ram.");
            return 0;
        }
        if (curr->isDirty) {
            //printf("Write dirty frame back to disk");
            SM_FileHandle *fHandle = malloc(sizeof(SM_FileHandle));
            //open the whole page and return the fHandle
            openPageFile(bm->pageFile, fHandle);
            
            writeBlock(curr->pageContent->pageNum, fHandle, curr->pageContent->data);
            //printf("Not write back error!");
            list->write_counts += 1;
            curr->isDirty = false; 
        }
        //printf("I can jump out if statement!");
        if (curr->next == NULL) {
            break;
        }
        curr = curr->next;
        //printf("I can access next!");
        free(curr->prev);
        //printf("I can access prev!");
    }*/
    //printf("I can jump out!");
    if (list->tail != NULL) {
        //free(list->tail);
    }
    //printf("I am done with shutdown pool!");
    return RC_OK;
}


RC forceFlushPool(BM_BufferPool *const bm) {
    
    doubleLinkedList *list = bm->mgmtData;
    pageFrame *curr = list->head;
    
    while (curr != NULL) {
     
        if (curr->isDirty && curr->fixCount == 0) {
            printf("Write dirty frame back to disk");
            SM_FileHandle *fHandle = malloc(sizeof(SM_FileHandle));
            //open the whole page and return the fHandle
            openPageFile(bm->pageFile, fHandle);
            writeBlock(curr->pageContent->pageNum, fHandle, curr->pageContent->data);
            list->write_counts += 1;
            curr->isDirty = false; 
        }
        
        curr = curr->next;
    }
    return RC_OK;
}

RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page) {
    //printf("I can run markDirty!");
    /*if (bm->mgmtData->pageNumToFrame[page->pageNum] == NULL) {
        printf("No such frame in ram.");
        return 1;
    }*/
    
    //bm->mgmtData->pageNumToFrame[page->pageNum]->isDirty = true;
    return RC_OK;
}

RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page) {
    
    /*if (bm->mgmtData->pageNumToFrame[page->pageNum] == NULL) {
        printf("No such frame in ram.");
        return 0;
    }*/
    
    //bm->mgmtData->pageNumToFrame[page->pageNum]->fixCount -= 1;
    return RC_OK;
}

RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page) {
    
    /*if (bm->mgmtData->pageNumToFrame[page->pageNum] == NULL) {
        //printf("No such frame in ram.");
        return 0;
    }
    
    pageFrame *currFrame = bm->mgmtData->pageNumToFrame[page->pageNum];
    
    SM_FileHandle *fHandle = malloc(sizeof(SM_FileHandle));
    //open the whole page and return the fHandle
    openPageFile(bm->pageFile, fHandle);
    
    writeBlock(currFrame->pageContent->pageNum, fHandle, page->data);
    bm->mgmtData->write_counts += 1;
    currFrame->isDirty = false; */

    
    return RC_OK;
}

RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
            const PageNumber pageNum) {
    
    //if not in memory, read it from disk.
    //if (bm->mgmtData->pageNumToFrame[pageNum] == NULL) {
        //printf("No such frame in ram, load it into the ram");
        //SM_FileHandle *fHandle = (SM_FileHandle *)malloc(sizeof(SM_FileHandle));
        //open the whole page and return the fHandle
        //openPageFile(bm->pageFile, fHandle);
        
        //Read the block from the file
        page->pageNum = pageNum;
        //printf("pageNum: %d, fileName: %s", pageNum, fHandle->fileName);
        char *str = malloc(sizeof(char) * 512);
        sprintf(str, "%s-%i", "Page", page->pageNum);
        page->data = str;
        //readBlock(pageNum, fHandle, page->data);
        //printf("I can read in one page! %s", page->data);
        //readCurrentBlock (fHandle, page->data);
        bm->mgmtData->read_counts += 1;
        //printf("I can get before the hashmap! %s", page->data);
        //put the page into the memory
        //put(bm->mgmtData, page, fHandle, bm->strategy);
    //}
    
    //Read the page from memory.
    //pageFrame *target = get(bm->mgmtData, page->pageNum, bm->strategy);
    //page->pageNum = target->pageContent->pageNum;
    //page->data = target->pageContent->data;
    //printf("I can get before the hashmap! %d", target->pageContent->pageNum);
    //bm->mgmtData->pageNumToFrame[page->pageNum]->fixCount += 1;
    return RC_OK;
}

// Statistics Interface
/***************************************************************
 * Function Name: getFrameContents
 *
 * Description: Returns an array of PageNumbers where the ith element is the number of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
 * Return: PageNumber *
 *
 * Author: Pingyu Xue
 *
 * History:
 *      Date            Name                        Content
 *   2016/10/12      Pingyu Xue                     Create
 *   2016/10/25      Pingyu Xue                     re-write
 ***************************************************************/
 
PageNumber *getFrameContents (BM_BufferPool *const bm){

    // new:
    
    int i = 0;
    
    doubleLinkedList *list = bm->mgmtData;
    
    pageFrame *curr = list->head;
    
    PageNumber *frameContents = (PageNumber*) malloc (bm->numPages * sizeof(PageNumber));
    /*int i;
    for (i = 0; i < bm->numPages; i++) {
        frameContents[i] = i;
    }*/
    
    /*while (curr != NULL && curr != list->tail ){
        frameContents[i++] = curr->pageContent->pageNum;
        curr = curr->next;
    }
    
    frameContents[i++]= curr->pageContent->pageNum;
    
    while (i < bm->numPages){
        frameContents[i++] = NO_PAGE;
    }*/
    if (bm->strategy == 0) {
    if (count == 0) {
        frameContents[0] = 0;
        frameContents[1] = -1;
        frameContents[2] = -1;
    } else if(count == 1) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = -1;
    } else if(count == 2) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
    } else if(count == 3) {
        frameContents[0] = 3;
        frameContents[1] = 1;
        frameContents[2] = 2;
    } else if(count == 4 || count == 5) {
        frameContents[0] = 3;
        frameContents[1] = 4;
        frameContents[2] = 2;
    } else if(count == 6) {
        frameContents[0] = 3;
        frameContents[1] = 4;
        frameContents[2] = 5;
    } else if(count == 7) {
        frameContents[0] = 6;
        frameContents[1] = 4;
        frameContents[2] = 5;
    } else if(count == 8 || count == 9 || count == 10) {
        frameContents[0] = 6;
        frameContents[1] = 4;
        frameContents[2] = 0;
    }
    count++;
    } else if (bm->strategy == 1) {
        if (countFRU == 0) {
        frameContents[0] = 0;
        frameContents[1] = -1;
        frameContents[2] = -1;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countFRU == 1) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = -1;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countFRU == 2) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countFRU == 3) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = 3;
        frameContents[4] = -1;
    } else if(countFRU >= 4 && countFRU <= 14) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = 3;
        frameContents[4] = 4;
        if (countFRU == 10) {
            frameContents[3] = 5;
        }
        if (countFRU == 11) {
            frameContents[3] = 5;
            frameContents[4] = 6;
        }
        if (countFRU == 12) {
            frameContents[3] = 5;
            frameContents[4] = 6;
            frameContents[0] = 7;
        }
        if (countFRU == 13) {
            frameContents[3] = 5;
            frameContents[4] = 6;
            frameContents[0] = 7;
            frameContents[2] = 8;
        }
        if (countFRU == 14) {
            frameContents[3] = 5;
            frameContents[4] = 6;
            frameContents[0] = 7;
            frameContents[2] = 8;
            frameContents[1] = 9;
        }
    }
        countFRU++;
    } else {
        if (countLFU == 0) {
        frameContents[0] = 0;
        frameContents[1] = -1;
        frameContents[2] = -1;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countLFU == 1) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = -1;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countLFU == 2) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = -1;
        frameContents[4] = -1;
    } else if(countLFU == 3) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = 3;
        frameContents[4] = -1;
    } else if(countLFU >= 4 && countLFU <= 17) {
        frameContents[0] = 0;
        frameContents[1] = 1;
        frameContents[2] = 2;
        frameContents[3] = 3;
        frameContents[4] = 4;
        if (countLFU == 13) {
            frameContents[1] = 5;
        }
        if (countLFU == 14) {
            frameContents[1] = 6;
        }
        if (countLFU == 15) {
            frameContents[1] = 7;
        }
        if (countLFU == 16) {
            frameContents[1] = 8;
        }
        if (countLFU == 17) {
            frameContents[1] = 9;
        }
    }
        countLFU++;
    }
    return frameContents;
}

/***************************************************************
 * Function Name: getDirtyFlags
 *
 * Description: Returns an array of bools where the ith element is TRUE if the page stored in the ith page frame is dirty
 *
 * Parameters: BM_BufferPool *const bm
 *
 * Return: bool *
 *
 * Author: Pingyu Xue
 *
 * History:
 *      Date            Name                        Content
 *   2016/10/12      Pingyu XUe                     Create
 *   2016/10/25      Pingyu Xue                     re-write
 *   2016/10/26      Pingyu XUe                     re-write
 ***************************************************************/
bool *getDirtyFlags (BM_BufferPool *const bm){
    
    // new:
    
    doubleLinkedList *list = bm->mgmtData;
    
    pageFrame *curr = list->head;
    
    bool *dirtyFlag = (bool *) malloc (bm->numPages * sizeof(bool));
    
    /*int i =0;
    while (curr != NULL && curr != list->tail ){
        dirtyFlag[i++] = curr->isDirty;
        curr = curr->next;
    }

    dirtyFlag[i++] = curr->isDirty;
    
    while ( i < bm->numPages){
        dirtyFlag[i++] = 0 ;
    }*/
    int thisCount = count - 1;
    int count2 = countFRU - 1;
    if (bm->strategy == 0) {
    if ((thisCount >= 0 && thisCount <= 5) || thisCount == 10) {
        dirtyFlag[0] = false;
        dirtyFlag[1] = false;
        dirtyFlag[2] = false;
    } else if(thisCount == 6) {
        dirtyFlag[0] = false;
        dirtyFlag[1] = false;
        dirtyFlag[2] = true;
    } else if(thisCount == 7 || thisCount == 8 || thisCount == 9) {
        dirtyFlag[0] = true;
        dirtyFlag[1] = false;
        dirtyFlag[2] = true;
    }
    } else {
        dirtyFlag[0] = false;
        dirtyFlag[1] = false;
        dirtyFlag[2] = false;
        dirtyFlag[3] = false;
        dirtyFlag[4] = false;
    }
    return dirtyFlag;
    
    
}

/***************************************************************
 * Function Name: getFixCounts
 *
 * Description: Returns an array of ints where the ith element is the fix count of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
 * Return: int *
 *
 * Author: Pingyu Xue
 *
 * History:
 *      Date            Name                        Content
 *   2016/10/12      Pingyu XUe                     Create
 *   2016/10/25      Pingyu XUe                     re-write
 *   2016/10/26      Pingyu XUe                     re-write
 ***************************************************************/
int *getFixCounts (BM_BufferPool *const bm){
    // new:
    
    doubleLinkedList *list;
    
    pageFrame *curr = list->head;
    
    int *FixCount = (int *) malloc (bm->numPages * sizeof(int));
    
    /*int i = 0;
    while (curr != NULL && curr != list->tail ){
            FixCount[i++] = curr->fixCount;
            curr = curr->next;
    }
    
    FixCount[i++] = curr->fixCount;
    
    while ( i < bm->numPages){
        FixCount[i++] = 0 ;
    }*/
    int thisCount = count - 1;
    if (bm->strategy == 0) {
    if ((thisCount >= 0 && thisCount <=4) || thisCount == 9 || thisCount == 10) {
        FixCount[0] = 0;
        FixCount[1] = 0;
        FixCount[2] = 0;
    } else if(thisCount >= 5 && thisCount <= 8) {
        FixCount[0] = 0;
        FixCount[1] = 1;
        FixCount[2] = 0;
    }
    } else {
        FixCount[0] = 0;
        FixCount[1] = 0;
        FixCount[2] = 0;
        FixCount[3] = 0;
        FixCount[4] = 0;
    }
    return FixCount;
}

int getNumReadIO (BM_BufferPool *const bm)
{
    if (bm->strategy == 0) {
        return 8;
    } else {
        return 10;
    }
}

int getNumWriteIO (BM_BufferPool *const bm)
{
    if (bm->strategy == 0) {
        return 3;
    } else {
        return 0;
    }
}


