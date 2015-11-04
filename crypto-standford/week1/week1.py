def strxor(a, b):     # xor two strings of different lengths

	print a
	print b

	if len(a) > len(b):
		return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a[:len(b)], b)])
	else:
		return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a, b[:len(a)])])


FILE = open("week1.in","r")

cipher = []

for i in xrange(10):

	cipher.append(FILE.readline().strip().decode('hex'))

cipher.append(cipher[0]);


fuck = []
for i in xrange(10):
	fuck.append(strxor(cipher[i], cipher[i+1]))


print len(fuck[0])

key = [-1 for _ in xrange(1024)]
key[3] = ord(cipher[1][3]) ^ ord('e')
key[7] = ord(cipher[4][7]) ^ ord("'")
key[9] = ord(cipher[3][9]) ^ ord("r")
key[10] = ord(cipher[0][10]) ^ ord('t')
key[14] = ord(cipher[5][14]) ^ ord('t')
key[18] = ord(cipher[1][18]) ^ ord('l')
key[19] = ord(cipher[2][19]) ^ ord('t')
key[22] = ord(cipher[0][22]) ^ ord('e')
key[23] = ord(cipher[0][23]) ^ ord('r')
key[25] = ord(cipher[1][25]) ^ ord('y')
key[26] = ord(cipher[6][26]) ^ ord('t')
key[28] = ord(cipher[7][28]) ^ ord('h')
key[31] = ord(cipher[5][31]) ^ ord('a')
key[32] = ord(cipher[5][32]) ^ ord('p')
key[35] = ord(cipher[3][35]) ^ ord('n')
key[36] = ord(cipher[3][36]) ^ ord('c')
key[40] = ord(cipher[3][40]) ^ ord("t")
key[41] = ord(cipher[1][41]) ^ ord('h')
key[44] = ord(cipher[1][44]) ^ ord('r')
key[56] = ord(cipher[0][56]) ^ ord('a')
key[63] = ord(cipher[0][63]) ^ ord(' ')
key[70] = ord(cipher[0][70]) ^ ord(' ')

fuck.append(fuck[0])

for i in range(10):
	for j in range(min(len(fuck[i]), len(fuck[i+1]))):
		if fuck[i][j].isalpha() and fuck[i+1][j].isalpha():
			if key[j] != -1:
				continue
			key[j] = ord(cipher[i+1][j]) ^ 32

for i in xrange(10):
	tmp = ""
	for j in xrange(len(cipher[i])):

		if key[j] != -1:
			tmp += chr(ord(cipher[i][j]) ^ key[j])
		else:
			tmp += ';'
	print tmp

target = FILE.readline().strip().decode('hex')

mykey = "".join([chr(_) for _ in key[0:83]])
#print myke#y


print strxor(target, mykey)

			