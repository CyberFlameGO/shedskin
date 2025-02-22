from __future__ import print_function

a = frozenset([1])
d = a & a
d = a | a
d = a - a
d = a ^ a
print(sorted(a), sorted(d))

c = set([1,2])
e = set([])
f = set()
print(sorted(c), sorted(e), sorted(f))

g = frozenset([1])
h = {}
h[g] = 4
print(h[g])
h[frozenset([3,2,1])] = 5
del h[frozenset([1])]
for x in h:
    print(sorted(x), h[x])

try:
    {set([1]): 1}
except TypeError as m:
    print(m)

z,y  = [(1,2),(3,), (4,5,6)], [(3,),(4,5,6),(1,2)]
v, w = frozenset(z), frozenset(y)
print('eq', [0, 1][v == w])
print('hash', [0, 1][hash(v) == hash(w)])

k = set([1])
k = frozenset([2])


