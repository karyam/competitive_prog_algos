#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_map>

using namespace std;

struct TrieNode{
    bool leaf;
    int which;
    unordered_map<char, TrieNode*> next;
    // members only used by AhoTrie
    TrieNode* parent;
    char chFromParent;
    TrieNode* failLink;
    unordered_map<char, TrieNode*> trans;

    TrieNode() {
        this->leaf = false;
        this->which = -1;
        this->next = {};
        this->parent = nullptr;
        this->failLink = nullptr;
        this->trans = {};
    }
};

class Trie {
    public:
    Trie() {
        this->root = new TrieNode();
        this->root->parent = this->root;
        this->nodeCount = 0;
    }

    Trie(const vector<string>& words) {
        this->root = new TrieNode();
        this->nodeCount = 0;
        for (string word : words) {
            this->insert(word);
        }
    }
    void insert(const string&);
    bool search(const string&);
    void deletion(TrieNode*, string);

    TrieNode* root;
    int nodeCount;
};

void Trie::insert(const string& word) {
    TrieNode* node = this->root;
    for (int i = 0; i < word.size(); ++i) {
        if (node->next.find(word[i]) == node->next.end()) {
            TrieNode* child = new TrieNode();
            this->nodeCount ++;
            node->next[word[i]] = child;
        }
        node = node->next[word[i]];
    }
    node->leaf = true;
}

bool Trie::search(const string& word) {
    TrieNode* node = this->root;
    for (int i = 0; i < word.size(); ++i) {
        if (node->next.find(word[i]) == node->next.end()) {
            return false;
        }
        node = node->next[word[i]];
    }
    return node->leaf;
}