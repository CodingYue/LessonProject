
from Crypto.Cipher import AES;
import sys;
import re;
import os;
import math;

BS = 16
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[:-ord(s[len(s)-1:])]

key_cbc="140b41b22a29beb4061bda66b6747e14";
key_ctr="36f18357be4dbd77f050515c73fcf9f2";

#note: both ciphertexts are 64 bytes length (in fact both are 128 characters long and 2 characters form a byte)
ciphertext_cbc1="4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
ciphertext_cbc2="5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253";

#these have variable length
ciphertext_ctr1="69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329";
ciphertext_ctr2="770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451";

cipher = AES.new(key_cbc.decode('hex'), mode=AES.MODE_CBC, IV=ciphertext_cbc1[:32].decode('hex'))
print unpad(cipher.decrypt(ciphertext_cbc1.decode('hex'))[16:])

cipher = AES.new(key_cbc.decode('hex'), mode=AES.MODE_CBC, IV=ciphertext_cbc2[:32].decode('hex'))
print unpad(cipher.decrypt(ciphertext_cbc2.decode('hex'))[16:])

secret = os.urandom(16)

cipher = AES.new(key_ctr.decode('hex'), mode=AES.MODE_CTR, IV=ciphertext_ctr1[:32].decode('hex'), counter=lambda :secret)
print cipher.decrypt(ciphertext_ctr1.decode('hex'))
