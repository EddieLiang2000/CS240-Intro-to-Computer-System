def map(filename):
    map = {}
    with open(filename, "r", encoding="utf-8") as f:
        word = []
        for line in f.readlines():
            for character in line:
                if (character.isalpha()):
                    real_letter = character.lower()
                    word.append(real_letter)
                else:
                    if (len(word) > 0):
                        key = "".join(word)
                        if (key in map):
                            map[key] += 1
                        else:
                            map[key] = 1
                        word.clear()
                    elif (len(word) == 0):
                        continue
        if (len(word) > 0):
            key = "".join(word)
            if (key in map):
                map[key] += 1
            else:
                map[key] = 1
    return map