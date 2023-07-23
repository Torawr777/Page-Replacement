#include "helper.h"

int isCached(int page) {

    // Checks to see if page is cached
    struct node *newNode = head;

    while(newNode != NULL) {

        if((*newNode).page == page) {
            return 1;
        }
        newNode = (*newNode).next;
    }
    // If page is not in cache return flag 0 
    return 0;
}

void addPage(int page) {

    // Checks to see if cache is full or empty
    if(count == max || head == NULL) {
        head = newNodes(page);
        (*head).prev = NULL;
        (*head).next = NULL;
    }

    else {

        // If not full, traverse LL to find tail node then add node
        struct node *temp = head;

        while((*temp).next != NULL) {
            temp = (*temp).next;
        }

        (*temp).next = newNodes(page);
        (*temp).next->prev = temp;
        (*temp).next->next = NULL;
    }
    count++;
}

// Handles page faults
void handlePF(int page) {
    // Memory is full so we need to delete the least recently used page
    if(count == max) {
        if((*head).dPage) {
            writeBackTime += 10;
            totalWBT += writeBackTime;
        }

        struct node *oldHead = head;
        head = (*oldHead).next;

        // If there are still nodes in the list, then set NULL to indicate new head
        if(head != NULL) {
            (*head).prev = NULL;
        }

        free(oldHead);
        count--;

        // If the head is NULL after deletion, update the tail to NULL as well
        if(head == NULL) {
            tail = NULL;
        }
    }

    addPage(page);
    pageMiss++;

    // Check if the page being replaced is the tail and if it is dirty
    if(tail != NULL && (*tail).page == page && (*tail).dPage) {
        // Add time for writing back dirty page
        writeBackTime += 10;
        totalWBT += writeBackTime;
    }

    // Update the time for page faults
    pageMissTime += 5 + writeBackTime;

    // Reset writeBackTime for next page replacement
    writeBackTime = 0;
}

void handlePageRead(int page) {

    switch(algorithm) {

        // FIFO
        case 1:

            // If already cache, do nothing otherise handle page fault
            if(isCached(page)) {
                return;
            }
            handlePF(page);

            break;

        // LRU
        case 2:
            
            // if the page is already cached and there is more than one page in memory
            if(isCached(page) && max != 1) {

                struct node *temp = head;

                while(1) {

                    // If page is found, remove it
                    if((*temp).page == page) {

                        if((*temp).prev != NULL) {
                            (*temp).prev->next = (*temp).next;
                        }

                        if((*temp).next != NULL) {
                            (*temp).next->prev = (*temp).prev;
                        }
                        break;
                    }
                    temp = (*temp).next;
                }
                
                struct node *tail = head;

                // Moving page to the end
                while((*tail).next != NULL) {
                    tail = (*tail).next;
                }
                
                // If already at end, do nothing
                if((*tail).page == (*temp).page) {
                    return;
                }
                
                // Update pointers
                if((*temp).page == (*head).page) {
                    head = (*temp).next;
                }
                
                (*tail).next = temp;
                (*temp).prev = tail;
                (*temp).next = NULL;
            } 

            // If not cache then handle page fault
            else {
                handlePF(page);
            }
            break;

        default:
            printf("Invalid algorithm.\n");
            break;
    }
}

void handlePageWrite(int page) {

    if(isCached(page)) {
        // If the page is already isCached, set the dirty page flag to 1 to indicate it was modified
        struct node *temp = head;
        while ((*temp).page != page) {
            temp = (*temp).next;
        }

        (*temp).dPage = 1;

        // if the page is already cached and there is more than one page in memory
        if(isCached(page) && max != 1 && strcmp(algName, "LRU") == 0) {
            printf("alg is %s", algName);
            struct node *temp = head;

            while(1) {

                // If page is found, remove it
                if((*temp).page == page) {

                    if((*temp).prev != NULL) {
                        (*temp).prev->next = (*temp).next;
                    }

                    if((*temp).next != NULL) {
                        (*temp).next->prev = (*temp).prev;
                    }
                    break;
                }
                temp = (*temp).next;
            }
            
            struct node *tail = head;

            // Moving page to the end
            while((*tail).next != NULL) {
                tail = (*tail).next;
            }
            
            // If already at end, do nothing
            if((*tail).page == (*temp).page) {
                return;
            }
            
            // Update pointers
            if((*temp).page == (*head).page) {
                head = (*temp).next;
            }
            
            (*tail).next = temp;
            (*temp).prev = tail;
            (*temp).next = NULL;
        } 
    } 
    else {
        // If the page is not isCached, handle the page fault and then recurse to write to the cache
        handlePF(page);
        handlePageWrite(page);
    }
}

void handlePageRef(FILE *file) {

    while(getline(&line, &linelen, file) != -1) {
        
        int page; 

        // Skip if line does not contain valid info
        if(sscanf(line, "%c %i", &operation, &page) != 2) {
            continue; 
        }

        totalPageR++;

        // Calls either two functions depending on read/write
        if(operation == 'R') {
            handlePageRead(page);
        }
        else if(operation == 'W') {
            handlePageWrite(page);
        }
        
        free(line);
        line = NULL;
    }
    
    free(line);
}