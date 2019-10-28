#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
using namespace std;

int main(int argc, char** argv)
{
	//reads text (from redirected text file on command line)
	string text;
	getline(cin, text);
	string fixedText = "";
	if(text.length() > 10000) {
		cerr << "07 - Input too long" << endl;
		return 0;
	}
	//remove punctuation, numbers, spaces, etc.
	for (int i = 0; i < text.length(); i++) {
		if (text[i] >= 'A' && text[i] <= 'Z' || text[i] >= 'a' && text[i] <= 'z') {
			fixedText = fixedText + text[i];
		}
	}
	
/*----------Sets the key and size of matrix--------------------*/
	string key;
	if(strcmp(argv[2], "-k") == 0) {
		if(argc > 3) {
			key = argv[3];
			for (int i = 0; i < key.length(); i++) {
				if (key[i] < 'A' || key[i] > 'Z' && key[i] < 'a' || key[i] > 'z') {
					cerr << "03 - Bad key" << endl;
					return 0;
				}
			}
			transform(key.begin(), key.end(), key.begin(), ::tolower);
		} else {
			cerr << "02 - Missing key" << endl;
			return 0;
		}
	} else {
		cerr << "01 - Required key option not present" << endl;
		return 0;
	}
	int numCol = key.length();
	int numRow;
	if (fixedText.length() % key.length() == 0) {
		numRow = fixedText.length() / key.length();
	} else {
		numRow = (fixedText.length() / key.length()) + 1;
	}
	vector < vector < char > > data(numRow, vector < char > (numCol, 0));
	
/*----------Sets the pad character, if provided by the user-----------------*/	
	char* pad;
	char padchar;
	bool padf = false;
	if(argc>4) {
		if(argc==6){
			if(strcmp(argv[4], "-p") == 0) {
				pad = argv[5];
				if(strlen(pad) > 1) {
					cerr << "05 - Bad pad character" << endl;
					return 0;
				}
				padchar = pad[0];
				if(padchar < 'A' || padchar > 'Z' && padchar < 'a' || padchar > 'z') {
					cerr << "05 - Bad pad character" << endl;
					return 0;
				}
				padf = true;
			}
		} else if(argc==5){
			cerr << "04 - Missing pad character" << endl;
			return 0;
		} else {
			cerr << "06 - Unknown flag" << endl;
			return 0;
		}
	}
	
/*------------Determine the order that columns are written to/read from based on the key------------------*/	
	string sortedKey = key;
	sort(sortedKey.begin(), sortedKey.end());
	int order[numCol];
	fill_n(order, numCol, -1);
	for (int i = 0; i < numCol; i++) {
		for (int j = 0; j < numCol; j++) {
			if (sortedKey[j] == key[i] && order[j] == -1) {
				order[j] = i;
				break;
			}
		}
	}
	
/*-------------------------This section handles the ENCRYPTION process------------------------------*/
	if(strcmp(argv[1], "-e") == 0) {
		if(padf){
		/*------------with pad-------------*/
			//write to matrix
			int counter = 0;
			for (int i = 0; i < numRow; i++) {
				for (int j = 0; j < numCol; j++) {
					if (counter < fixedText.length()) {
						data[i][j] = fixedText[counter];
						counter++;
					} else {
						data[i][j] = padchar;
					}
				}
			}
			//read from matrix and format
			string ciphertext = "";
			for(int i = 0; i < numCol; i++) {
				for(int j = 0; j < numRow; j++) {
					ciphertext = ciphertext + data[j][order[i]];
				} 
			}
			for (int i = 5; i < ciphertext.length(); i+=6) {
				ciphertext.insert(i, " ");
			}
			if (ciphertext.length() > 50) {
				for (int i = 50; i < ciphertext.length(); i+=51) {
					ciphertext.insert(i, "\n");
				}
			}
			transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(), ::toupper);
			cout << ciphertext << endl;
		} else {
		/*-----------without pad--------------*/
			//write to matrix
			int counter = 0;
			for (int i = 0; i < numRow; i++) {
				for (int j = 0; j < numCol; j++) {
					if (counter < fixedText.length()) {
						data[i][j] = fixedText[counter];
						counter++;
					}
				}
			}
			//read from matrix and format
			string ciphertext = "";
			for(int i = 0; i < numCol; i++) {
				for(int j = 0; j < numRow; j++) {
					if(data[j][order[i]] == 0) {
						continue;
					} else {
						ciphertext = ciphertext + data[j][order[i]];
					}
				} 
			}
			for (int i = 5; i < ciphertext.length(); i+=6) {
				ciphertext.insert(i, " ");
			}
			for (int i = 50; i < ciphertext.length(); i+=51) {
				ciphertext.insert(i, "\n");
			}
			transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(), ::toupper);
			cout << ciphertext << endl;
		}
	}
			
/*-------------------------This section handles the DECRYPTION process----------------------------------*/
	else if(strcmp(argv[1], "-d") == 0) {
		if(padf) {
		/*--------------with pad---------------*/
			//write to matrix
			int counter = 0;
			for(int i = 0; i < numCol; i++) {
				for(int j = 0; j < numRow; j++) {
					data[j][order[i]] = fixedText[counter];
					counter++;
				} 
			}
			//read from matrix and format
			string plaintext = "";
			for (int i = 0; i < numRow; i++) {
				for (int j = 0; j < numCol; j++) {
					plaintext = plaintext + data[i][j];
				}
			}
			transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::tolower);
			for (int i = 1; i < numCol; i++) {
				if(plaintext[plaintext.length()-1] == padchar) {
					plaintext.pop_back();
				} else {
					break;
				}
			}
			for (int i = 5; i < plaintext.length(); i+=6) {
				plaintext.insert(i, " ");
			}
			for (int i = 50; i < plaintext.length(); i+=51) {
				plaintext.insert(i, "\n");
			}
			cout << plaintext << endl;
		} else {
		/*-------------without pad---------------*/
			//write to matrix
			int numEmpty = numCol - (fixedText.length() % numCol);
			for (numEmpty; numEmpty > 0; numEmpty--) {
				data[numRow-1][numCol-numEmpty] = '!';
			}
			
			int counter = 0;
			for(int i = 0; i < numCol; i++) {
				for(int j = 0; j < numRow; j++) {
					if(data[j][order[i]] == '!') {
						continue;
					} else {
						data[j][order[i]] = fixedText[counter];
						counter++;
					}
				} 
			}
			//read from matrix and format
			string plaintext = "";
			for (int i = 0; i < numRow; i++) {
				for (int j = 0; j < numCol; j++) {
					if (data[i][j] == '!') {
						continue;
                    } else {
						plaintext = plaintext + data[i][j];
                    }
                }
            }
			for (int i = 5; i < plaintext.length(); i+=6) {
				plaintext.insert(i, " ");
			}
			for (int i = 50; i < plaintext.length(); i+=51) {
				plaintext.insert(i, "\n");
			}
			transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::tolower);
			cout << plaintext << endl;
		}

	} else {
		cerr << "06 - Unknown flag" << endl;
		return 0;
	}
	
	return 0;
}
