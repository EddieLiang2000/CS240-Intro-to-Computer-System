#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "http.h"


// 1. explicitly assign ptrs to NULL
// 2. check != NULL
void insert(dict* node, HTTPRequest* req) {
  if (req->head == NULL) {
    req->head = req->tail = node;
  } else {
    req->tail->next = node;
    req->tail = node;
  }
}

/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  req->head = NULL;
  req->tail = NULL;
  req->payload = NULL;
  int size = 0;
  char* firstline = NULL;
  for (char *line = strtok(buffer,"\r\n"); line != NULL ; line = strtok(NULL, "\r\n")) {
    printf("The line is %s\n",line);
    printf("The line length is %d\n",strlen(line));

    if(firstline == NULL) {
      firstline = (char*) malloc(strlen(line)+1);
      memmove(firstline, line, strlen(line));
    }
    
    size += 1;
    if (size != 1) {
      char* key = NULL;
      char* value = NULL;
      key = strtok_r(line, " ", &value);
      key[strlen(key)-1] = '\0';

      dict* newone = (dict*) malloc(sizeof(dict));
      newone->next = NULL;
      newone->key = NULL;
      newone->value = NULL;
      newone->key = key;
      newone->value = value;
      // printf("The key is %s and the value is %s", newone->key, newone->value);
      insert(newone, req);
      // printf("The key is %s and the value is %s", newone->key, newone->value);
      
      if (strcmp(key,"Content-Length") == 0) {
        char* last = strtok(NULL, "\r\n");
        req->payload = last;
        printf("The last line is %s",last);
        break;
      }
      printf("%s\n", "start here");
    }
  }

  printf("%s\n", "reach here");
  req->action = NULL;
  req->path = NULL;
  req->version = NULL;
  
  for (char *token = strtok(firstline," "); token != NULL; token = strtok(NULL, " ")) {
    if (req->action == NULL) {
      req->action = token;
    } else if (req->path == NULL) {
      req->path = token;
    } else if (req->version == NULL) {
      req->version = token;
    }
  }
  
  return 0;
}

/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  int size = 500;
  char* buffer = malloc(size*sizeof(char)); 
  ssize_t result = read(sockfd, buffer, 500);
  int bytes_read = result;
  while (result == 500) {
    buffer = realloc(buffer, 500+size);
    ssize_t result = read(sockfd, buffer+size, 500);
    bytes_read += result;
    size += 500;
  }
  httprequest_parse_headers(req, buffer, bytes_read);
  dict* cur = req->head;
  char* saveptr;
  while (cur) {
    if (strcmp(cur->key, "Content-Length") == 0) {
      char* split1 = strtok_r(buffer, "\r\n\r\n", &saveptr);
      req->payload = malloc(atoi(cur->value));
      memcpy(req->payload, buffer + bytes_read - atoi(cur->value), atoi(cur->value));
      return bytes_read;
    }
    cur = cur->next;
  }
  req->payload = NULL;
  return 0;
}


/**
 * httprequest_et_action
 * 
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req) {
  return req->action;
}


/**
 * httprequest_get_action
 * 
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  dict* chosen = NULL;
  for (chosen = req->head; chosen != NULL; chosen = chosen->next) {
    if (strcmp(chosen->key, key) == 0) {
      return chosen->value;
    }
  }
  return NULL;
}


/**
 * httprequest_get_action
 * 
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req) {
  return req->path;
}


/**
 * httprequest_get_action
 * 
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req) {
  free((char*)req->action);
  free((char*)req->path);
  free((char*)req->version);
  free((char*)req->payload);
  dict* chosen = NULL;
  for (chosen = req->head; chosen != NULL; chosen = chosen->next) {
    free(chosen->key);
    free(chosen->value);
  }
  free(req->head);
  free(req);
}