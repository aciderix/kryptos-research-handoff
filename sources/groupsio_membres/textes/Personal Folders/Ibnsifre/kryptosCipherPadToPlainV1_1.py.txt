#!/usr/bin/python

# v 1.1 contains a bug fix in the doc string

docString = """kryptosCipherPadToPlain.py is a quick and simple python script
   to convert a ciphertext and padtext to a plaintext using the Kryptos
   alphabet.  Enjoy! 
       -Ibn Sifre
   
   Usage: python kryptosCipherPadToPlain.py 'ciphertext' 'padtext'"""

import sys

# if the user wants help
if len(sys.argv) == 2 and sys.argv[1].strip().lower() == '-h':
    print docString
    sys.exit(0)

# check the inputs
if len(sys.argv) != 3:
    print docString
    print ""
    print "Both arguments are needed."
    sys.exit(1) 

# set up dictionaries to go from letter to number and number to letter
kryptosBet = "KRYPTOSABCDEFGHIJLMNQUVWXZ"

letterToNumDict = {kryptosBet[i]:i for i in range(0, 26)}
numToLetterDict = {i:kryptosBet[i] for i in range(0, 26)}

ciphertext = sys.argv[1]
padtext = sys.argv[2]

# for debugging and testing
#ciphertext = "NYPVTT"
#padtext = "ELYOIE"

# find the plaintext
plaintext =''

try:
    # put everything into uppercase
    ciphertext = ciphertext.strip().upper()
    padtext = padtext.strip().upper()
    
    # find the length of the shortest string
    minLength = min(len(ciphertext), len(padtext))
    
    # find the plaintext, letter by letter
    for i in range(0, minLength):
        cipherNum = letterToNumDict[ciphertext[i]]
        padNum = letterToNumDict[padtext[i]]
        
        plainNum = (cipherNum - padNum) % 26
        plaintext += numToLetterDict[plainNum]
        
except KeyError, e:
    print "There was an invalid character in either the ciphertext or padtext: " + str(e)
    
except Exception, e:
    print "There was an error of some sort: " + str(e)
    
else:
    print plaintext

pass