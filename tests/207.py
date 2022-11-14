import random

# --- module-level functions
random.seed(37)
rstate = random.getstate()   # (state is not cross-compatible with CPython)
random.setstate(rstate)
for i in range(25):
    s = "%.8f" % random.random()
    f = random.randrange(-30,15)
    f = random.randrange(-15,15,3)
    f = random.randint(50,100)
    fibs = [0,1,1,2,3,5,8,13,21]
    print(fibs)
    i = random.choice(fibs)
    l = random.sample(fibs,3)
    random.shuffle(fibs)
    print(len(fibs))
    nums = [3.141, 2.71828, 1.41421, 1.0]
    print(nums)
    f = random.choice(nums)
    lf = random.sample(nums,3)
    random.shuffle(nums)
    print(len(nums))
    s = "%.8f" % random.uniform(-0.5,0.5)
    s = "%.8f" % random.normalvariate(0.0, 1.0)
    s = "%.8f" % random.lognormvariate(0.0, 1.0)
    s = "%.8f" % random.expovariate(1.0)
    s = "%.8f" % random.vonmisesvariate(0.0, 1.0)
    s = "%.8f" % random.gammavariate(20.0, 1.0)
    s = "%.8f" % random.gauss(0.0, 1.0)
    s = "%.8f" % random.betavariate(3.0, 3.0)
    s = "%.8f" % random.paretovariate(1.0)
    s = "%.8f" % random.weibullvariate(1.0, 1.0)
    #print "%.8f" % random.stdgamma(1.0,1.0,1.0,1.0) # deprecated in CPython
    #print "%.8f" % random.cunifvariate(0.0,1.0)     # deprecated in CPython
    b = random.getrandbits(8)
    b = random.getrandbits(16)
    b = random.getrandbits(30)
    print('')

# --- (test set for RNGs)
def runrng(r):
    s = "%.8f" % r.random()
    f = r.randrange(0,10)
    f = r.randrange(-10,10,2)
    i = r.randint(-5,5)
    fibs = [0,1,1,2,3,5,8,13,21]
    print(fibs)
    i = r.choice(fibs)
    l = r.sample(fibs,4)
    print(len(l))
    r.shuffle(fibs)
    print(len(fibs))
    nums = [3.141, 2.71828, 1.41421, 1.0]
    print(nums)
    f = random.choice(nums)
    lf = random.sample(nums,1)
    random.shuffle(nums)
    print(len(nums))
    s = "%.8f" % r.uniform(-0.5,0.5)
    s = "%.8f" % r.normalvariate(0.0, 1.0)
    s = "%.8f" % r.lognormvariate(0.0, 1.0)
    s = "%.8f" % r.expovariate(1.0)
    s = "%.8f" % r.vonmisesvariate(0.0, 1.0)
    s = "%.8f" % r.gammavariate(20.0, 1.0)
    s = "%.8f" % r.gauss(0.0, 1.0)
    s = "%.8f" % r.betavariate(3.0, 3.0)
    s = "%.8f" % r.paretovariate(1.0)
    s = "%.8f" % r.weibullvariate(1.0, 1.0)
    #print "%.8f" % r.stdgamma(1.0, 1.0, 1.0, 1.0) # deprecated in CPython
    #print "%.8f" % r.cunifvariate(0.0, 1.0)       # deprecated in CPython
    print('')

# --- random.Random (Mersenne Twister)
mt = random.Random()
mt.seed()
mt.seed(79)
mtstate = mt.getstate()   # (state is not cross-compatible with CPython)
mt.setstate(mtstate)
#mt.jumpahead(1000000)    # (not yet supported)
for i in range(25):
    runrng(mt)

# --- random.WichmannHill
#wh = random.WichmannHill()
#wh.seed()
#wh.seed(86)
#wh.whseed()
#wh.whseed(41)
#whstate = wh.getstate()   # (state is not cross-compatible with CPython)
#wh.setstate(whstate)
#wh.jumpahead(1000000)
#for i in range(25): runrng(wh)

# random module improvements
import random
random.seed(1)
f = random.triangular()
f = random.triangular(high=1.1, low=0.0)
f = random.triangular(0.1)
f = random.triangular(-2, 2)
f = random.triangular(-2.0, 2.1, 1.5)
f = random.triangular(mode=1.5)
f = random.triangular(0, 5, 0)
random.seed()
random.seed('seed')
random.seed(8.0)
random.seed(None)
random.seed(4)
f = random.random()
