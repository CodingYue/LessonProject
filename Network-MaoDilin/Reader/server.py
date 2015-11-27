# -*- coding: utf-8 -*-

import os
import socket

class Interaction(object):
	def __init__(self, client):
		self.client = client
		self.pageSize = 200
		self.pageList = []
		self.pageHasBookmark = []

	def run(self):

		while 1:
			msg = self.client.recv(1024)
			if not msg: break

			op = int(msg[:2], 16)
			info = msg[2:]

		#	print op

			if op == 0x00: self.listBooks();
			if op == 0x01: self.openBook(info);
			if op == 0x02: self.nextPage(info);
			if op == 0x03: self.previousPage(info);
			if op == 0x04: self.goToPage(info);
			if op == 0x05: self.listBookmark();
			if op == 0x06: self.addOrDeleteBookmark(info);
			if op == 0x08: break;

	def listBooks(self):
		bookList = []
		for book in os.listdir(os.getcwd()+"/books"):
			bookList.append(book)

		self.client.send(" ".join(bookList))

	def openBook(self, book):
		FILE = open("books/" + book, "r")
		text = FILE.read().decode("utf-8")
	#	print text
		pageSize = 200

		total = 0
		textLen = len(text)
		self.pageList = []
		while total < textLen:
			self.pageList.append(text[total:min(textLen, total+200)].encode("utf-8"))
			total += 200
			self.pageHasBookmark.append(0)

		self.client.send("0")
		self.client.send(self.pageList[0])
		return self.pageList

	def nextPage(self, info):
		self.curPage = int(info)

		if self.curPage != len(self.pageList):
			self.curPage += 1
		self.client.send(str(self.curPage))
		self.client.send(self.pageList[self.curPage])

	def previousPage(self, info):
		self.curPage = int(info)
		if self.curPage != 0:
			self.curPage -= 1

		self.client.send(str(self.curPage))	
		self.client.send(self.pageList[self.curPage])

	def goToPage(self, info):
		self.curPage = int(info)
		self.client.send(str(self.curPage))
		self.client.send(self.pageList[self.curPage])
		
	def listBookmark(self):
		bookmarkStr = " ".join([str(page) for page in xrange(len(self.pageHasBookmark)) if self.pageHasBookmark[page] == 1])
		if bookmarkStr == "":
			bookmarkStr = "None"
		self.client.send(bookmarkStr)
		print bookmarkStr

	def addOrDeleteBookmark(self, info):
		pageNumber = int(info)
		self.pageHasBookmark[pageNumber] ^= 1

class Server(object):

	def __init__(self):

		self.port = 2333

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.bind(('', self.port))
		self.sock.listen(5)

	def run(self):

		while 1:
			(clientsocket, address) = self.sock.accept()
			interaction = Interaction(clientsocket)
			interaction.run()
if __name__ == "__main__":
	server = Server()
	server.run()
