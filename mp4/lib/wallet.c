#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wallet.h"
wallet_resource *last = NULL;

/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Implement
  pthread_mutex_init(&wallet->lock, NULL);
  wallet->head = NULL;
}

/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  // Implement this
  if (wallet->head == NULL) {
    return 0;
  } else {
    wallet_resource *chosen = NULL;
    for (chosen = wallet->head; chosen != NULL; chosen = chosen->next) {
      if (strcmp(chosen->resource_name, resource) == 0) {
        return chosen->amount;
      }
    }
    return 0;
  }
}

/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  // Implement this
  pthread_mutex_lock(&wallet->lock);
  if (wallet->head == NULL) {
    wallet->head = malloc(sizeof(wallet_resource));
    wallet->head->resource_name = resource;
    wallet->head->amount = 0;
    pthread_cond_init(&wallet->head->cond, NULL);
    last = wallet->head;
    if (delta >= 0) {
      wallet->head->amount += delta;
    }
    if (delta < 0) {
      while(wallet->head->amount + delta < 0) {
        pthread_cond_wait(&wallet->head->cond, &wallet->lock);
      }
      wallet->head->amount += delta;
    }
    pthread_cond_broadcast(&wallet->head->cond);
  } else {
    wallet_resource *chosen = NULL;
    for (chosen = wallet->head; chosen != NULL; chosen = chosen->next) {
      if (strcmp(chosen->resource_name, resource) == 0 ) {
        if (delta >= 0) {
          chosen->amount += delta;
        }
        if (delta < 0) {
          while(chosen->amount + delta < 0) {
            pthread_cond_wait(&chosen->cond, &wallet->lock);
          }
          chosen->amount += delta;
        }
        pthread_cond_broadcast(&chosen->cond);
        break;
      }
    }
    if(chosen == NULL) {
      wallet_resource *new = malloc(sizeof(wallet_resource));
      pthread_cond_init(&new->cond, NULL);
      new->resource_name = resource;
      new->amount = 0;
      last->next = new;
      last = new;
      if (delta >= 0) {
        new->amount += delta;
      }
      if (delta < 0) {
        while(new->amount + delta < 0) {
          pthread_cond_wait(&new->cond, &wallet->lock);
        }
        new->amount += delta;
      }
      pthread_cond_broadcast(&new->cond);
    }
  }
  pthread_mutex_unlock(&wallet->lock);
}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement this
  pthread_mutex_destroy(&wallet->lock);
  wallet_resource *chosen = NULL;
  for (chosen = wallet->head; chosen != NULL; chosen = chosen->next) {
    pthread_cond_destroy(&chosen->cond);
  }
}