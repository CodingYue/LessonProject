
import socket


class Client(object):

	def __init__(self):

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect(("127.0.0.1", 2333))


if __name__ == "__main__":

	client = Client()
	print client.sock.recv(1024)
	client.sock.send("Hello server!")
