def check(a):
	for i in range(len(a)):
		for j in range(len(a)):
			if a[i] == 0 and a[j] == 1:
				return 0
			if a[i] == 2 and a[j] == 3:
				return 0

	print a

	return 1

ans = 0

for a in range(4):
	for b in range(4):
		for c in range(4):
			for d in range(4):
				for e in range(4):
					ans += check([a, b, c, d, e])

print ans