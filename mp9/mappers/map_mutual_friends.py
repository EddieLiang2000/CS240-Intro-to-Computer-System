def map(filename):
    
    map = {}
    mutual_friends = {}
    with open(filename, "r", encoding="utf-8") as f:
        for line in f.readlines():
            pairs = []
            for character in line:
                if (character.isalpha()):
                    pairs.append(character)
                else:
                    continue
            map[pairs[0]] = pairs[1:]
    

    # ks = list(map)
    # for j in range(len(ks)):
    #     values = []
    #     values = map[ks[j]]
    #     for i in range(len(values)):
    #         if (ks[j] > values[i]):
    #             keys = "(" + values[i] + " " + ks[j] + ")"
    #         else:
    #             keys = "(" + ks[j] + " " + values[i]  + ")"
    #         value = values.copy()
    #         value.pop(i)
    #         mutual_friends[keys] = value
    # return mutual_friends
    
    ks = list(map.keys())
    for i in range(len(ks)):
        values = []
        values = map[ks[i]]
        for j in range(len(values)):
            if ((j+1) < len(values)):
                for k in range((j+1), len(values)):
                    keys = ""
                    if (values[j] > values[k]):
                        keys = "(" + values[k] + " " + values[j] + ")"
                        
                    else:
                        keys = "(" + values[j] + " " + values[k] + ")"
                    output = []
                    output.append(ks[i])
                    if (keys not in mutual_friends):
                    # print(ks[i])
                        mutual_friends[keys] = output
                    elif (keys in mutual_friends):
                        mutual_friends[keys].append(ks[i])
                    
    return mutual_friends
