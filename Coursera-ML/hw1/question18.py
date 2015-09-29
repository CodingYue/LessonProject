import random
from random import randint

def sign(x):
	if x <= 0: return -1
	return 1

def check(w, xn):
	dotsum = 0
	for i in range(len(w)):
		dotsum += w[i] * xn[i]
	return sign(dotsum) == xn[5]

def count_error(w, VX, ec):
	res = 0
	for xn in VX :
		if not check(w, xn):
			res += 1
			if res >= ec:
				return -1
	return res

def compare(w, wp, VX, e1):
	e1 = count_error(w, VX)
	e2 = count_error(wp, VX)
	if e1 > e2: return wp
	return w

def PLA(VX, round):

	#print VX[0]

	n = len(VX)
	w = [0 for _ in xrange(5)]
	bestw = [0 for _ in xrange(5)]

	ec = count_error(bestw, VX, 100000)

	for times in xrange(round):
		which = -1
		for _ in xrange(50):
			x = randint(0, n-1)
			if check(w, VX[x]):
				continue
			which = x
			break
		if which == -1:
			continue

		for i in range(len(w)):
			w[i] += VX[which][5] * VX[which][i]
		
		tmp_ec = count_error(w, VX, ec)
		if tmp_ec != -1 and tmp_ec < ec:
			ec = tmp_ec
			bestw = w

	return bestw

training = open("ntumlone-hw1-hw1_18_train.dat.txt","r")

TrainX = []
for line in training:
	x = [1]
	for j in map(float, line.split(' ')):
		x.append(j)
	TrainX.append(x)

verify = open("ntumlone-hw1-hw1_18_test.dat.txt","r")

VerifyX = []

for line in verify:
	x = [1]
	for j in map(float, line.split(' ')):
		x.append(j)
	VerifyX.append(x)

rate = 0.0

for times in xrange(2000):
	print times
	random.seed(None)
	random.shuffle(TrainX)
	w = PLA(TrainX, 100)
	error = count_error(w, VerifyX, 100000)
	rate += float(error) / len(VerifyX)

print rate / 2000.0