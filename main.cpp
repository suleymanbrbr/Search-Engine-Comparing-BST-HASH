#include <fstream>
#include "BST.h"
#include "Hash.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Struct to represent a document item
struct DocumentItem {
    
    string documentName = "";
    int count = 0; // Count of occurrences of a word in this document
};

// Struct to represent a word item
struct WordItem {
    
    string word_name = ""; // The word itself
    vector<DocumentItem> documents; // List of documents containing this word
};

// Struct to represent word output
struct WordOutput {
    
    string documentName; // Name of the document
    string word; // Word
    int count; // Count of occurrences of the word in the document
};

// Function to convert a string to lowercase
string toLowercase(string &data) {
    
    transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return data;
}

// Function to check if a document is already in the vector

bool check_document(vector<DocumentItem> vec, string file_name, int & idx){
    for (int i = 0; i < vec.size(); i++){
        if (vec[i].documentName == file_name){
            idx = i;
            return true;
        }
    }
    return false;
}

// Function to remove punctuation and digits from a word
void removePunctuationAndDigits(string& word, vector<string> &result) {
    
    string temp = "";
    for (char letter : word) {
        if (isalpha(letter)) {
            temp += letter; // Append the character to the result string
        }
        else {
            result.push_back(temp);
            temp = "";
        }
    }
    result.push_back(temp);
}

// Function to check if a word is in the vector and add it to a temporary vector
void isWordInVector(const vector<WordOutput>& vec, vector<WordOutput>& temp, const string& filename) {
    
    for (const auto& element : vec) {
        if (element.documentName == filename)
            temp.push_back(element); // Word found in vector
    }
}

// Function to check if a word is found in the vector
bool isFoundWordInVector(const vector<WordOutput>& vec, const string& filename, const string& word) {
    
    for (const auto& element : vec) {
        if (element.documentName == filename && element.word == word)
            return true;
    }
    return false;
}


int main() {
    // Constants
    const string ITEM_NOT_FOUND = "not found";

    // Variables
    int num_files;
    vector<string> files_name; // List of file names
    AvlTree<string, WordItem *> myTree (ITEM_NOT_FOUND); // AVL tree to store words and their details
    HashTable<string, WordItem> myHashTable (ITEM_NOT_FOUND);
    // Input number of files
    cout << "Enter number of input files: ";
    cin >> num_files;
    // Input file names and process each file
    for(int i = 0; i < num_files; i++){
        
        string file_name;
        cout << "Enter " << i+1 << ". file name: ";
        cin >> file_name;
        cin.ignore(); // Ignore any remaining characters in the input buffer
        
        files_name.push_back(file_name); // Store file name
    }
    
    for (int g = 0; g < files_name.size(); g++){
        
        ifstream file;
        // Open file
        file.open(files_name[g]);
        string word;
        // Read each word from file
        while (file >> word) {
            
            WordItem * n_word = new WordItem;
            WordItem n_wordhash;
            word = toLowercase(word); // Convert word to lowercase
            vector <string> separated_word;
            removePunctuationAndDigits(word, separated_word); // Remove punctuation and digits from word
        
            for (int y = 0; y < separated_word.size(); y++){
                
                if (separated_word[y] != ""){ // If word is not empty
                    
                    n_word->word_name = separated_word[y]; // Set word name in WordItem
                    
                    if(myTree.isEmpty()){ // If tree is empty
                        
                        DocumentItem document;
                        document.documentName = files_name[g];
                        document.count += 1;
                        n_word->documents.push_back(document); // Add document to word's document list
                        myTree.insert(separated_word[y], n_word); // Insert word into AVL tree
                    }
                    
                    else {
                        // if word is not in tree, insert to tree
                        if (myTree.find(separated_word[y]) == ITEM_NOT_FOUND){
                            
                            DocumentItem document;
                            document.documentName = files_name[g];
                            document.count += 1;
                            n_word->documents.push_back(document); // Add document to word's document list
                            myTree.insert(separated_word[y], n_word); // Insert word into AVL tree
                        }
                        
                        else {
                            // if word is in tree, increase count
                            int index = -1;
                            WordItem * temp = myTree.update(separated_word[y])->details;
                            // if document is same, do nothing
                            if (check_document(temp->documents, files_name[g], index))
                                temp->documents[index].count += 1; // Increment count
                            // if document name is different, update the document item
                            else {
                                
                                DocumentItem new_document;
                                new_document.documentName = files_name[g];
                                new_document.count += 1;
                                temp->documents.push_back(new_document); // Add document to word's document list
                            }
                        }
                    }
                    
                    // This part is for hash map
                    n_wordhash.word_name = separated_word[y];
                    
                    // if word is not in hashtable, insert to hashtable
                    if (myHashTable.find(separated_word[y]) == ITEM_NOT_FOUND){
                        
                        DocumentItem document;
                        document.documentName = files_name[g];
                        document.count += 1;
                        n_wordhash.documents.push_back(document); // Add document to word's document list
                        myHashTable.insert(separated_word[y], n_wordhash); // Insert word into AVL tree
                    }
                    
                    else {
                        // if word is in hashtable, increase count
                        int index = -1;
                        WordItem temp = myHashTable.getvalue(separated_word[y]);
                        
                        // if document is same, do nothing
                        if (check_document(temp.documents, files_name[g], index))
                            temp.documents[index].count += 1; // Increment count
                        
                        // if document name is different, update the document item
                        else {
                            
                            DocumentItem new_document;
                            new_document.documentName = files_name[g];
                            new_document.count += 1;
                            temp.documents.push_back(new_document); // Add document to word's document list
                        }
                        myHashTable.update(separated_word[y], temp);
                    }
                }
            }
        }
    }
    
    float ratio = 0;
    int num = myHashTable.output(ratio);
    cout << endl << "After preprocessing, the unique word count is " << num << ". Current load ratio is" << endl;
    cout << ratio << endl;
    
    bool flag = true;
    string query;

    // Input query words until "ENDOFINPUT" is entered
    while (flag) {
        
        cout << "Enter queried words in one line: ";
        vector<string> BST_words;
        vector<string> HASH_words;
        getline(cin, query); // Read the entire line of input

        if (query == "ENDOFINPUT")
            flag = false; // Stop loop if "ENDOFINPUT" is entered
        
        else {
            
            istringstream iss(query); // Create a stringstream to tokenize the input
            string word;
            // Tokenize the input line
            while (iss >> word) {
                toLowercase(word);
                vector <string> separated_words;
                removePunctuationAndDigits(word, separated_words);
                
                for (int z = 0; z < separated_words.size(); z++){
                    
                    if (separated_words[z] != ""){
                        BST_words.push_back(separated_words[z]); // Store each word in the vector
                        HASH_words.push_back(separated_words[z]);
                    }
                }
            }

            bool controlBST = true, controlHASH = true;
            vector<WordOutput> BST_word_details, BST_found_word;
            vector<WordOutput> HASH_word_details, HASH_found_word;

            int k = 20;
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < k; i++){
                // Process each word in the query
                for (int a = 0; a< BST_words.size(); a++){
                    
                    if (BST_words[0] == "remove"){ // Check if the command is to remove a word
                        
                        myTree.remove(BST_words[1]);
                        cout << BST_words[1] << " has been REMOVED" << endl;
                        BST_words.clear();
                        controlBST = false;
                        break;
                    }
                    // Check if word exists in the AVL tree
                    if (myTree.find(BST_words[a]) != ITEM_NOT_FOUND){
                        // Get word information
                        WordItem * word_information = myTree.update(BST_words[a])->details;
                        
                        for (int c = 0; c < word_information->documents.size(); c++){
                            
                            WordOutput temp;
                            temp.documentName = word_information->documents[c].documentName;
                            temp.count = word_information->documents[c].count;
                            temp.word = BST_words[a];
                            BST_word_details.push_back(temp);
                        }
                    }
                }
            }
            auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
              
                      
          bool checkBST = false;
          if (controlBST){
              // Check if all words in query exist in any document
              for (int a = 0; a < files_name.size(); a++){
                  
                  int num = 0;
                  for (int b = 0; b < BST_words.size(); b++){
                      if(isFoundWordInVector(BST_word_details, files_name[a], BST_words[b]))
                          num++;
                  }
                  if (num == BST_words.size()){
                      checkBST = true;
                  }
              }

              if (!checkBST)
                  cout << "No document contains the given query" << endl; // No document contains all the queried words
              else {
                  // Print occurrences of queried words in each document
                  BST_found_word.clear();
                  for (int i = 0; i < files_name.size(); i++){
                      
                      isWordInVector(BST_word_details, BST_found_word, files_name[i]);
                      
                      if (BST_found_word.size()/20 == BST_words.size()){
                          
                          cout << "in Document " << files_name[i] << ", ";
                          for (int j = 0; j < BST_words.size(); j++){
                              
                              for (int k = 0; k < BST_found_word.size()/20; k++){
                                  
                                  if (j != BST_found_word.size()/20-1 && BST_words[j] == BST_found_word[k].word)
                                      cout << BST_words[j] << " found " << BST_found_word[k].count << " times, ";
                                  else if (j == BST_found_word.size()/20-1 && BST_words[j] == BST_found_word[k].word)
                                      cout << BST_words[j] << " found " << BST_found_word[k].count << " times." << endl;;
                              }
                          }
                      }
                      BST_found_word.clear();
                  }
              }
          }
            
      start = std::chrono::high_resolution_clock::now();
      for (int i = 0; i < k; i++)
      {
          // For hash table
          
          // Process each word in the query
          for (int a = 0; a< HASH_words.size(); a++){
              
              if (HASH_words[0] == "remove"){ // Check if the command is to remove a word
                  myHashTable.remove(HASH_words[1]);
                  cout << HASH_words[1] << " has been REMOVED" << endl;
                  HASH_words.clear();
                  controlHASH = false;
                  break;
              }
              // Check if word exists in the hashtable
              if (myHashTable.find(HASH_words[a]) != ITEM_NOT_FOUND){
                  // Get word information
                  WordItem word_information = myHashTable.getvalue(HASH_words[a]);
                  
                  for (int c = 0; c < word_information.documents.size(); c++){
                      
                      WordOutput temp;
                      temp.documentName = word_information.documents[c].documentName;
                      temp.count = word_information.documents[c].count;
                      temp.word = HASH_words[a];
                      HASH_word_details.push_back(temp);
                  }
              }
          }
    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
          
  bool checkHASH = false;
  if (controlHASH){
      // Check if all words in query exist in any document
      for (int a = 0; a < files_name.size(); a++){
          
          int num = 0;
          for (int b = 0; b < HASH_words.size(); b++){
              if(isFoundWordInVector(HASH_word_details, files_name[a], HASH_words[b]))
                  num++;
          }
          if (num == HASH_words.size()){
              checkHASH = true;
          }
      }

      if (!checkHASH)
          cout << "No document contains the given query" << endl; // No document contains all the queried words
      else {
          // Print occurrences of queried words in each document
          HASH_found_word.clear();
          for (int i = 0; i < files_name.size(); i++){
              
              isWordInVector(HASH_word_details, HASH_found_word, files_name[i]);
              if (HASH_found_word.size()/20 == HASH_words.size()){
                  
                  cout << "in Document " << files_name[i] << ", ";
                  for (int j = 0; j < HASH_words.size(); j++){
                      
                      for (int k = 0; k < HASH_found_word.size()/20; k++){
                          
                          if (j != HASH_found_word.size()/20-1 && HASH_words[j] == HASH_found_word[k].word)
                              cout << HASH_words[j] << " found " << HASH_found_word[k].count << " times, ";
                          else if (j == HASH_found_word.size()/20-1 && HASH_words[j] == HASH_found_word[k].word)
                              cout << HASH_words[j] << " found " << HASH_found_word[k].count << " times." << endl;;
                      }
                  }
              }
              HASH_found_word.clear();
          }
     }
 }
        cout << "\nTime: " << BSTTime.count() / k << "\n";
        cout << "Time: " << HTTime.count() / k << "\n";
        cout << "Speed Up: " <<  (float) BSTTime.count() / HTTime.count( )<< endl;
    }
        cout << endl;
  }
    return 0;
}
// SÜLEYMAN BERBER
