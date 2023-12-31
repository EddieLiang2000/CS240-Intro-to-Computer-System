def map(filename):
    map = {}
    with open(filename, "r", encoding="utf-8") as f:
        words = []
        bigrams = []
        for line in f.readlines():
            for character in line:
                if (character.isalpha()):
                    real_letter = character.lower()
                    words.append(real_letter)
                else:
                    if (len(words) > 0):
                        word = "".join(words)
                        bigrams.append(word)
                        if (len(bigrams) == 2):
                            key = "(" + bigrams[0] + " " + bigrams[1] + ")"
                            if (key in map):
                                map[key] += 1
                            elif (key not in map):
                                map[key] = 1
                            bigrams.pop(0)
                        words.clear()
                    elif (len(words) == 0):
                        continue
    return map