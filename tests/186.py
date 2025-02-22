from __future__ import print_function

# print space or not
print('aha	',)
print('hum')

# minus in front
print(hex(200), hex(-200))

# import as
import random
from random import random as randum
from string import *
random.seed(1)
#print a
rrr = randum()
print(0 <= rrr <= 1)

#default argument problem
import getopt
flats = getopt.getopt(['a'],'a')
print(flats)

#more casting problems..
def hop():
    yield ()
    yield (1,2)
h = hop()
print(list(h))
print(list(h))
def hap():
    return ()
    return (1,2)
print(hap())

#FOR_IN_T2 for listcomp
#class animal:
#    def sound(self):
#        print 'oink'
#class dog(animal): pass
#class cat(animal): pass
#print [a.sound() for a in dog(), cat()]

#tests used for optimizing enumerate/zip
l = [(7,8),(9,10)]
class D: pass
y = D()
for a, b in enumerate(l):
    print(a,b)
for a, y.b in enumerate(l):
    print(a,y.b)
class C: pass
x = C()
for x.a, (c,d) in enumerate(l):
    print(x.a,c,d)
for t in enumerate(l):
    print(t)
print([(a, b) for a, b in enumerate(l)])
print([(a, y.b) for a, y.b in enumerate(l)])
print([(x.a, (c,d)) for x.a, (c,d) in enumerate(l)])
print([t for t in enumerate(l)])

l2 = [(7,8), (9,10)]
for a2, b2 in zip(l2, l2[::-1]+l2):
    print(a2, b2)
for t2 in zip(l2, l2):
    print(t2)
class C2: pass
c2 = C2()
for c2.x, (d2,e2) in zip(2*l2, l2):
    print(c2.x, d2, e2)
for (d2,e2), c2.x in zip(2*l2, l2):
    print(d2, e2, c2.x)
for (d2,e2), (f2,g2) in zip(l2, l2):
    print(d2, e2, f2, g2)
print(list(zip('hoi','hap')), list(zip('ah', 'bh', 'ch')))
for u2,v2 in zip('hoi','hap'):
    print(u2+v2)
print(list(zip('ahoi', range(5))))
print([((d2, e2), c2.x) for (d2,e2), c2.x in zip(2*l2, l2)])

#pyseq:str special cases
print('a'.join('hap'))
fi = open('testdata/humba', 'w')
fi.writelines('hoei')
fi.close()
print(open('testdata/humba').read())
print(tuple('hap'))
up = ['a']
up.extend('hoi')
print(up)
up += 'wap'
print(up)
print(min('gehakt'), max('gehakt'))
print(list(reversed('gehakt')))

#variable naming
#def sentences():
#    next = 12
#    yield next
#print list(sentences())

#defdict problem
import collections
hoppa = collections.defaultdict(int)
hoppa[4] = 5
for xxx in hoppa:
    print(xxx, hoppa[xxx])


#sum ints with double
items2 = range(5)
ork = sum(items2, 0.3)
print(ork)


