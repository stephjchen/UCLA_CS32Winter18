
#include "provided.h"
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
    
private:
    unordered_set<char> m_separators;
    bool isSeparator(const char c) const;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i=0; i < separators.size(); i++)
        m_separators.insert(separators[i]);
}

bool TokenizerImpl::isSeparator(const char c) const
{
    unordered_set<char> :: iterator it;
    it = m_separators.find(c);
    
    if (it != m_separators.end())
        return true;
    else
        return false;
}


vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    string temp = "";
    vector <string> tokens;
    
    for (int i = 0; i< s.size(); i++)
    {
        if (!isSeparator(s[i])) //if the current char is not a separator
        {
            temp += s[i]; //append it to empty string, continue to do so until a separator is encountered
            continue;
        }
        
        if(temp != "")
            tokens.push_back(temp);
        temp = "";
    }
    return tokens;
    
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

