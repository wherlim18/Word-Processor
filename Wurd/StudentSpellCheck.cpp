#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
    
    m_trie->Destroy(m_trie->getRoot());
    
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    
    m_trie->Destroy(m_trie->getRoot());
    
    ifstream infile("/Users/wherlim18/Desktop/Wurd/dictionary.txt");    // infile is a name of our choosing
        if ( ! infile )                // Did opening the file fail?
        {
            cerr << "Error: Cannot open dictionary.txt!" << endl;
            return false;
        }
    
    string s;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, s))
    {
        m_trie->insert(s);
    }
    
	return true; // TODO
    
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
    
    if(m_trie->search(word))
        return true;

    suggestions.clear();

    string test = word;

    int counter = 0;

    for(int i = 0; i < word.size(); i++)
    {
        test[i] = 'a';

        for(int j = 0; j < 25; j++)
        {
            test[i]++;
            
            if(m_trie->search(test))
            {
                if(counter < max_suggestions)
                {
                    suggestions.push_back(test);
                    counter++;
                }
                else
                    return false;
            }
        }
        test[i] = word[i];
    }

    
	return false; // TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// TODO
    
    problems.clear();
    
    int start = 0;
    
    for(int i = 0; i < line.size(); i++)
    {
        string temp;
        
        if(!isalpha(line[i]) && line[i] != '\'')
        {
            temp = line.substr(start, i-start);
            
            if(!m_trie->search(temp))
            {
                Position pos;
                pos.start = start;
                pos.end  = i-1;
                
                problems.push_back(pos); 
            }
            
            start = i+1;
        }
    }
    
    string temp = line.substr(start, line.size()-start);
    if(!m_trie->search(temp))
    {
        Position pos;
        pos.start = start;
        pos.end  = line.size()-1;
        
        problems.push_back(pos);
    }
}

