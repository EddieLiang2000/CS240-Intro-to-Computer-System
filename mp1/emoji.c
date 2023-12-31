#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Return your favorite emoji; do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
const char *emoji_favorite() {
  return "\xF0\x9F\x98\x82";
}


// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(char *utf8str) {
  int count = 0;
  for (int i = 0; i < strlen(utf8str); i++) {
    if (utf8str[i] == '\xF0' && (i + 3) < strlen(utf8str)) {
      if ((utf8str[i+1] == '\x9F') 
      && ('\x80' <= utf8str[i+2] && utf8str[i+2] <= '\xA7') 
      && ('\x80' <= utf8str[i+3] && utf8str[i+3] <= '\xBF')) {
        count += 1;
      }
    }
  }
  return count;
}



// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-simpling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x8A') {
    utf8str[3] = '\x94';
  }
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x83') {
    utf8str[3] = '\x96';
  }
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x84') {
    utf8str[3] = '\x9E';
  }
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x86') {
    utf8str[3] = '\xA0';
  }
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x89') {
    utf8str[3] = '\xA1';
  }
  if (utf8str[0] == '\xF0' && utf8str[2] == '\x98'&& utf8str[3] == '\x8B') {
    utf8str[3] = '\xA2';
  }
}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {
  for (int i = 0; i < strlen(utf8str); i++) {
    if (utf8str[i] != '\xF0') {
      continue;
    } else if (utf8str[i] == '\xF0' && (i+3 < strlen(utf8str))) {
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x8A') {
        utf8str[i+3] = '\x94';
      }
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x83') {
        utf8str[i+3] = '\x96';
      }
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x84') {
        utf8str[i+3] = '\x9E';
      }
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x86') {
        utf8str[i+3] = '\xA0';
      }
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x89') {
        utf8str[i+3] = '\xA1';
      }
      if (utf8str[i+1] == '\x9F' && utf8str[i+2] == '\x98'&& utf8str[i+3] == '\x8B') {
        utf8str[i+3] = '\xA2';
      }
    }
  }
}


// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
  char *random_emoji = malloc(5*sizeof(char));
  int random = rand() % 39 + 128;
  int random2 = rand() % 63 + 128;
  random_emoji[0] = '\xF0';
  random_emoji[1] = '\x9F';
  random_emoji[2] = random;
  random_emoji[3] = random2;
  random_emoji[4] = '\0';
  return random_emoji;
}
