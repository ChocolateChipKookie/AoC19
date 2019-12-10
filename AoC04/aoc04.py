min_ = 130254
max_ = 678275
print("First = ", len([1 for x in range(min_, max_) if len(set(str(x))) < 6 and str(x) == "".join(sorted(str(x)))]))
print("Second = ", len([1 for x in range(min_, max_) if 2 in {i: str(x).count(i) for i in set(str(x))}.values() and str(x) == "".join(sorted(str(x)))]))
