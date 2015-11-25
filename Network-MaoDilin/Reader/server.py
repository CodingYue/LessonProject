import socket


class Server(object):

	def __init__(self):

		self.port = 2333

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.bind(('', self.port))
		self.sock.listen(5)

	def run(self):

		while 1:
			(clientsocket, address) = self.sock.accept()
			self.interaction(clientsocket)

	def interaction(self, client):

		while 1:
			print client.recv(1024)

if __name__ == "__main__":
	server = Server()
	server.run()
