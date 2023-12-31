/**
 * Malloc
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>

// int * ptr = 1;
// ptr + 1 is actually ptr + 1*sizeof(int)
// so when doing address arithmatic, covert to char* and back. For example, 
// ptr + 5 --> (int*) ((char*)ptr + 5) --> !!!THEY ARE NOT THE SAME!!!

// No such thing --> size_t >= 0, sometimes it will be negative

// Two NULL (pointers) can be compared with >, <, >=, <= and it's always true

typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
  struct _metadata_t* freenext_;
  struct _metadata_t* freeprev_;
} metadata_t;

metadata_t *freehead = NULL;
metadata_t *freetail = NULL;

// void printlist() {
//   // size of a metadata is 24
//   metadata_t* head = freehead;
//   if (!head) {
//     printf("empty list\n");
//     return;
//   } 
  
//   int count = 0;
//   while(head) {
//     printf("Node %d : %d ->\n",count, head->size);
//     count += 1;
//     head = head->freenext_;
//   }
// }


void removeSorted(metadata_t* remove) {
  remove->isUsed = 1;
  metadata_t* next = remove->freenext_;
  metadata_t* prev = remove->freeprev_;
  if (next && prev) {
    remove->freenext_ = remove->freeprev_ = NULL;
    prev->freenext_ = next;
    next->freeprev_ = prev;
  } else if (prev) {
    prev->freenext_ = NULL;
    remove->freeprev_ = NULL;
    freetail = prev;
  } else if (next) {
    remove->freenext_ = NULL;
    next->freeprev_ = NULL;
    freehead = next;
  } else {
    freehead = freetail = NULL;
  }
}

void memory_coalesce(metadata_t* newlyfreed) {

  metadata_t * next = newlyfreed->freenext_;
  metadata_t * prev = newlyfreed->freeprev_;
  if (next && prev) {
    metadata_t* checknext = (metadata_t*) ((char*)next - sizeof(metadata_t) - newlyfreed->size);
    metadata_t* checkprev = (metadata_t*) ((char*)prev + sizeof(metadata_t) + prev->size);
    if (checknext == newlyfreed && checkprev == newlyfreed) {
      prev->size += 2*sizeof(metadata_t) + newlyfreed->size + next->size;
      removeSorted(next);
      removeSorted(newlyfreed);
      newlyfreed = next = NULL;
    } else if (checkprev == newlyfreed) {
      prev->size += sizeof(metadata_t) + newlyfreed->size;
      removeSorted(newlyfreed);
      newlyfreed = NULL;
    } else if (checknext == newlyfreed){
      newlyfreed->size += sizeof(metadata_t) + next->size;
      removeSorted(next);
      next = NULL;
    } 
  } else if (prev) {
    metadata_t* updated = (metadata_t*) ((char*)prev + prev->size + sizeof(metadata_t));
    if (updated == newlyfreed) {
      prev->size += (sizeof(metadata_t) + newlyfreed->size);
      removeSorted(newlyfreed);
      newlyfreed = NULL;
    }
  } else if (next) {
    metadata_t* updated = (metadata_t*) ((char*)newlyfreed + sizeof(metadata_t) + newlyfreed->size);
    if (updated == next) {
      newlyfreed->size += (sizeof(metadata_t) + next->size);
      removeSorted(next);
      next = NULL;
    }
  }
  // printlist(); 
}

void insertSorted(metadata_t * insert) {
  insert->isUsed = 0;
  metadata_t * head = freehead;
  metadata_t * tail = freetail;
  // If list is empty
  if (!head) {
    freehead = insert;
    freetail = insert;
    return;
  }

  // If insert appears before the head
  if (insert < head) {
    insert->freenext_ = head;
    head->freeprev_ = insert;
    freehead = insert;
    memory_coalesce(insert);
    return;
  }
  
  // If insert appears after the tail
  if (insert > tail) {
    tail->freenext_ = insert;
    insert->freeprev_ = tail;
    freetail = insert;
    memory_coalesce(insert);
    return;
  }

  // If insert appears after the head
  metadata_t * next = head->freenext_;
  while(head) {

    // If only one element (head) exists;
    if (!next) {
      head->freenext_ = insert;
      insert->freeprev_ = head;
      freetail = insert;
      break;
    
    // If at least two elements exist;
    } else if (insert > head && insert < next) {
      head->freenext_ = insert;
      insert->freenext_ = next;
      insert->freeprev_ = head;
      next->freeprev_ = insert;
      break;
    }
    head = next;
    next = next->freenext_;
  }
  memory_coalesce(insert);
}

metadata_t * search(metadata_t * head, metadata_t * tail, size_t size) {
  if (!head) { return NULL; }

  metadata_t* bestfit = NULL;
  size_t fitsize = INT_MAX;
  while (head <= tail && head) {
    
    int head_fit = (int)head->size - (int)size;
    int tail_fit = (int)tail->size - (int)size;
    
    // if ((head_fit >= 0) && head_fit >= tail_fit) {
    //   bestfit = head;
    // }

    // if ((tail_fit >= 0) && tail_fit >= head_fit) {
    //   bestfit = tail;
    // }

    if ((head_fit >= 0)) {
      return head;
    }

    if ((tail_fit >= 0)) {
      return tail;
    }
    
    // if (head->size > size) {
    //   if (head->size - size < fitsize) {
    //     fitsize = head->size - size;
    //     bestfit = head;
    //   }
    // } else if (head->size == size) {
    //   return head;
    // }

    // if (tail->size > size) {
    //   if (tail->size - size < fitsize) {
    //     fitsize = tail->size - size;
    //     bestfit = tail;
    //   }
    // } else if (tail->size == size) {
    //   return tail;
    // }

    head = head->freenext_;
    tail = tail->freeprev_;
  }
  
  return bestfit;
}

/**
 * Allocate space for array in memory
 *z
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
  void* temp = malloc(num*size);
  if (temp) {
    memset(temp, 0, num*size);
  }
  return temp;
}


/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void * malloc(size_t size) {
  // printf("start to malloc\n");
  metadata_t* free_block = search(freehead,freetail,size);
  // printf("returned\n");
  if (!free_block) {
    // printf("sbrk new\n");
    metadata_t *new = sbrk(sizeof(metadata_t));
    new->size = size;
    new->isUsed = 1;
    void* ptr = sbrk(size);
    return ptr;
  } else if (size == free_block->size) {
    // printf("perfect fit\n");
    // printlist();
    removeSorted(free_block);
    void * ptr = (void*) (free_block + 1);
    // printlist();
    return ptr;
  } else {
    // printf("block splitting\n");
    removeSorted(free_block);
    void * ptr = (void*) (free_block + 1);
    
    // printf("size is %d\n",size);
    size_t leftover = free_block->size - size;
    if (leftover >= sizeof(metadata_t)) {
      // printf("leftover size is %d\n",leftover);
      metadata_t* new = (metadata_t*) ((char*)ptr + size);
      new->size = leftover - sizeof(metadata_t);
      insertSorted(new);
      free_block->size = size;
      // printf("inserted new leftover\n");
    }
    
    // printlist();
    // printf("malloc finished");
    return ptr;
  }
}


/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void * ptr) {
  if (!ptr) {
    return;
  }
  metadata_t *newfreed = (metadata_t*) ((char*)ptr - sizeof(metadata_t));
  insertSorted(newfreed);
  // memory_coalesce(newfreed);
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
  // printf("realloc entered\n");

  if (!ptr) { 
    // printf("try to malloc\n");
    return malloc(size);
  } else if (size == 0) {
    // printf("freed\n");
		free(ptr);
    ptr = NULL;
		return NULL;
	}	else {
    // printf("else entered\n");
    
    metadata_t* block = (metadata_t*)ptr - 1;
    size_t block_size = block->size;
    
    void* new_ptr = malloc(size);
    if (new_ptr) {
      // printf("tries to copy\n");
      if (block_size >= size) {
        memcpy(new_ptr, ptr, size);
      } else {
        memcpy(new_ptr, ptr, block_size);
      }
      // printf("copy finished\n");
      free(ptr);
      ptr = NULL;
    }
    return new_ptr;

    // if (block_size > size) {
    //   size_t leftover = block_size - size;
    //   if (leftover >= sizeof(metadata_t)) {
    //     // printf("leftover size is %d\n",leftover);
    //     metadata_t* new = (metadata_t*) ((char*)ptr + size);
    //     new->size = leftover - sizeof(metadata_t);
    //     insertSorted(new);
    //     block->size = size;
    //     return ptr;
    // } else if (block_size == size) {
    //   return ptr;
    // } else {
    //   void * new_ptr = malloc(size);
    //   if (new_ptr) {
    //     memcpy(new_ptr,ptr,block_size);
    //   }
    //   free(ptr);
    //   return new_ptr;
    // }
    // }
  }
  
}