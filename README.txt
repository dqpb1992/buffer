=====================================================================================
=                   525 Programming Assignment 2 README file                        = 
=====================================================================================
                    1. Personnel information

     Hanqiao Lu                 A20324072               hlu22@hawk.iit.edu
     Pingyu Xue                 A20358086               pxue2@hawk.iit.edu
     Uday Tak                   A20381053               utak@hawk.iit.edu
     Vaibhav Uday Hongal	  A20378220               vhongal@hawk.iit.edu

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    2. File list 

  - buffer_mgr.c
  - buffer_mgr.h
  - buffer_mgr_stat.c
  - buffer_mgr_stat.h
  - dberror.c
  - dberror.h
  - dt.h
  - Makefile
  - README
  - storage_mgr.c
  - storage_mgr.h
  - replacement_strategy.c
  - replacement_strategy.h
  - test_assign2_1.c
  - test_helper.h

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    3. Milestone

  2016/10/26 Complete project.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    4. Installation instruction  
  using test_assign2_1.c test:
    $ make test1
    $ ./test1

  after test, use clean to delete files except source code.
    $ make clean

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    5. Function descriptions: of all additional functions  

/***************************************************************
 * Function Name: initBufferPool
 *
 * Description: initBufferPool creates a new buffer pool with numPages page frames using the page replacement strategy strategy. The pool is used to cache pages from the page file with name pageFileName. Initially, all page frames should be empty. The page file should already exist, i.e., this method should not generate a new page file. stratData can be used to pass parameters for the page replacement strategy. For example, for LRU-k this could be the parameter k.
 *
 * Parameters: BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData.
 *
***************************************************************/

/***************************************************************
 * Function Name: shutdownBufferPool
 *
 * Description: shutdownBufferPool destroys a buffer pool. This method should free up all resources associated with buffer pool. For example, it should free the memory allocated for page frames. If the buffer pool contains any dirty pages, then these pages should be written back to disk before destroying the pool. It is an error to shutdown a buffer pool that has pinned pages.
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/


/***************************************************************
 * Function Name: forceFlushPool
 *
 * Description: forceFlushPool causes all dirty pages (with fix count 0) from the buffer pool to be written to disk.
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/

/***************************************************************
 * Function Name: markDirty
 *
 * Description:mark a page as dirty
 *
 * Parameters: BM_BufferPool *const bm, BM_PageHandle *const page
 *
***************************************************************/

/***************************************************************
 * Function Name: unpinPage
 *
 * Description:unpin a page indicate by "page"
 *
 * Parameters:BM_BufferPool *const bm, BM_PageHandle *const page
 *
***************************************************************/

/***************************************************************
 * Function Name: forcePage
 *
 * Description:write the current content of the page back to the page file on disk
 *
 * Parameters:BM_BufferPool *const bm, BM_PageHandle *const page
 *
***************************************************************/

/***************************************************************
 * Function Name: pinPage
 *
 * Description:pin a page, if the page does not exist in memory, read it from file
 *
 * Parameters:BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum
 *
***************************************************************/

/***************************************************************
 * Function Name: getFrameContents
 *
 * Description: Returns an array of PageNumbers where the ith element is the number of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/

/***************************************************************
 * Function Name: getDirtyFlags
 *
 * Description: Returns an array of bools where the ith element is TRUE if the page stored in the ith page frame is dirty
 *
 * Parameters: BM_BufferPool *const bm
 *
 *
***************************************************************/

/***************************************************************
 * Function Name: getFixCounts
 *
 * Description: Returns an array of ints where the ith element is the fix count of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/

/***************************************************************
 * Function Name: getNumReadIO
 *
 * Description: Returns an array of ints where the ith element is the fix count of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/

/***************************************************************
 * Function Name: getNumWriteIO
 *
 * Description: Returns an array of ints where the ith element is the fix count of the page stored in the ith page frame
 *
 * Parameters: BM_BufferPool *const bm
 *
***************************************************************/

/***************************************************************
 * Function Name: strategyFIFOandLRU
 *
 * Description: decide use which frame to save data using FIFO.
 *
 * Parameters: BM_BufferPool *bm
 *
***************************************************************/


Additional  function: (replacement_strategy.c)
/***************************************************************
 * Function Name: initializeStrategy
 *
 * Description: initialize the doubleLinklist link list.
 *
 * Parameters: int capacity
 *
 * Return: doubleLinkedList *list
 *
***************************************************************/

/***************************************************************
 * Function Name: createFrame
 *
 * Description: create the pageFrame List
 *
 * Parameters: BM_PageHandle *page
 * 
 * Return: pageFrame
 *
***************************************************************/

/***************************************************************
 * Function Name: fifo_put
 *
 * Description: FIFO strategy interface 
 *
 * Parameters: doubleLinkList *list, BM_PageHandle *page, SM_FileHandle *fileHandle
 *
 * Return: RC
 *
 * Author: Xiaoliang Wu
 *
***************************************************************/

/***************************************************************
 * Function Name: fifo_eviction
 *
 * Description: FIFO eviction strategy interface 
 *
 * Parameters: doubleLinkList *list, SM_FileHandle *fileHandle
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: fifo_get
 *
 * Description: FIFO strategy interface 
 *
 * Parameters: doubleLinkList *list, int pageNumber 
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: lru_get
 *
 * Description: LRU strategy interface 
 *
 * Parameters: doubleLinkList *list, BM_PageHandle *page, SM_FileHandle *fileHandle
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: lru_put
 *
 * Description: LRU strategy interface 
 *
 * Parameters: doubleLinkList *list, BM_PageHandle *page, SM_FileHandle *fileHandle
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: lru_eviction
 *
 * Description: lru strategy interface 
 *
 * Parameters: doubleLinkList *list, SM_FileHandle *fileHandle
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: put
 *
 * Description: get a page from the data structure as per strategy
 *
 * Parameters: doubleLinkList *list, BM_PageHandle *page, SM_FileHandle *fileHandle, const ReplacementStrategy algorithm
 *
 * Return: RC
 *
***************************************************************/

/***************************************************************
 * Function Name: get
 *
 * Description: put a page into data structure  
 *
 * Parameters: doubleLinkList *list, BM_PageHandle *page, SM_FileHandle *fileHandle, const ReplacementStrategy algorithm
 *
 * Return: RC
 *
***************************************************************/



~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    6. Data structure: main data structure used

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

 typedef struct pageFrame {
    int freq;
    int fixCount;
    bool isDirty;   
    BM_PageHandle *pageContent;
    struct pageFrame *prev;
    struct pageFrame *next;
} pageFrame;

typedef struct BM_BufferPool {
    char *pageFile;
    int numPages;
    ReplacementStrategy strategy;
    struct  doubleLinkedList *mgmtData;       
} BM_BufferPool;

typedef struct BM_PageHandle {
    PageNumber pageNum;
    char *data;
} BM_PageHandle;

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    7. Extra credit: of all extra credits 

  No extra credit.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    8. Additional files: of all additional files 
  replacement_strategy.h
  replacement_strategy.c

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    9. Test cases: of all additional test cases added 

  Test case:
    testCreatingAndReadingDummyPages()
      test initBufferPool, pinPage, markDirty, unpinPage, shutdownBufferPool
    testReadPage()
      test forcePage 
    testFIFO()
      test function when strategy is FIFO
    testLRU()
      test functions when strategy is LRU
          
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    10. Problems solved  
  Implement all required functions and additional test case.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    11. Problems to be solved

  This program could be optimized.