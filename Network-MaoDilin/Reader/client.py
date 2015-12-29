# -*- coding: utf-8 -*-

import socket


class Client(object):

	'''
		Four bytes string message
		First byte show what kind of operation.

		0x00 List Books.
		0x01 Open Book
		0x02 Next Page
		0x03 Previous Page
		0x04 Go to page
		0x05 List bookmark
		0x06 xor bookmark.

		0x08 shutdown.
	'''

	def __init__(self):

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect(("127.0.0.1", 2333))

	def listBook(self):
		self.sock.send("00")
		self.bookList = self.sock.recv(65536).split()

	def openBook(self, book):
		self.sock.send("01" + book)
		self.text = self.sock.recv(65536)

	def nextPage(self):
		self.sock.send("02"+str(self.curPage))
		self.text = self.sock.recv(65536)
		#print self.text

	def previousPage(self):
		self.sock.send("03"+str(self.curPage))
		self.text = self.sock.recv(65536)
		#print self.text

	def toPage(self, number):
		self.sock.send("04" + str(number))
		self.text = self.sock.recv(65536)
		#print self.text

	def listBookmark(self):
		self.sock.send("05")
		self.bookmarkList = self.sock.recv(65536).split()

	def xorBookmark(self):
		self.sock.send("06"+str(self.curPage))

	def curPage(self):
		self.sock.send("09")
		return self.recv(65536)

	def shutdown(self):
		self.sock.send("08")
		self.sock.close()

if __name__ == "__main__":

	client = Client()
	client.listBook()
	client.openBook("神雕侠侣1.txt")
	client.nextPage()
