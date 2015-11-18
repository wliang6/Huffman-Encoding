#include <iostream>
#include <queue>
#include <map>
#include <climits>
#include "bstream.h"

using namespace std;

//function declarations



/* Type: ext_char
* A type representing a character, a pseudo-eof, or nothing. */
typedef int ext_char;

typedef map<ext_char, int> FreqMap;
typedef map<ext_char, string> CodeMap;
typedef map<string, ext_char> ReverseCodeMap;

/* Constant: PSEUDO_EOF
* A constant representing the PSEUDO_EOF marker that you will
* write at the end of your Huffman-encoded file.
*/
const ext_char PSEUDO_EOF = 256;

/* Constant: NOT_A_CHAR
* A constant representing an extended character that does not
* actually hold a value.	 When you are constructing your Huffman
* encoding tree, you should set the characters in each internal
* node to this value to explicitly mark that they are not being
* used.
*/
const ext_char NOT_A_CHAR = 257;

/* Struct used to store all the informantion a huffman node needs*/
struct HuffmanNode
{
	ext_char data;
	int weight;
	HuffmanNode *left; // Left subtree ( corresponding code to '0' )
	HuffmanNode *right; // Left subtree ( corresponding code '1' )
	//constructor for leaves
	HuffmanNode(ext_char dat, int wgt) :data(dat), weight(wgt), left(NULL), right(NULL){}
	// constructor for internal nodes
	HuffmanNode(HuffmanNode *l, HuffmanNode *r) :data(NOT_A_CHAR), weight(l->weight + r->weight), left(l), right(r) {}

};


/* Structure used for comparing two huffman Node. Used by priority queue*/
struct compareHuffmanNode
{
	bool operator()(const HuffmanNode *first, const HuffmanNode *second)
	 {
		return first->weight  > second->weight;
	}
};


// Get a line from the user. 
string getLine(string str)
{
	bool tryAgain = true;
	string outString;
	while (tryAgain)
	{
		cout << str << endl;
		getline(cin, outString);
		if (outString == "")
		{
			cout << "Enter a valid string\n";
		}
		else
		{
			tryAgain = false;
		}
	}
	return outString;
}

/* Function: openFile
* --------------------------------------------------------
* Prompts the user for the name of a file to open, opening
* the indicated stream object to that file.
*/
template <typename FileStream>
void openFile(FileStream& file, string filename ) {
	//while (true) {

		file.open(filename.c_str());

		if (file.is_open()) return;

		cout << "Sorry, I couldn't open that file." << endl;
		file.clear();
		
	//}
}



/* Function: compareFiles
* --------------------------------------------------------
* Compares two files byte-by-byte to determine whether or
* not they are equivalent to one another.
*/
void compareFiles(string firstFile, string secondFile) {
	/* Get the two files to compare. */
	ifstream one, two;
	openFile(one, firstFile);
	openFile(two, secondFile);

	/* Read the contents of the files into a stringstream for
	* comparison.
	*/
	ostringstream oneContents, twoContents;
	oneContents << one.rdbuf();
	twoContents << two.rdbuf();

	const string oneString = oneContents.str(), twoString = twoContents.str();

	/* Check lengths are the same. */
	if (oneString.length() != twoString.length()) {
		cout << "Files differ!" << endl;
		cout << "File one has length " << oneString.length() << "." << endl;
		cout << "File two has length " << twoString.length() << "." << endl;
	}
	else {
		/* Compare the two sequences to find a mismatch. */
		pair<string::const_iterator, string::const_iterator> diff =
			mismatch(oneString.begin(), oneString.end(), twoString.begin());
		if (diff.first != oneString.end()) {
			cout << "Files differ!" << endl;
			ptrdiff_t offset = diff.first - oneString.begin();
			cout << "Bytes differ at offset " << offset << "." << endl;
			cout << "File one has value " << *diff.first << endl;
			cout << "File two has value " << *diff.second << endl;
		}
		else {
			/* Files match! */
			cout << "Files match!" << endl;
		}
	}
}


/*
//WINNIE'S CODE
//storing character frequency text file into a map

template<typename ext_char>
FreqMap::map<ext_char, int> fillFreqMap(string& filename)
{
	FreqMap::map<ext_char, int> freqmap;
	ifbstream infile;
	openFile(infile, filename.c_str());
	string line;
	while(infile >> line){
		freqmap.insert(pair<ext_char, int> (line, 1));
	}
	
	typename map<ext_char, int>::iterator itr = freqmap.begin();
	while(itr != freqmap.end()){
		cout << itr -> first << "\t" << itr -> second << endl;
	}
	return freqmap;
}*/



//Preorder traversal of Huffman Tree to find the binary code for each character
//This is also stored into the CodeMap
void printCode(CodeMap& codemap, HuffmanNode* root, string code)
{
	if(root->data != NOT_A_CHAR){
		codemap.insert(pair<ext_char, string> (root->data, code));
	}else{ 
		if(root->left != NULL){
		printCode(codemap, root->left, code + "0");}
		if(root->right != NULL){
		printCode(codemap, root->right, code + "1");}
	}
}

//Storing Huffman Tree into compressed file used for decompression later
void storeHuffmanTree(ofbstream& ofs, HuffmanNode* root, string code)
{
	if(root->data != NOT_A_CHAR){
		if(root->left == NULL && root->right == NULL){ //leaf node
			ofs.writeBit(1);}
		else{ ofs.writeBit(0);} //internal node
	}else{ 
		if(root->left != NULL){
		storeHuffmanTree(ofs, root->left, code + "0");}
		if(root->right != NULL){
		storeHuffmanTree(ofs, root->right, code + "1");}
	}
}


//Constructing the Huffman Tree using a priority queue
HuffmanNode* constructHuffmanTree(const vector<HuffmanNode>& symbols)
{
	priority_queue<HuffmanNode*, vector<HuffmanNode*>, compareHuffmanNode> pqueue;

	for(size_t i=0; i<symbols.size(); i++){
		pqueue.push(new HuffmanNode(symbols[i]));}

	//binary tree construction
	while(pqueue.size() > 1){ 
		HuffmanNode* left = pqueue.top();
		pqueue.pop();
		HuffmanNode* right = pqueue.top();
		pqueue.pop();
		HuffmanNode* newHuff = new HuffmanNode(left, right);
		pqueue.push(newHuff); //push internal node back into pqueue
	}

	HuffmanNode* root = pqueue.top(); //root of the Huffman tree
	pqueue.pop(); //popping the completed Huffman tree off pqueue
	return root;
}


//Overloading insertion operator << used to write a map into a file
template<typename character, typename frequency>
std::ostream& operator<< (std::ostream& out, const std:: map<character, frequency>& map)
{
	for(typename std::map<character, frequency>::const_iterator itr = map.begin(); itr != map.end(); ++itr){
		out << itr->first << "     " << itr->second << endl;
	}
	return out;
}



/* Function: runEncoding
* --------------------------------------------------------
* Harness code to run your compression routine. */
void runEncoding()
{

	string inputFileName = getLine("Enter the name of file to encode ");
	string outputFileName = getLine("Enter the encoded file name ");
	string freqFileName = getLine("Enter the freq. file name");
  /* Your implementation */

	FreqMap freqmap;
	CodeMap codemap;

	ifbstream ifb(inputFileName.c_str());
	ofbstream ofb(freqFileName.c_str());

//Generating character frequency file	
	if(!(ifb.is_open() && ofb.is_open())){
		cerr << "Error opening files." << endl;
	}
	
	while(!ifb.eof()){
		ext_char c = ifb.get(); //get one byte at a time from file
		std::map<ext_char, int>::iterator itr;
		itr = freqmap.find(c);
		if(itr != freqmap.end()){ 
			itr->second += 1; //increment character freq if found
		}

		else{
			freqmap[c] = 1;
		}
	}
	freqmap.insert(pair<ext_char, int> (PSEUDO_EOF, 1)); //EOF inserted
	
	ofb << freqmap; //frequency map has been made

//Convert frequency map to vector of HuffmanNodes
	vector <HuffmanNode> symbols;

	std::map<ext_char, int>::iterator itr;
	for(itr = freqmap.begin(); itr != freqmap.end(); ++itr){
		symbols.push_back(HuffmanNode(itr->first, itr->second));
}
	
//Constructing the Huffman Tree
	HuffmanNode* root = constructHuffmanTree(symbols);

//Printing out binary code in string form for each character node
	string code = "";
	printCode(codemap, root, code); //helper method

//CodeMap check (self-reference) 
	std::map<ext_char, string>::iterator iter;
	for(iter = codemap.begin(); iter != codemap.end(); ++iter){
		cout << iter->first << "     " << iter->second << endl;
	}

//Writing the encoding into output file
	ifbstream three(inputFileName.c_str());
	ofbstream four(outputFileName.c_str());
	storeHuffmanTree(four, root, code); //store Huffman tree in beginning
	while(!three.eof()){
		ext_char a = three.get();
		string code = codemap[a];
		for(size_t i=0; i < code.length(); i++){
			char x = code.at(i);
			if(x == '0'){ 	four.writeBit(0);}
			else if(x == '1'){ four.writeBit(1);}
		}
	}
}




/* Huffman Decoding stuff starting here *****************************/
/*
typedef HuffmanNode* nodePointer;
nodePointer myRoot;

//Insert a node for a character in Huffman decoding tree.
//code is initially in bit string converted into character
void insert(char ch, string code)
{
	nodePointer p = myRoot;
	for(unsigned int i=0; i<code.length(); i++){
		switch(code[i])
		{
		case '0': //go down left
			if(p->left == 0)
				p->left = new HuffmanNode(NOT_A_CHAR, 0);
			p = p->left;
			break;
		case '1': //go down right
			if(p->right == 0)
				p->right = new HuffmanNode(NOT_A_CHAR, 0);
			p = p->right;
			break;
		default:
			cerr << "Illegal character in code" << endl;
			exit(1);
		}
	}
	p->data = ch;
} 

*/

/*
//Initializes a tree that consists of a single node and then read letters and their codes from a code file and constructs a decoding tree
HuffmanNode* buildDecodingTree(ifbstream& ifb)
{
	HuffmanNode* root;
	while(!ifb.eof()){
		ifb.readBit(); 
	for(;;)
	{
		ifb >> ch >> code;
		if(ifb.eof()) return;
		insert(ch, code);		
	}
	}
}



//Reads a message of string bits from encoded file and then decode it using the decoding tree
void decode(ifbstream& ifb, ofbstream& ofb)
{
	char bit;
	nodePointer p;
	for(;;)	
	{
		p = myRoot;
		while(p->left != 0 || p->right != 0)
		{
			ifb >> bit;
			if(ifb.eof()) return;
			//cout << bit;
			if(bit == '0') 
				p = p->left;
			else if(bit == '1')
				p = p->right;
			else{
				cerr << "Illegal bit: " << bit << endl;}
		}
		ofb << p->data;
	}
}
*/
//extraction operator >> 
template<typename K, typename V>
std::istream& operator>> (std::istream& in, std::map<K,V>& map){
	string mapkeyword;
	size_t num;
	in >> mapkeyword >> num;
	for(size_t i=0; i<num; ++i){
		K key;
		V value;
		in >> key >> value;
		map[key] = value;
	}
	return in;
}

void encodeLeaves(HuffmanNode* root, string code, ReverseCodeMap& codemap)
{
	if(root->data != NOT_A_CHAR){
		codemap.insert(std::pair<string, ext_char>(code, root->data));}
	if(root->left != NULL){
		encodeLeaves(root->left, code, codemap);}
	if(root->right != NULL){
		encodeLeaves(root->right, code, codemap);}
}

/* Function: runDecoding
* --------------------------------------------------------
* Harness code to run your decompression routine. */
void runDecoding()
{
	string inEncodeFileName = getLine("Enter the name of file to decode ");
	string outEncodeFileName = getLine("Enter the  file name to save after decoding");
	string freqFileName = getLine("Enter the freq. file name");
  /* Your implementation */

	ifbstream ifs(inEncodeFileName.c_str());
	ofbstream ofs(outEncodeFileName.c_str());
	ifbstream ifr(freqFileName.c_str());
	ifs.rewind(); //rewind input stream
	if(!(ifs.is_open() && ofs.is_open())){
		cerr << "Error opening files." << endl;
	}
	/*
	int bit = 0;
	while(!ifs.eof()){
		bit = ifs.readBit();
		if(bit != -1){
		buildDecodingTree(ifs);}
		decode(ifs, ofs);
	}
}	
*/

/*
	int bit = 0;
	while(true){
		bit = ifs.readBit();
		if(bit == -1){ //indicates EOF
			
		}
		else if(bit == 0){ //go down left branch
			tree->left;
		}
		else if(bit == 1){ //go down right branch
			tree->right;
		}
		if(tree == NULL){ //corrupt tree
			cerr << "Corrupt Huffman Tree in encoded text."<< endl;}
		if(tree->data == PSEUDO_EOF){
			
		}
		if(tree->data == NOT_A_CHAR){
		}		
	*/	
	FreqMap freqmap;
	ReverseCodeMap codemap; 
	vector<HuffmanNode> symbols;
	ifr >> freqmap; //using extraction operator ----- somewhat error here
	std::map<ext_char, int>::iterator it;
	for(it = freqmap.begin(); it!= freqmap.end(); ++it){	
		symbols.push_back(HuffmanNode(it->first, it->second));
		//self-reference
		cout << it->first << "   " << it->second << endl;
	}
 	HuffmanNode* root = constructHuffmanTree(symbols);

	string encoded = "";
	//encodeLeaves(root, encoded, codemap);
	while(!ifs.eof()){
	int bit = ifs.readBit();
	if(bit == 0){ 
		encoded += "0";}
	if(bit == 1){
		encoded += "1";}
	encodeLeaves(root, encoded, codemap);
	std::map<string, ext_char>::iterator itr; 
		for(itr = codemap.begin(); itr != codemap.end(); ++itr){
			if(itr->second == PSEUDO_EOF) break;
			if(encoded == itr->first){
				ofs << char(itr->second);
				encoded = "";
			}
		}
	
	}		
}	


/* Function: runCompare
* --------------------------------------------------------
* Harness code to compare two files */
void runCompare()
{
	string firstFile = getLine("Enter the name of first file ");
	string secondFile = getLine("Enter the name of second file ");
	compareFiles(firstFile, secondFile);
}


// Entry point
int main()
{
  while (1)
  {
    cout << "Enter your choice \n"
      " 1. Huffman Encoding \n"
      " 2. Huffman Decoding \n"
      " 3. Compare Files\n" 
      " 4. Exit" << endl;
    int choice;
    cin >> choice;
    cin.clear(); cin.ignore(INT_MAX, '\n');
    if (choice == 1)
    {
      cout << " Encoding " << endl;
      runEncoding();
      cout << " Encoding done " << endl;
    }
    else if (choice == 2)
    {
      cout << " Decoding " << endl;
      runDecoding();
      cout << " Decoding done " << endl;
    }
    else if (choice == 3)
    {
      cout << "Comparing two files" <<endl;
      runCompare();
      cout << "Comparing done" <<endl;
    }
    else if( choice == 4)
    {
      cout <<"Exiting" <<endl;
      return 0;
    }
    else
    {
      cout << "Wrong choice. Try again \n";
    }
  }
  return 0;
}

int test()
{
	string name = "a.txt";
	ofbstream outfile;
	outfile.open(name.c_str());
	if (outfile.fail()) error("Can't open output file!");
	outfile << 134;
	char c = 'C';

	cout << sizeof('C') << c << " "<< int(c)<< endl;
	outfile.put('C');
	outfile.writeBit(0);
	outfile.writeBit(1);
	outfile.close();
	ifbstream infile;
	infile.open(name.c_str());
	if (infile.fail()) error("Can't open input file!");
	int num;
	infile >> num;
	cout << "read " << num;
	char dd = infile.get();
	cout << " " << dd << " and "
		<< infile.readBit() << infile.readBit() << endl;
	infile.close();
	getchar();
	getchar();
	return 0;
}
