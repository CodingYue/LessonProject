import subprocess
from random import randint
import os
import Tkinter, Tkconstants, tkFileDialog, tkMessageBox
import RSA

class UI(Tkinter.Frame):
	def __init__(self):
		self.root = Tkinter.Tk()
		#Tkinter.Frame.__init__(self, self.root)
		self.uiInit()

	def uiInit(self):
		self.root.title('RSA + DES Experiment')

		'''
			Row 0: RSA Label.
			Row 1: RSA displays.
		'''


		Tkinter.Label(self.root, text='RSA').grid(row=0, column=0, columnspan=16)
		
		Tkinter.Label(self.root, text='Public Key').grid(row=1, column=0, columnspan=4)
		Tkinter.Label(self.root, text='Private Key').grid(row=1, column=4, columnspan=4)
		Tkinter.Label(self.root, text='N').grid(row=1, column=8, columnspan=4)

		self.rsa_pubkey_text = Tkinter.Text(self.root, width=45, height=1)
		self.rsa_pubkey_text.grid(row=2, column=0, columnspan=4)

		self.rsa_prikey_text = Tkinter.Text(self.root, width=45, height=1)
		self.rsa_prikey_text.grid(row=2, column=4, columnspan=4)

		self.rsa_N_text = Tkinter.Text(self.root, width = 45, height = 1)
		self.rsa_N_text.grid(row=2, column=8, columnspan=4)

		Tkinter.Button(self.root, text='Generate RSA Key Pair', command=self.genRSAKey).grid(row=1, column=12, columnspan=4)

		Tkinter.Label(self.root, text='DES').grid(row=3, column=0, columnspan=16)

		'''
			Row 2: Generate DES Key and Key displays
		'''
		
		self.desKeyText = Tkinter.Text(self.root, width=45, height=1)
		self.desKeyText.grid(row=4, column=0, columnspan=4)
		Tkinter.Button(self.root, text='Generate DES Key', command=self.genDESKey).grid(row=4, column = 4, columnspan=4)

		self.desIVText = Tkinter.Text(self.root, width=45, height=1)
		self.desIVText.grid(row=4, column=8, columnspan=4)
		Tkinter.Button(self.root, text='Generate DES IV', command = self.genDESIV).grid(row=4, column=12, columnspan=4)

		'''
			Row 3: Passing DES Key
		'''

		self.desKeyPlainText = Tkinter.Text(self.root, width=45, height=1)
		self.desKeyPlainText.grid(row=5, column=0, columnspan=2)
		Tkinter.Button(self.root, text='Encrypt DES Key',command=self.encryptDESKey).grid(row=5, column=2, columnspan=5)

		self.desKeyEncryptedText = Tkinter.Text(self.root, width=45, height=1)
		self.desKeyEncryptedText.grid(row=5, column=7, columnspan=2)

		Tkinter.Button(self.root, text='Decrypt DES Key', command=self.decryptDESKey).grid(row=5, column=9, columnspan=5)

		self.desKeyDecryptedText = Tkinter.Text(self.root, width=45, height=1)
		self.desKeyDecryptedText.grid(row=5, column=14, columnspan=2)


		Tkinter.Label(self.root, text="Cipher").grid(row=6, column=0, columnspan=16)

		'''
			Row 4: Input Path, Output Path
		'''

		self.file_opt = options = {}
		options['defaultextension'] = '.*'
		options['initialdir'] = os.getcwd()+'/test'
		options['parent'] = self.root
		options['title'] = 'DES Files'

		self.inputPathText = Tkinter.Text(self.root, width=45, height=2)
		self.inputPathText.grid(row=7, column=0, columnspan=4)
		Tkinter.Button(self.root, text="Choose Input File", command=self.chooseInput).grid(row=7, column=4, columnspan=4)

		self.outputPathText = Tkinter.Text(self.root, width=45, height=2)
		self.outputPathText.grid(row=7, column=8, columnspan=4)
		Tkinter.Button(self.root, text="Choose Output File", command=self.chooseOutput).grid(row=7, column=12, columnspan=4)

		'''
			Row 5: Encrypt and Decrypt button
		'''

		Tkinter.Button(self.root, text='DES encrypt plaintext', command=self.desEncryptPlainText).grid(row=10, column=0, columnspan=8)
		Tkinter.Button(self.root, text='DES decrypt ciphertext', command=self.desDecryptCipherText).grid(row=10, column=8, columnspan=8)

	def chooseInput(self):
		self.inputPath = tkFileDialog.askopenfilename(**self.file_opt)
		self.display(self.inputPathText, self.inputPath)

	def chooseOutput(self):
		self.outputPath = tkFileDialog.asksaveasfilename(**self.file_opt)
		self.display(self.outputPathText, self.outputPath)

	def display(self, target, text):
		target.delete(1.0, Tkinter.END)
		target.insert(1.0, text)

	def genRSAKey(self):
		self.server = RSA.Server(64)
		self.client = RSA.Client(self.server.e, self.server.N)
		self.display(self.rsa_prikey_text, "d = " + str(self.server.d))
		self.display(self.rsa_pubkey_text, "e = " + str(self.server.e))
		self.display(self.rsa_N_text, "N = " + str(self.server.N))

	def genDESKey(self):
		self.desKey = randint(0, 2**64)
		self.display(self.desKeyText, "DES Key = " + str(hex(self.desKey)).rstrip("L"))
		self.display(self.desKeyPlainText, "DES Key = " + str(self.desKey))

	def genDESIV(self):
		self.desIV = randint(0, 2**64)
		self.display(self.desIVText, "IV = " + str(hex(self.desIV)).rstrip("L"))


	def encryptDESKey(self):
		try:
			assert self.desKey
		except:
			self.showinfo('DES Key NOT generated')
			return

		try:
			assert self.server
		except:
			self.showinfo('RSA Key NOT generated')
			return
		self.desEncryptedKey = self.client.encrypt(self.desKey)
		self.display(self.desKeyEncryptedText, "Encrypted DES Key = " + str(self.desEncryptedKey))

	def decryptDESKey(self):
		try:
			assert self.desEncryptedKey
		except:
			self.showinfo('DES Key NOT encrypted')
			return


		self.desDecryptedKey = self.server.decrypt(self.desEncryptedKey)
		self.display(self.desKeyDecryptedText, "Decrypted DES Key = " + str(self.desDecryptedKey))

	def showinfo(self, message):
		tkMessageBox.showinfo("RSA + DES", message)

	def desEncryptPlainText(self):

		try:
			assert self.desKey
		except:
			self.showinfo('DES Key NOT FOUND')
			return

		try:
			assert self.desIV
		except:
			self.showinfo('DES IV NOT generated')
			return	

		args = ["./des", "encrypt", str(self.desKey), self.inputPath, self.outputPath, str(self.desIV)]
		ret = subprocess.check_call(" ".join(args), shell=True)
		if ret == 0:
			self.showinfo("Encrypt Successed")
		else:
			self.showinfo("Encrypt Failed")

	def desDecryptCipherText(self):

		try:
			assert self.desKey
		except:
			self.showinfo('DES Key NOT FOUND')
			return

		try:
			assert self.desIV
		except:
			self.showinfo('DES IV NOT generated')
			return	

		args = ["./des", "decrypt", str(self.desKey), self.inputPath, self.outputPath, str(self.desIV)]
		ret = subprocess.check_call(" ".join(args), shell=True)
		
		if ret == 0:
			self.showinfo("Decrypt Successed")
		else:
			self.showinfo("Decrypt Failed")

if __name__ == "__main__":
	os.system("rm -rf des")
	os.system("CXXFLAGS=-O2 make des")
	ui = UI()
	ui.root.mainloop()