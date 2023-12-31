# reduce function:
def reduce(left, right):
  for key in left:
    if (key in right):
      right[key] += left[key]
    elif (key not in right):
      right[key] = left[key]
  return right
