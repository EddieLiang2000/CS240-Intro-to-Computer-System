def reduce(left, right):
    for key in left:
        values1 = left[key]
        if (key in right):
            values2 = right[key]
            common = []
            for value in values1:
                if value not in values2:
                    values2.append(value)
            # right[key] = common
        elif (key not in right):
            right[key] = values1
    return right

# def reduce(left, right):
#     for key in left:
#         values1 = left[key]
#         if (key in right):
#             values2 = right[key]
#             common = []
#             for value in values1:
#                 if value in values2:
#                     common.append(value)
#             right[key] = common
#         elif (key not in right):
#             right[key] = values1
#     return right

