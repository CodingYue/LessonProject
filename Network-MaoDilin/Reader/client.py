# -*- coding: utf-8 -*-

import socket


class Client(object):


	'''
		Four bytes string message
		First byte show what kind of operation.

		0x00 Open Book / List Books.
		0x01 Next / Previous Page
		0x02 Next / Previous Chapter
		0x03 Go to page
		0x04 List bookmark / xor bookmark.
		0x05 server will continue sending books' name.
		0x06 shutdown.
	'''

	def __init__(self):

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect(("127.0.0.1", 2333))

	def listBook(self):
		self.sock.send("00")
		bookstr = self.sock.recv(2048).split()
		print bookstr

	def openBook(self, book):
		self.sock.send("01" + book)
		self.curPage = self.sock.recv(2048)
		self.text = self.sock.recv(2048)
		print self.text

	def nextPage(self):
		self.sock.send("02"+str(self.curPage))
		self.curPage = self.sock.recv(2048)
		self.text = self.sock.recv(2048)
		print self.text

	def previousPage(self):
		self.sock.send("03"+str(self.curPage))
		self.curPage = self.sock.recv(2048)
		self.text = self.sock.recv(2048)
		print self.text

	def toPage(self, number):
		self.text = self.sock.sned("04" + str(number))
		self.curPage = self.sock.recv(2048)
		self.text = self.sock.recv(2048)
		print self.text

	def shutdown(self):
		self.sock.send("06")
		self.sock.close()

if __name__ == "__main__":

	client = Client()
	client.listBook()
	client.openBook("a.txt")
	client.nextPage()
	
	
