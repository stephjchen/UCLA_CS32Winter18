#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;


class DecrypterImpl
{
public:
    DecrypterImpl();
    ~DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    
    void decryptHelper(string const& ciphertext, const vector<string>& cipherwords,Translator& curMapping, vector<string>& solutionVec);
    
    void eraseUntranslated(vector<string>& strVec);
    int count_qmarks(string const& s);
    
    WordList* m_wl;
    Tokenizer m_tokenizer;
};

DecrypterImpl::DecrypterImpl(): m_wl(), m_tokenizer(",;:.!()[]{}-\"#$%^& 0123456789")
{}

DecrypterImpl::~DecrypterImpl()
{
    if (m_wl != nullptr)
        delete m_wl;
}

bool DecrypterImpl::load(string filename)
{
    if (m_wl != nullptr) //discard old word list if needed
        delete m_wl;
    
    m_wl = new WordList; //dynamically allocate a new wordlist
    
    if( m_wl->loadWordList(filename))
        return true;
    
    return false;
}


int DecrypterImpl::count_qmarks(string const& s) {
    return count_if(s.cbegin(), s.cend(), [] (char c){return c == '?';});
}

void DecrypterImpl::eraseUntranslated(vector<string>& strVec)
{
    vector<string> :: iterator it;
    
    for (it = strVec.begin(); it != strVec.end();)
    {
        bool erased = false;
        for (int i = 0; i < (*it).size(); i++)
        {
            if ( ((*it)[i]) == '?' )
            {
                it = strVec.erase(it);
                erased = true;
                break;
            }
        }
        
        if (!erased)
            it++;
    }
}


void DecrypterImpl::decryptHelper(const string& ciphertext, const vector<string>& cipherwords,Translator& curMapping,vector<string>& solutionVec) //directly update solutionVec with each recursive call
{
    
    string curDecoded = curMapping.getTranslation(ciphertext);
    vector<string> curDecodedTok = m_tokenizer.tokenize(curDecoded);
    
    int chosen_index = 0;
    for (int i = 0; i < curDecodedTok.size(); i++)
    {
        int qmarks = count_qmarks(curDecodedTok[i]); //for each word in the current translation, mark the number of '?' still pressent
        
        if (qmarks > count_qmarks(curDecodedTok[chosen_index]) )
        {
            chosen_index = i;
        }
    }
    
    //choose candidate base on the word with the most '?'
        string chosen_cipherword = cipherwords[chosen_index];
        string chosen_curDecoded = curDecodedTok[chosen_index];
    
        vector<string> candidates = m_wl->findCandidates(chosen_cipherword, chosen_curDecoded);
    
        if (candidates.empty()) //since no candidate are found, discard current mapping
        {
            curMapping.popMapping();
            return;
        }
    
        //traverse though each candidate found from wordlist
        for (auto const& candidate : candidates) {
            
            bool r = curMapping.pushMapping(chosen_cipherword, candidate);
            if (!r)
                continue; //trying another candidate
            
            //evaluate just decrypted message
            string justDecryptedMessge = curMapping.getTranslation(ciphertext);
            
            vector<string> decryptedWords = m_tokenizer.tokenize(justDecryptedMessge);
            
            //evaluate just decrypted text
            int total_number_of_words = (int)decryptedWords.size();
            eraseUntranslated(decryptedWords); //only look at translated words
            int fully_translated_number_of_words = (int)decryptedWords.size();
            
            bool notContained = false;
            for (int i = 0; i < decryptedWords.size(); i++)
            {
                if (!m_wl->contains(decryptedWords[i]))
                {
                    notContained = true;
                    curMapping.popMapping();
                    break;
                }
            }
            if (notContained == true)
                continue;
            
            if (all_of(decryptedWords.begin(), decryptedWords.end(), [this] (string const& word) {
                return m_wl->contains(word);
            }) && fully_translated_number_of_words < total_number_of_words )
            {
                decryptHelper(ciphertext, cipherwords, curMapping, solutionVec);
            }
            
            if (total_number_of_words == fully_translated_number_of_words)
            {
                
                solutionVec.push_back(justDecryptedMessge);
                curMapping.popMapping();
            }
        
    }
    curMapping.popMapping();
}


vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    //initialize new translator with empty mapping
    Translator t;
    
    //empty vector
    vector<string> solutionVec;
    vector<string> cipherwords = m_tokenizer.tokenize(ciphertext);
    
    decryptHelper(ciphertext, cipherwords, t, solutionVec);
    
    sort(solutionVec.begin(), solutionVec.end());
    
    return solutionVec;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}
