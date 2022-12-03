#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct new_block
{
    size_t size;
    struct new_block* next;
}new_block;

static new_block new_first = { 0, 0 };

// bytes to add
static const size_t align_to = 8;

void *mymalloc(size_t size)
{
    size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
    new_block* block = new_first.next;
    new_block** head = &(new_first.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char*)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }
    block = (new_block*)sbrk(size);
    block->size = size;
    return ((char*)block) + sizeof(size_t);
}


void *mycalloc(size_t nmemb, size_t size)
{
    size_t nmemb_new = nmemb * size;
    void *ptr = mymalloc( nmemb_new );
    memset( ptr, 0, nmemb_new );
    return(ptr);
}

size_t getsize(void * ptr) {
    size_t * in = ptr;
    if (in)
    { 
        --in; 
        return *in; 
    }
    return 0;
}

void *myrealloc(void *ptr, size_t size)
{
    void *newptr;
    unsigned int msize;
    msize = getsize(ptr);
    newptr = malloc(size); 
    if (ptr == NULL)
    {
        return malloc (size);
    }
    else
    {
        if(size <= msize)
        {
            return ptr;
        }
        else
        {
        newptr = malloc(size);
        memcpy(newptr, ptr, msize);
        free(ptr);
        return newptr;
        }
    }
}

void myfree(void *ptr)
{

    new_block* block = (new_block*)(((char*)ptr) - sizeof(size_t));
    block->next = new_first.next;
    new_first.next = block;
}

/*
 * Enable the code below to enable system allocator support for your allocator.
 * Doing so will make debugging much harder (e.g., using printf may result in
 * infinite loops).
 */

void *malloc(size_t size) { return mymalloc(size); }
void *calloc(size_t nmemb, size_t size) { return mycalloc(nmemb, size); }
void *realloc(void *ptr, size_t size) { return myrealloc(ptr, size); }
void free(void *ptr) { myfree(ptr); }

