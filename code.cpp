// c++ code
// The operating system is osx yosemite version 10.10.2
// compile command: g++ -O2 code.cpp -std=c++14
// to run ./a.out input_files
// This code removes punctuations from the input files, and don’t consider them.

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <unordered_map> // to hash keys to many values
#include <algorithm> // for_each
#include <utility>
#include <sstream> //stringstream
#include <unordered_set>
#include <cctype>

using namespace std;



// This class manages address of sentences in the input files (filename & line number)
class address{
private:
	int fileNumber;
	int lineNumber;
public:
	address(int num,int line){
		fileNumber=num;
		lineNumber=line;
	}
	address(){};
	void set(int num,int line){
		fileNumber=num;
		lineNumber=line;
	}
	int getNumber() const {return fileNumber;}
	int getLine() const {return lineNumber;}
	bool operator==(const address &anotherAddress) const
	{
		return (fileNumber == anotherAddress.fileNumber && lineNumber == anotherAddress.lineNumber);
	}
};

namespace std
{
	template <> // a template specialization to use for hashing user-defined class: address
	struct hash<address>
	{
		size_t operator()(const address& k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			size_t const hs = hash<int>()(k.getNumber());
			size_t const hi = hash<int>()(k.getLine());
			return ( hs ^ ( hi << 1)) >> 1;
		}
	};
}

// this function hash each file to the main hash table filehash to easy access in future.


int fileHashing(int fileNumber,string filename,unordered_multimap<int,address> & fileHash,vector<vector<string> > & library){
	// we use unordered map to hash words . we might have different filenames (multimap).
	// we do not need to store words or the whole sentence, instead the hash values of words in a hash table.

	hash<string> str_hash;
	
	ifstream file(filename);
	if (! file.good())
	{
		cerr << "Warning: '"<< filename << "' is not readable or doesn't exist. File ignored." << endl;
		return 0;
	}
	
	string sentence;
	int line=1;
	while (!file.eof()){
		getline(file,sentence);
		library[fileNumber-1].push_back(sentence);
		// we need to remove punctuations and hash each word in the sentence
		sentence.erase (remove_if(sentence.begin(), sentence.end(), ::ispunct),sentence.end());
		stringstream ss(sentence);
		string word;
		while(ss>>word) {
			for(auto & c: word) c=tolower(c); // convert all letters to lower case
			int hashValue=str_hash(word);
			address add(fileNumber,line);
			pair<int,address> p(hashValue,add);
			fileHash.insert (p);
		}
		line ++;
	}
	file.close();
	return 1;

}


//****************************************************************************
//                    MAIN FUNCTION
//***************************************************************************


int main(int argc, char* argv[]){
	unordered_multimap<int,address> fileHash;
	vector<vector<string> > library(argc-1); // it stores sentences in all files to have fast access when printing out the results.
	// we first do a preprocessing on files. we read all files. we hash each word to an int rather than stroing the whole sentence.
        // We use the int as a key and the value is a class that stores filename and line number.
	// the file names are entered by command line. They are stored in argv array
	hash<string> str_hash;
	cout << "Loading the files...\n" << flush;
	int num_read_files = 0;
	for(int i=1;i<argc;i++){
		num_read_files += fileHashing(i,argv[i],fileHash,library);
	}
	if (num_read_files == 0) 
	{
		cerr << "No (readable) input files, exiting..." << endl;
		exit (EXIT_FAILURE);
	}
	
	cout << "done." << endl;
	int occurrence=0; // count the total co-occurrence
	// we hash each line of query to an integer and we check if we have the same int in fileHash; if we have we store the sentence and filename and linenumber in a vector called"out".
	cout<<"query> ";
	string query;
	getline(cin,query);
	while (query !="!exit"){
		unordered_set<address> match1; // match is a hashed based container to keep co-occurrence of each word in a query.
		unordered_set<address> match2; // this is a temporary set to extract address
		stringstream ss(query);
		string qword;
		bool firstWord=true;
		while(ss>>qword) {
			for(auto & c: qword) c=tolower(c); // convert all letters to lower case
			match2.clear();
			int qhash=str_hash(qword);
			if (fileHash.find(qhash) != fileHash.end() ){
				auto range = fileHash.equal_range(qhash);
				// we find all occurrence in hash table created from files corresponding the query.
				for_each (
				range.first,
				range.second,
				[& match1,& match2,firstWord](auto& x){
					address add= x.second;
					if ( firstWord ) match1.insert(add);// for the first word of query
					else{
						if(match1.find(add) != match1.end()) match2.insert(add);
					}
				}
				);
			} else {
				match1.clear();
				break;} // we exit the loop if one of words does not exist.
			if (!firstWord) match1=match2;
			firstWord=false;
		}
		// in this step we gather co-occurrence of each query before receiving the next one
		for(auto & x: match1){
			int fn=x.getNumber();
			int ln=x.getLine();
			cout<<argv[fn]<<"#"<<ln<<": "<<library[fn-1][ln-1]<<endl;
			occurrence +=1;
		}
		// next query
		cout<<"query> ";
		getline(cin,query);
	}
	
	cout<<" Number of total co-occurrence: "<<occurrence<<endl;
	
	
	return 0;
}