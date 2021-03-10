#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct TrieNode
    {
        TrieNode* children[27] = {nullptr};
        bool isWord = false;
    };

    struct Trie
    {
        Trie()
        {
            m_root = new TrieNode();
        }

        void insert(string word)
        {
            if(m_root == nullptr)
            {
                m_root = new TrieNode();
            }

            TrieNode* cur = m_root;

            for(int i = 0; i < word.size(); i++)
            {
                char c = tolower(word[i]);
                
                if(c == '\'')
                {
                    if(cur->children[26] == nullptr)
                        cur->children[26] = new TrieNode();
                    
                    cur = cur->children[26];
                }
                else
                {
                    if(cur->children[c-'a'] == nullptr)
                        cur->children[c-'a'] = new TrieNode();

                    cur = cur->children[c-'a'];
                }
            }
            cur->isWord = true;
        }

        bool search(string word)
        {
            TrieNode* cur = m_root;

            for(int i = 0; i < word.size(); i++)
            {
                char c = tolower(word[i]);
                
                int index = c - 'a';
                
                string test = word;
                
                if(c == '\'')
                {
                    if(cur->children[26] == nullptr)
                        return false;
                    
                    cur = cur->children[26];
                }
                else
                {
                    if(cur->children[index] == nullptr)
                        return false;

                    cur = cur->children[index];
                }
            }

            return cur->isWord;
        }

        void Destroy(TrieNode* node)
        {
            if(node == nullptr)
                return;
            
            for(int i = 0; i < 27; i++)
            {
                Destroy(node->children[i]);
            }
            delete node;
        }

        TrieNode* getRoot()
        {
            return m_root;
        }
        
    private:
        TrieNode* m_root = nullptr;
    };

    Trie* m_trie = new Trie();
    
};


#endif  // STUDENTSPELLCHECK_H_
