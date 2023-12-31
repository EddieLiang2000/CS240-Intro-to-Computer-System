# map function:
def map(filename):
  map = {}
  with open(filename, "r", encoding="utf-8") as f:
    for line in f.readlines():
      for character in line:
        if (character.isalpha()):
          real_letter = character.lower()
          if (real_letter not in map):
            map[real_letter] = 1
          elif (real_letter in map):
            map[real_letter] += 1
        else:
          continue
  return map