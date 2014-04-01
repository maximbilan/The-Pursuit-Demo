# localization.py

import os, sys, struct, csv, xlrd

#------------------------------------------------------------------------------
# Usage information
#------------------------------------------------------------------------------
def Usage():
	print "localization.py"
	print "Usage:"
	print
	print "localization.py <XLS File>"

#------------------------------------------------------------------------------
# Create a hash value from a string
#------------------------------------------------------------------------------
def Hash(aStr):
	lHash = 0;
	aStr = aStr.upper()
	for c in aStr:
		lHash = (lHash * 29) + ord(c)
		lHash &= 0xffffffff
	return lHash

#------------------------------------------------------------------------------
# Create order hash list from first column values
#------------------------------------------------------------------------------
def CreateHashTable(aIDs):
	lIDList = []
	
	lRow = 0
	while lRow < len(aIDs):
		lID = aIDs[lRow]
		lHashValue = Hash(lID)
		
		# Create a tuple containing the hash value and the row to access
		lEntry = (lHashValue, lRow)
		lIDList.append(lEntry)
		lRow += 1

	# Sort list on hash value			
	lIDList = sorted(lIDList)
	
	return lIDList

#------------------------------------------------------------------------------
# Produce the output string files from the provided XLS file
#------------------------------------------------------------------------------
def PackStrings(aXLS):
	lXLS = xlrd.open_workbook(aXLS)
	lSheet = lXLS.sheet_by_index(0)
	lStringCount = lSheet.nrows - 1
	print str(lStringCount) + " strings found"

	lIDs = CreateHashTable(lSheet.col_values(0, 1, lSheet.nrows))
	
	lCol = 1
	while lCol < lSheet.ncols:
		lLanguageCode = lSheet.cell_value(0, lCol)
		print "Processing strings for " + lLanguageCode

		# Start building output file data by adding string count
		lData = struct.pack("I", lStringCount)
			
		# Create a used characters list
		lCharList = []
		
		lRow = 0			
		lStrings = lSheet.col_values(lCol, 1, lSheet.nrows)
		while lRow < len(lIDs):		
			(lHash, lLookupRow) = lIDs[lRow]
			lString = lStrings[lLookupRow]
				
			# Ensure we're trying to encode a string
			if type(lString) != unicode:
				lString = str(int(lString))
			
			# Replace carriage return and quote mark codes with real characters
			lString = lString.replace("\\n", "\x0a")
			lString = lString.replace("\\x22", "\x22")

			# Build list of unique characters			
			for lChar in lString:
				lOrd = ord(lChar)
				
				# Ignore whitespace
				if (lOrd == 0) or (lOrd == 10) or (lOrd == 13) or (lOrd == 9):
					continue
					
				if lChar not in lCharList:
					lCharList.append(lChar)
			
			# Convert to UTF-8 and add null terminator
			lString = lString.encode('utf-8', 'strict')
			lString += '\0'
			
			# Pad to string to be multiple of 4 bytes long
			lPad = 4 - (len(lString) % 4)
			if lPad != 4:
				lString += '\0' * lPad
				
			# Add hash value to data
			lData += struct.pack("I", int(lHash))
									
			# Add string to data
			lData += lString
						
			# Move to next row
			lRow += 1

		# Write strings to output file
		f = open(lLanguageCode + ".str", "wb")
		f.write(lData)
		f.close()
		
		# Create used characters files too
		lCharList = sorted(lCharList)

		lUsedCharacters = ""
		for lChar in lCharList:
			lStr = ""
			lStr += lChar
			lUsedCharacters += lStr.encode('utf-8', 'strict')

		f = open(lLanguageCode + ".chr", "wb")
		f.write(lUsedCharacters)
		f.close()
		
		# Move to next column
		lCol += 1
			
#------------------------------------------------------------------------------
# Main entry point
#------------------------------------------------------------------------------
# Start by checking command line parameters
if len(sys.argv) < 2:
	Usage()
	sys.exit(0)
	
PackStrings(sys.argv[1])
	


