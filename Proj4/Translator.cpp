#include "provided.h"
#include <string>
#include <map>
#include <cctype>
#include <iostream>
using namespace std;


class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    bool checkDupMapping (const char& cw, const char& pw, const map<char,char>& currMap) const;
    map<char,char> createMap();
    
    bool caseInsensitiveStringCompare(const string& str1, const string& str2) const;
    bool caseInsensitiveCharCompare(const char& chr1, const char& chr2) const;
    
    vector<map<char, char>> m_mapping;
    int numPop;
    int numPush;
};

bool TranslatorImpl::caseInsensitiveStringCompare(const string& str1, const string& str2) const {
    
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

bool TranslatorImpl::caseInsensitiveCharCompare(const char& chr1, const char& chr2) const
{
    if ( tolower(chr1) != tolower(chr2) )
        return false;
    return true;
}

TranslatorImpl::TranslatorImpl()
{
    map<char,char> map = createMap();
    m_mapping.push_back(map); //insert map into vector
    numPush = 0;
    numPop = 0;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if (ciphertext.size() != plaintext.size())
        return false;
    
    map<char,char> temp = m_mapping.back(); //current map
    
    for (int i = 0; i < ciphertext.size(); i++)
    {
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i])) //can only make alphabet associations
            return false;
        else
        {
            char ct = toupper(ciphertext[i]);
            char pt = toupper(plaintext[i]);
            
            //if mapping for one letter result in inconsistency, do not push mapping
            if (checkDupMapping(ct, pt, m_mapping.back()))
                return false;
            
            map<char,char> :: iterator temp_it;
            temp_it = temp.find(ct);
            temp_it -> second = pt;
        }
    }
    
    m_mapping.push_back(temp);
    numPush++; //update the number of time pushed
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (numPush == numPop)
        return false;
    
    numPop++;
    
    m_mapping.pop_back();
    
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string s = "";
        
    for (int i=0; i < ciphertext.size(); i++)
    {
        if (isalpha(ciphertext[i]))
        {
            if (islower(ciphertext[i]))
                s += tolower(m_mapping.back().find(toupper(ciphertext[i]))->second); //append the value that is associated with targeted letter of ciphertext, converted to lowercase if the ciphertext is a lowercase
        
            else if ( isupper(ciphertext[i]) )
                s += ( toupper(m_mapping.back().find(ciphertext[i])->second) ) ; //append mapping as uppercase if the ciphertext is an uppercase
        }
        
        else
            s += ciphertext[i]; //append original value of ciphertext if it is not a letter
    }
    
    return s;
}

//Private Member Function Implementation

bool TranslatorImpl::checkDupMapping (const char& cw, const char& pw, const map<char,char>& currMap) const
{
    map<char,char> :: const_iterator m_it;

    for (m_it = currMap.begin(); m_it != currMap.end(); m_it++)
    {
        if ( caseInsensitiveCharCompare(cw, m_it->first) )
        {
            if ( isalpha(m_it->second) && !caseInsensitiveCharCompare(m_it->second, pw))
                return true; //check whether the plain text letter and current mapping are the same, returning true indicates there is duplicate mapping
        }
        
        else if ( caseInsensitiveCharCompare(pw, m_it->second) )
        {
            if (isalpha(m_it->first) && !caseInsensitiveCharCompare(m_it->first, cw))
                return true; //check whether two cipherword letters are being mapped to the same value
        }

    }
    return false; //no dup mapping
}

map<char,char> TranslatorImpl::createMap()
{
    map<char,char> m;
    const char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    for (int i = 0; i < 26; i++)
        m.insert(pair<char,char> (alphabets[i],'?'));
    
    return m;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
