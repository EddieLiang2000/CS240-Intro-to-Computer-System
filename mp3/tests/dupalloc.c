/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
  struct _metadata_t* freenext_;
  struct _metadata_t* freeprev_;
} metadata_t;

metadata_t *freehead = NULL;
metadata_t *freetail = NULL;

void insertSorted(metadata_t * head, metadata_t * insert) {
  if (!head) {
    freehead = insert;
    freetail = insert;
    return;
  }

  if (insert < head) {
    insert->freenext_ = head;
    head->freeprev_ = insert;
    freehead = insert;
    freetail = head;
    return;
  }

  metadata_t * next = head->freenext_;
  while(head) {
    if (!next) {
      head->freenext_ = insert;
      insert->freeprev_ = head;
      freetail = insert;
      break;
    } else if (insert > head && insert < next) {
      head->freenext_ = insert;
      insert->freenext_ = next;
      insert->freeprev_ = head;
      next->freenext_ = insert;
      break;
    }
    head = next;
    next = next->freenext_;
  }
  insert->isUsed = 0;
}

void removeSorted(metadata_t* remove) {
  metadata_t* next = remove->freenext_;
  metadata_t* prev = remove->freeprev_;
  if (!next && !prev) {
    freehead = freetail = NULL;
  } else if (!next) {
    prev->freenext_ = NULL;
    remove->freeprev_ = NULL;
    freetail = prev;
  } else {
    remove->freenext_ = NULL;
    next->freeprev_ = NULL;
    freehead = next;
  }
  remove->isUsed = 1;
}

metadata_t * search(metadata_t * head, metadata_t * tail, size_t size) {
  if (head) {
    if (head->size >= size && tail->size >= size) {
      if (head->size - size <= tail->size - size) {
        return head;
      } else {
        return tail;
      }
    } else if (head->size >= size) {
      return head;
    } else if (tail->size >= size) {
      return tail;
    } else {
      head = head->freenext_;
      tail = tail->freeprev_;
    }
  }
  return NULL;
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
  metadata_t* free_block = search(freehead,freetail,size);
  if (!free_block) {
    metadata_t *new = sbrk(sizeof(metadata_t));
    new->size = size;
    new->isUsed = 1;
    void *ptr = sbrk(size);
    return ptr;
  } else if (size == free_block->size) {
    removeSorted(free_block);
    void * ptr = free_block + sizeof(metadata_t);
    return ptr;
  } else {
    void * ptr = free_block + sizeof(metadata_t);
    size_t leftover = free_block->size - size;
    if (leftover >= sizeof(metadata_t)) {
      metadata_t* new = ptr + size;
      new->size = leftover - sizeof(metadata_t);
      new->isUsed = 0;
      insertSorted(freehead, new);
    }
    return ptr;
  }
    // metadata_t *chosen = NULL;
    // for (chosen = freehead; chosen != NULL; chosen = chosen->freenext) {
    //   if (chosen->size == size) {
    //     void *ptr = (void*) chosen + sizeof(metadata_t);
    //     chosen->isUsed = 1;
    //     if (chosen == freehead) {
    //       if (!chosen->freenext) {
    //         freehead = NULL;
    //       } else {
    //         freehead = chosen->freenext;
    //         freehead->freeprev = NULL;
    //       }
    //     } else if (!chosen->freenext && chosen->freeprev) {
    //       chosen->freeprev->freenext = NULL;
    //     } else {
    //       chosen->freeprev->freenext = chosen->freenext;
    //       chosen->freenext->freeprev = chosen->freeprev;
    //     } 
    //     return ptr;
    //   } else if (chosen->size > size) {
    //     // blocksplitting
    //     chosen->size = size;
    //     chosen->isUsed = 1;
    //     metadata_t *add_in = (void*) chosen + size + sizeof(metadata_t);
    //     add_in->size = chosen->size - size - sizeof(metadata_t);
    //     add_in->isUsed = 0;
    //     void *ptr = (void*) chosen + sizeof(metadata_t);
    //     if (chosen == freehead) {
    //       if (!chosen->freenext) {
    //         freehead = add_in;
    //       } else {
    //         freehead = add_in;
    //         add_in->freenext = chosen->freenext;
    //         chosen->freenext->freeprev = add_in;
    //       }
    //     } else if (!chosen->freenext && chosen->freeprev) {
    //       add_in->freeprev = chosen->freeprev;
    //       chosen->freeprev->freenext = add_in;
    //     } else {
    //       add_in->freenext = chosen->freenext;
    //       chosen->freenext->freeprev = add_in;
    //       add_in->freeprev = chosen->freeprev;
    //       chosen->freeprev->freenext = add_in;
    //     }
    //     return ptr;
    //   }
    }
//     metadata_t *new = sbrk(sizeof(metadata_t));
//     new->size = size;
//     new->isUsed = 1;
//     void *ptr = sbrk(size);
//     return ptr;
//   }
//   return NULL;
// }

void memory_coalesce(metadata_t* newlyfreed) {
  metadata_t * next = newlyfreed->freenext_;
  metadata_t * prev = newlyfreed->freeprev_;
  if (prev && ((prev + sizeof(metadata_t) + prev->size) == newlyfreed)) {
    prev->size += (sizeof(metadata_t) + newlyfreed->size);
    prev->freenext_ = next;
    if (next) {
      next->freeprev_ = prev;
    }
    newlyfreed->freenext_ = NULL;
    newlyfreed->freeprev_ = NULL;
    newlyfreed = NULL;
    memory_coalesce(prev);
  } else if (next && ((newlyfreed + sizeof(metadata_t) + newlyfreed->size) == next)) {
    newlyfreed->size += (sizeof(metadata_t) + next->size);
    newlyfreed->freeprev_ = prev;
    if (prev) {
      prev->freenext_ = next;
    }
    next->freenext_ = NULL;
    next->freeprev_ = NULL;
    next = NULL;
    memory_coalesce(newlyfreed);
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
void free(void *ptr) {
  if (!ptr) {
    return;
  }
  metadata_t *newfreed = ptr - sizeof(metadata_t);
  // newfreed->isUsed = 0;
  // if (freehead == NULL) {
  //   freehead = newfreed;
  //   freecur = freehead;
  //   newfreed->isUsed = 0;
  // }
  insertSorted(freehead,newfreed);
  memory_coalesce(newfreed);
  // memory coalescing
  // metadata_t *search = NULL;
  // for (search = freehead; search != NULL; search = search->freenext) {
  //   if (search + sizeof(metadata_t) + search->size == newfreed) {
  //     search->size += sizeof(metadata_t) + newfreed->size;
  //     search->isUsed = 0;
  //   }
  //   if (search - sizeof(metadata_t) - newfreed->size == newfreed) {
  //     newfreed->size += sizeof(metadata_t) + search->size;
  //     if (search == freehead) {
  //       if (!search->freenext) {
  //         freehead = newfreed;
          
  //       } else {
  //         freehead = newfreed;
  //         newfreed->freenext = search->freenext;
  //         search->freenext->freeprev = newfreed;
          
  //       }
  //     } else if (!search->freenext) {
  //       search->freeprev->freenext = newfreed;
  //       newfreed->freeprev = search->freeprev;
      
  //     } else {
  //       newfreed->freeprev = search->freeprev;
  //       newfreed->freenext = search->freenext;
  //       search->freenext->freeprev = newfreed;
  //       search->freeprev->freenext = newfreed;
  //     }
  //     newfreed->isUsed = 0;
  //     return;
  //   }
  // }
  // freecur->freenext = newfreed;
  // newfreed->freeprev = freecur;
  // newfreed->freenext = NULL;
  // freecur = newfreed;
  // newfreed->isUsed = 0;
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
  if (ptr == NULL) { 
    return malloc(size);
  }
	if (size == 0) {
		free(ptr);
		return NULL;
	}	

	metadata_t* block = ptr;
	// block = block - 1;
	// if (block->size >= size) {
  //   block->size
  //   return ptr;
  // }
	void* new_ptr = malloc(size);
	if (new_ptr == NULL) {
    return NULL;
  }
	memcpy(new_ptr, ptr, block->size);
	free(ptr);
	return new_ptr;
}