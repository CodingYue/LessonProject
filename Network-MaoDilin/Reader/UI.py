# -*- coding: utf-8 -*-
import Tkconstants, tkFileDialog, tkMessageBox
import Tkinter as tk

class UI(tk.Frame):

	""" A GUI for simple Reader """

	def __init__(self, master, client):
		tk.Frame.__init__(self, master)
		self.client = client

		self.grid()
		self.createWidges()
		self.initialDisplay()
		self.createhBookList()

	def initialDisplay(self):
		self.bookText.grid(row=1, column=0, columnspan=20)
		self.bookListbox.grid(row=2,column=4, columnspan=4)
		self.bookmarkListbox.grid(row=2, column=12, columnspan=4)

		self.previousPageButton.grid(row=3, column=0, columnspan=4)
		self.openBookButton.grid(row=3, column=4, columnspan=4)
		self.nextPageButton.grid(row=3, column=8, columnspan=4)
		self.xorBookmarkButton.grid(row=3, column=12, columnspan=4)
		self.toBookmarkPageButton.grid(row=3, column=16, columnspan=4)

	def createhBookList(self):

		self.client.listBook()

		for book in self.client.bookList:
			self.bookListbox.insert(tk.END, book)

	def updateBookmark(self):
		self.client.listBookmark()
		self.bookmarkListbox.delete(0, tk.END)
		for page in self.client.bookmarkList:
			self.bookmarkListbox.insert(tk.END, page)

	def openBook(self, book):
		print book
		self.client.openBook(book)
		#self.client.listBookmark()
		#print self.client.text
		self.display(self.bookText, self.client.text)
		self.updateBookmark()

	def createWidges(self):

		self.bookListbox = tk.Listbox(self)
		self.bookmarkListbox = tk.Listbox(self)

		self.bookLabel = tk.Label(self, text=u"书籍")
		self.bookmarkLabel = tk.Label(self, text=u"书签")
		self.textLabel = tk.Label(self, text=u"正文阅读")

		self.nextPageButton = tk.Button(self, text=u"下一页", command = self.nextPage)
		self.previousPageButton = tk.Button(self, text=u"上一页", command = self.previousPage)
		self.xorBookmarkButton = tk.Button(self, text=u"加入/删除此页书签", command = self.xorBookmark)
		self.openBookButton = tk.Button(self, text=u"阅读此书",
			command = lambda : self.openBook(
				self.client.bookList[self.bookListbox.curselection()[0]]))
		self.toBookmarkPageButton = tk.Button(self, text=u"前往书签",
			command = lambda: self.toPage(
				self.client.bookmarkList[self.bookmarkListbox.curselection()[0]]
				))

		self.bookText = tk.Text(self, height=35, width = 130)
		self.statusText = tk.Text(self, height=1, width=10)

	def nextPage(self):
		self.client.nextPage()
		self.display(self.bookText, self.client.text)

	def previousPage(self):
		self.client.previousPage()
		self.display(self.bookText, self.client.text)

	def xorBookmark(self):
		self.client.xorBookmark()
		self.updateBookmark()

	def toPage(self, page):
		self.client.toPage(page)
		self.display(self.bookText, self.client.text)

	def display(self, target, text):
		target.delete(1.0, tk.END)
		target.insert(1.0, text)

from client import Client

if __name__ == "__main__":

	root = tk.Tk()
	root.title("Reader")
	App = UI(root, Client())
	root.mainloop()
