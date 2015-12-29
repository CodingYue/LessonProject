# -*- coding: utf-8 -*-

import os
import socket

class Interaction(object):
	def __init__(self, client):
		self.client = client
		self.pageSize = 1000
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
			if op == 0x02: self.nextPage();
			if op == 0x03: self.previousPage();
			if op == 0x04: self.goToPage(info);
			if op == 0x05: self.listBookmark();
			if op == 0x06: self.addOrDeleteBookmark();
			if op == 0x08: break;
			if op == 0x09: self.getCurPage();

	def listBooks(self):
		bookList = []
		for book in os.listdir(os.getcwd()+"/books"):
			bookList.append(book)

		self.client.send(" ".join(bookList))

	def openBook(self, book):
		FILE = open("books/" + book, "r")
		text = FILE.readlines()

		lineSize = 80

		newText = []
		for line in text:
			tmp = line.decode("utf-8")
			while len(tmp) >= lineSize:
				newText.append(tmp[:lineSize].encode("utf-8"))
				tmp = tmp[lineSize:]
			if len(tmp) > 0:
				newText.append(tmp.encode("utf-8"))

		print "OK"

		text = newText

		lineEachPage = 25
		total = 0
		totalLine = len(text)
		self.pageList = []
		while total < totalLine:

			self.pageList.append("".join(text[total:min(totalLine, total+lineEachPage)]))
			total += lineEachPage
			self.pageHasBookmark.append(0)

		self.curPage = 0
		self.client.send(self.pageList[0])

		#print self.pageList[0]

		return self.pageList

	def nextPage(self):

		if self.curPage != len(self.pageList)-1:
			self.curPage += 1

		self.client.send(self.pageList[self.curPage])

	def previousPage(self):
		if self.curPage != 0:
			self.curPage -= 1

		self.client.send(self.pageList[self.curPage])

	def goToPage(self, info):
		self.curPage = int(info)
		self.client.send(self.pageList[self.curPage])

	def listBookmark(self):
		bookmarkStr = " ".join([str(page) for page in xrange(len(self.pageHasBookmark)) if self.pageHasBookmark[page] == 1])
		if bookmarkStr == "":
			bookmarkStr = " "
		self.client.send(bookmarkStr)
		print bookmarkStr

	def getCurPage(self):
		self.client.send(self.curPage)

	def addOrDeleteBookmark(self):
		self.pageHasBookmark[self.curPage] ^= 1

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
