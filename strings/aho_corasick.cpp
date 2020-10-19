#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_map>
#include "trie.hpp"
#include <thread>

using namespace std;

/**
PROBLEM: 
Find the list of words which occur as substrings in a given search string S.

ALGO:
Step1: build a trie with additional links called fail links
Step2: construct a finite state automata using memoization to
       store transition information from each state.

- The trie can be interpreted as a finite state automata 
  but which state do we go to when there is no outgoing edge
  upon reading a character from the given string?

- Intuitively you want to save as much work you have done in previous iterations.
  Thus, we must find the state corresponding to the longest proper suffix of the 
  string S and try to perform a transition via c from there.

- A fail link from a vertex p is the edge which points to the longest proper suffix in the trie.
- The root of the trie is a special case where the suffix link points to itself. 
- Now, whenever there is no transition from the current node upon reading c follow the fail link.
- The problem now reduces to building the fail links in a recursive manner. 
  The root and its children are edge cases all pointing to the root.

**/

class AhoTrie: public Trie {
    public:
    AhoTrie(){
        this->root = new TrieNode();
        this->root->parent = this->root;
        this->nodeCount = 0;
    }
    AhoTrie(vector<string> &words) {
        this->root = new TrieNode();
        this->root->parent = nullptr;
        this->nodeCount = 0;
        for (int i = 0; i < words.size(); ++i) {
            this->insert(words[i], i);
        }
    }

    void insert(const string&, int);
    TrieNode* transition(TrieNode* node, char ch);
    TrieNode* failLink(TrieNode* node);
};

void AhoTrie::insert(const string& word, int index) {
    TrieNode* node = this->root;
    for (int i = 0; i < word.size(); ++i) {
        if (node->next.find(word[i]) == node->next.end()) {
            TrieNode* child = new TrieNode();
            child->parent = node;
            child->chFromParent = word[i];
            this->nodeCount ++;
            node->next[word[i]] = child;
        }
        node = node->next[word[i]];
    }
    node->which = index;
    node->leaf = true;
}

TrieNode* AhoTrie::transition(TrieNode* node, char ch) {
    /**
     * Implements the automata transition from state node upon reading ch.
     * You can either follow a trie edge or a sequence of fail links.
    */
    
    if (node->trans.find(ch) == node->trans.end()) {
        if (node->next.find(ch) != node->next.end()) {
            node->trans[ch] = node->next[ch];
        } else {
            if (node == this->root) node->trans[ch] = node;
            else node->trans[ch] = transition(this->failLink(node), ch);
        }
    }
    return node->trans[ch];
}

TrieNode* AhoTrie::failLink(TrieNode* node) {
    /**
     * Function to return the node pointed to by the fail link.
     * The string corresponfing to this node is the longest 
     * proper suffix for the string associated with node.
    */
    if (node->failLink == nullptr) {
        if (node->parent == this->root) node->failLink = this->root;
        else node->failLink = transition(this->failLink(node->parent), node->chFromParent);
    }
    return node->failLink;
}


int main() {
    
    int n; cin >> n;
    vector<string> words(n);
    for (int i = 0; i < n; ++i) {
        cin >> words[i];
    }
    string text; cin >> text;

    if (text.empty()) return 0;
    
    AhoTrie trie = AhoTrie(words);
    vector<int> freq(n, 0); //store the number of occurrences for each word
    // traverse the automata upon iterating through the search string
    TrieNode* node = trie.root;
    for (int i = 0; i < text.size(); ++ i) {
        node = trie.transition(node, text[i]);
        if (node->leaf == true) freq[i]++;
    }

    for (int i = 0; i < n; ++i) {
        cout << freq[i] << " ";
    }
}