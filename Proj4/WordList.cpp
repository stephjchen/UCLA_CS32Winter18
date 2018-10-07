#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <iostream>
#include <fstream>
#include "MyHash.h"
using namespace std;

class WordListImpl
{
public:
    
    ~WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
    
private:
    
    void checkDuplicate (const string& w, vector<string>& vec);
    bool validWord(const string& w) const;
    bool validTranslation(const string& w) const;
    static string pattern(const string& s) ;
    
    bool caseInsensitiveStringCompare(const string& str1, const string& str2) const;
    bool caseInsensitiveCharCompare(const char& chr1, const char& chr2) const;
    
    MyHash<string,vector<string>> m_hash;
    vector<vector<string>*> m_todelete;
    
};

WordListImpl::~WordListImpl()
{
    for (int i = 0; i < m_todelete.size(); i++)
    {
        delete m_todelete[i];
    }
}

bool WordListImpl::caseInsensitiveStringCompare(const string& str1, const string& str2) const {
    
    if (str1.size() != str2.size())
    {
        return false;
    }
    
    for (string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
    {
        if (tolower(*c1) != tolower(*c2)) {
            return false;
        }
    }
    return true;
}

bool WordListImpl::caseInsensitiveCharCompare(const char& chr1, const char& chr2) const
{
    
    if ( tolower(chr1) != tolower(chr2) )
        return false;
    return true;
}

bool WordListImpl::validWord(const string& w) const
{
    //a valid word only contains alphabets and apostrophes
    for (int i=0; i < w.size(); i++)
    {
        if (!isalpha(w[i]))
            if (w[i]!='\'')
                return false;
    }
    
    return true;
}

bool WordListImpl::validTranslation(const string& w) const
{
    //a valid word only contains alphabets, apostrophes, and question marks
    for (int i=0; i < w.size(); i++)
    {
        if (!isalpha(w[i]))
            if (w[i]!='\'' && w[i]!='?')
                return false;
        
    }
    
    return true;
}


void WordListImpl::checkDuplicate (const string& w, vector<string> & vec)
{
    vector<string> :: const_iterator it;
    
    //use stl vector find function to see whether word is already present in given vector
    it = find(vec.begin(), vec.end(), w);
    
    //if found, erase to avoid storing duplicate words in the same vector
    if (it != vec.end())
        it = vec.erase(it);
    
}


string WordListImpl::pattern(const string& s)
{
    MyHash<char, char> m_labels;
    
    string word = s;
    string upper = "";
    
    for (int i=0; i < word.size(); i++) //made to uppercase
    {
        char temp = toupper(word[i]);
        upper += toupper(temp);
    }
    
    const char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int count = 0;
    
    for (int i = 0; i < upper.size(); i++)
    {
        char * k = m_labels.find(upper[i]);
        
        //no valuetype associated with letter upper[i]
        if (k == nullptr)
        {
            if (isalpha(upper[i]))
            {
                m_labels.associate(upper[i], alphabets[count]);
                count++;
                
                
            }
            else if ( upper[i] == '\'')
                m_labels.associate(upper[i], '\'');
        }
        
    }
    
    string value = "";
    for (int i=0; i < upper.size(); i++)
    {
        char * temp = m_labels.find(upper[i]); //get corresponding letter pattern mapping of each word
        {
            char mapping = *temp;
            value += mapping; //append to a temporary string
        }
    }
    
    return value;
}

bool WordListImpl::loadWordList(string filename)
{
    ifstream infile(filename);
    if ( ! infile )
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    m_hash.reset(); 
    
    string s = "";
    string add = "";
    vector<string> words;
    
    while (getline(infile, add))
    {
        s += add + " ";
        add = "";
    }
    
    Tokenizer split(" ");
    words = split.tokenize(s); //use tokenizer to obtain a vector of the actual words in wordlist

    
    for (int i=0; i < words.size(); i++)
    {
        if (validWord(words[i]))
        {
            string key = pattern(words[i]);
            vector<string>* temp = m_hash.find(key);
            
            if (temp == nullptr)
            {
                temp = new vector<string>; //dynamically allocate new vector of strings
                m_todelete.push_back(temp); //add to vector of pointers to the vector
                temp->push_back(words[i]);
                m_hash.associate(key, *temp);//make new association
            }
            else
            {
                checkDuplicate(words[i], *temp);//check if a word already exist in a vector
                temp->push_back(words[i]);
            }
        }
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    
    string k = pattern(word);

    const vector<string>* vp = m_hash.find(k); //returns a vector of words with same pattern
    
    if (vp == nullptr)
        return false;
    
    else
    {
        vector<string> :: const_iterator it;
        
        for (it = (*vp).begin(); it != (*vp).end(); it++) //traverse through the vector to see if target word is present
        {
            if ( caseInsensitiveStringCompare((*it), word) )
                return true;
        }
        return false;
    }
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation)  const
{
    vector<string> vec; //initialize an empty vector to store all potential candidates
    
    if (!validWord(cipherWord)||!validTranslation(currTranslation)||cipherWord.size()!=currTranslation.size())
            return vec;
    
    string k = pattern(cipherWord);
    
    const vector<string>* vp = m_hash.find(k);
    
    if (vp == nullptr) //there are no candidate since no other words have the same letter pattern
        return vec;
    
    else
    {
        for (int j=0; j < currTranslation.size(); j++) //check if there is even a possibility for a candidate by checking validity of currentTranslation and cipherWord
        {
            if (isalpha(currTranslation[j]) || currTranslation[j] == '?')
            {
                if (!isalpha(cipherWord[j]))
                    return vec;
            }
            else if (currTranslation[j] == '\'')
            {
                if (cipherWord[j] != '\'')
                    return vec;
            }
        }
        
        //compare current translation with each word from vector
        vector<string> :: const_iterator it;
        
        for (it = (*vp).begin(); it != (*vp).end(); it++) //iterate through words
        {
            int i = 0;
            for (; i < currTranslation.size(); i++)
            {
                if ( currTranslation[i] == '?' && !isalpha((*it)[i]) )
                    break;
                
                else if (isalpha(currTranslation[i]) || currTranslation[i] == '\'')
                    if ( !caseInsensitiveCharCompare(currTranslation[i], ((*it)[i])) )
                        break;
            }
            
            if (i == currTranslation.size())
                vec.push_back(*it);
        }
        
        return vec;
    }
    
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}

