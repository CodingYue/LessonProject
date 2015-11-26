class Protocol(object):

	def __init__(self):
		'''
			Four bytes string message
			First byte show what kind of operation.

			0x00 List Books.
			0x01 Open Book
			0x02 Next Page
			0x03 Previous Page
			0x04 Go to page
			0x05 List bookmark / xor bookmark.
			0x06 server will continue sending books' name.
			0x07 the last book's name
			0x08 shutdown.
		'''

	def handler(self, message):


