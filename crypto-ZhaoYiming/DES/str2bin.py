
def func(s):
	res=""
	for x in s:
		t=str(bin(ord(x)))
		t = t[2:len(t)]
		for j in xrange(8-len(t)):
			res += '0'
		res += t

	return res;

print func(raw_input())
