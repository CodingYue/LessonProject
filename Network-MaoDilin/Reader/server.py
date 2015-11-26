# -*- coding: utf-8 -*-

import os
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

		pageList = []

		while 1:
			msg = client.recv(1024)
			if not msg: break

			op = int(msg[:2], 16)
			info = msg[2:]

		#	print op

			if op == 0x00: self.listBooks(client);
			if op == 0x01: pageList = self.openBook(client, info);
			if op == 0x02: self.nextPage(client, info, pageList);
			if op == 0x03: self.previousPage(client, info, pageList);
			if op == 0x04: self.goToPage(client, info, pageList);
			if op == 0x08: break;

	def listBooks(self, client):
		bookList = []
		for book in os.listdir(os.getcwd()+"/books"):
			bookList.append(book)

		client.send(" ".join(bookList))

	def openBook(self, client, book):
		FILE = open("books/" + book, "r")
		text = FILE.read().decode("utf-8")
	#	print text
		pageSize = 200

		total = 0
		textLen = len(text)
		pageList = []
		while total < textLen:
			pageList.append(text[total:min(textLen, total+200)].encode("utf-8"))
			total += 200
		client.send("0")
		client.send(pageList[0])
		return pageList

	def nextPage(self, client, info, pageList):
		curPage = int(info)

		if curPage != len(pageList):
			curPage += 1
		client.send(str(curPage))
		client.send(pageList[curPage])

	def previousPage(self, client, info, pageList):
		curPage = int(info)
		if curPage != 0:
			curPage -= 1

		client.send(str(curPage))	
		client.send(pageList[curPage])

	def goToPage(self, client, info, pageList):
		pageNumber = int(info)
		client.send(str(pageNumber))
		client.send(pageList[pageNumber])

if __name__ == "__main__":
	server = Server()
	server.run()
