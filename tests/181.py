from __future__ import print_function

#time.strptime
import time
print(time.strftime("%d %b %Y %H:%M:%S", time.strptime("2001-11-12 18:31:01", "%Y-%m-%d %H:%M:%S")))
print(time.strftime("%Y", time.strptime("2001", "%Y")))

#improve default arguments
import testdata.bert181 as bert
print(bert.def1())

print(bert.def2())
bert.a = 16
print(bert.huh())
print(bert.def2())

print(bert.def3())

def bleh(l=[1,2]):
    return l
print(bleh())

bert.def4()

#C++ bool type
def h(x):
    if x in ['False', '0']: return 0
    elif x in ['True', '1']: return 1
    else: return 2

print(hex(1==2), hex(1!=2))
print(abs(1==2), abs(1!=2))
print(h(str(1==2)), h(str(1!=2)))
print(h(repr(1==2)), h(repr(1!=2)))
print(int(1==2), int(1!=2))
print(float(1==2), float(1!=2))
print(ord(chr(1==2)), ord(chr(1!=2)))

#random.sample/choice
import random
print(random.sample(range(1), 1))
print(random.sample(sorted(set([1])), 1))

#fast_for_neg in listcomp_for
print([(i, i) for i in range(29, -1, -1)])

#works, but add as test
def ah():
   pass
def bh(func=ah):
   func()
bh()

# tempvars/new nodes and inheritance (XXX add more here)
class network:
    def shortestpath(self):
        for node in set([1]):
            print(node)

        print([node for node in [1]])

class smallworld(network):
    pass

s = smallworld()
s.shortestpath()

# ss-progs regression
class LowLevel:
   def bslTxRx(self, blkout=None):
       pass

class BootStrapLoader(LowLevel):
   def actionRun(self):
       self.bslTxRx()

bsl = BootStrapLoader()
bsl.actionRun()

# test compilation
import socket
import stat

# test all cases
a = 1

print([x for x in range(1,10,1)])
print([x for x in range(10,1,-1)])
print([x for x in range(1,10,+1)])
print([x for x in range(1,10,a)])
print([x for x in range(10,1,-a)])
print([x for x in range(1,10,+a)])
print([x for x in range(1,10,a*1)])
print([x for x in range(1,10,-(-1))])
print([x for x in range(1,10,+(+a))])

for x in range(1,10,1): print(x,)
print()
for x in range(1,10,+1): print(x,)
print()
for x in range(1,10,a): print(x,)
print()
for x in range(1,10,+a): print(x,)
print()
for x in range(1,10,-(-1)): print(x,)
print()
for x in range(1,10,+(+1)): print(x,)
print()
for x in range(1,10,+(+a)): print(x,)
print()
for x in range(10,1,-1): print(x,)
print()
for x in range(10,1,-a): print(x,)
print()
