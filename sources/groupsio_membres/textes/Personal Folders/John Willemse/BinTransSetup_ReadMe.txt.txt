The Binary Transposition K4 (BTK4) application is a small tool I made, that uses Jason DeSeve's theory of a 5-bit binary transposition by writing out columns and reading of rows of binary information.

BTK4 features:

- enter an alternative alphabet, or an alphabet based on a key. The default is "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef", as used by Jason in his Excel file.
- enter or edit the ciphertext entry, which defaults to the K4 excluding the question mark.
- determine the row order in which the data is read. Jason reads the rows from top to bottom, but this will allow any transposition on the rows to be simulated.
- fully character case sensitive, i.e. it distinguishes between capital and small characters.
- Read the frequency of zeroes and ones realtime
- Displays the unigram and bigram frequency distributions of the produced plaintext in realtime.
- Shift the alphabet or ciphertext left or right one position, or reverse it entirely.

The tool requires the Microsoft .NET Framework 2.0 or later and Windows Installer 3.1. Usually, these are already installed on Windows XP. If not, the installer can download and install these prerequisites for you.

The application is pretty straightforward, but if you have any suggestions or questions, please email me at ruffnekk at gmail dot com or leave a message in the group.