import random

class Client:
	def __init__(self, e, N):
		self.e = e
		self.N = N
	
	def encrypt(self, m):
		return pow(m, self.e, self.N)

class Server:

	def __init__(self, length):

		'''
			RSA Server
		'''

		self.P = self.genPrime(length)
		self.Q = self.genPrime(length)
		self.phiN = (self.P-1)*(self.Q-1)
		self.N = self.P * self.Q
		self.e = 65537
		self.d = self.gcd(self.e, self.phiN)[1]
		self.d = ((self.d % self.phiN) + self.phiN) % self.phiN


		assert(self.phiN % self.e != 0)
		assert(self.d * self.e % self.phiN == 1)

	def gcd(self, a, b):
		'''
			extended greatest command divisor
		'''
		if b == 0:
			return [a, 1, 0]
		ret = self.gcd(b, a % b)
		tmp = ret[1]
		ret[1] = ret[2]
		ret[2] = tmp - a/b*ret[2]
		return ret

	def MillerRabin(self, a, n):
		if n == 2: return True
		if n == 1 or (n & 1) == 0: return False
		d = n-1
		while (d & 1) == 0: d >>= 1
		t = pow(a, d, n)
		while d != n-1 and t != 1 and t != n-1:
			t = t * t % n
			d <<= 1
		return t == n-1 or ((d & 1) == 1)

	def isPrime(self, n):
		testPrimeList = [2, 3, 5, 7, 11, 13, 17, 19, 23]
		result = True
		for prime in testPrimeList:
			if prime >= n:
				break
			result = result and self.MillerRabin(prime, n)
			if not result:
				break
		return result

	def genPrime(self, len) :

		begin = random.randint(2**(len-1), 2**len-1)
		
		while not self.isPrime(begin):
			begin += 1
		
		return begin

	def decrypt(self, c):
		return pow(c, self.d, self.N)



if __name__ == "__main__":
	server = Server(64)
	client = Client(server.e, server.N)

	key = random.randint(0, 2**64-1)
	print key

	
	c = client.encrypt(key)
	m = server.decrypt(c)

	print key
	print c
	print m


