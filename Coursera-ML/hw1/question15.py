import random
from numpy import sign




def solve(VX):

	i = 0
	n = len(VX)
	w = [0.0 for _ in range(5)]

	#print VX[6]

	ans = 0


	while True :
		OK = True

		for times in xrange(n):
			
			dotsum = 0.0
			for j in range(5):
				dotsum += w[j] * VX[i][j]
			sig = 0
			if dotsum == 0: sig = -1.
			else:
				sig = sign(dotsum)

			if sig != VX[i][5]: OK = False

			if not OK : 
				break

			i = (i+1)%n

		if OK: break

		for j in range(5):
			w[j] += VX[i][j] * VX[i][5] * 0.5
		i=(i+1)%n
		#print w
		ans += 1

	return ans

FILE = open("ntumlone-hw1-hw1_15_train.dat.txt", "r")
VX = []

for line in FILE :
	t = map(float, line.split(" "))
	x = [1]
	for p in t:
		x.append(p)
	VX.append(x)

sum = 0

for times in range(2000):
	random.seed(None)
	random.shuffle(VX)
	sum += solve(VX)
print sum / 2000.