#include "compact_trie.hpp"

Node::Node(std::string _prefix)
{
    this->prefix = _prefix;
    this->childs = new std::map<char, Node *>;
    this->code = -1;
    this->codeStr = "";
}

Node::~Node()
{
    // Deallocates the allocated memory.
    delete this->childs;
}

CompactTrie::CompactTrie()
{
    this->universalCode = 0;
    this->root = new Node("");

    this->root->code = this->universalCode;
    this->universalCode++;
}

CompactTrie::~CompactTrie()
{
    // Deallocates the allocated memory.
    for (auto it = (*this->root->childs).begin(); it != (*this->root->childs).end(); it++)
    {
        this->destructorAux(it->second);
    }
    delete this->root;
}

void CompactTrie::destructorAux(Node *_root)
{
    // Auxiliates recursively in deallocating the allocated memory.
    for (auto it = (*_root->childs).begin(); it != (*_root->childs).end(); it++)
    {
        this->destructorAux(it->second);
    }

    delete _root;
}

void CompactTrie::insert(std::string word)
{
    // If the word is empty, ends the method (the root is the empty string).
    if (word == "")
    {
        return;
    }
    // Verifies if the word matches any prefix in the root.
    // If it does not match, just add the word as a new child.
    // If it matches, starts the search for the right spot.
    if ((*this->root->childs).find(word[0]) == (*this->root->childs).end())
    {
        (*this->root->childs)[word[0]] = new Node(word);

        // Updates the code.
        (*this->root->childs)[word[0]]->code = this->universalCode;
        this->universalCode++;
    }
    else
    {
        this->insertAux((*this->root->childs)[word[0]], word);
    }
}

void CompactTrie::insertAux(Node *_root, std::string word)
{
    int lenWord = word.length();
    int lenPrefix = _root->prefix.length();

    int i = 0;
    std::string commonPrefix = "";
    // Checks until which point do they match.
    while (word[i] == _root->prefix[i])
    {
        commonPrefix += word[i];

        if (lenWord == lenPrefix)
        {
            // If the word matches exactly the prefix and if it hasn't happened before,
            // ...we update the prefix's code to count the occurence of the word.
            if (i == lenWord - 1)
            {
                if (((*_root->childs).size() > 0) && (_root->code == -1))
                {
                    // Updates the code.
                    _root->code = this->universalCode;
                    this->universalCode++;
                }
                // Repeated words are simply not counted.
                return;
            }
        }
        else if (lenWord > lenPrefix)
        {
            // If the word matches exactly the prefix and exceeds it,
            // ...we search the childs with the word's sufix.
            if (i == lenPrefix - 1)
            {
                std::string wordSufix = word.substr(i + 1, lenWord - (i + 1));
                // If the prefix has no childs (i.e. is the sufix of a word).
                if ((*_root->childs).size() == 0)
                {
                    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);

                    // Updates the code.
                    (*_root->childs)[wordSufix[0]]->code = this->universalCode;
                    this->universalCode++;

                    return;
                }
                // If the word's sufix does not match any child.
                else if ((*_root->childs).find(wordSufix[0]) == (*_root->childs).end())
                {
                    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);

                    // Updates the code.
                    (*_root->childs)[wordSufix[0]]->code = this->universalCode;
                    this->universalCode++;

                    return;
                }
                // If it matches a child, continues the search within its subtree.
                else
                {
                    this->insertAux((*_root->childs)[wordSufix[0]], wordSufix);
                    return;
                }
            }
        }
        else
        {
            // If the word matches exactly the prefix but is smaller,
            // ...we rearrange the tree accordingly.
            if (i == lenWord - 1)
            {
                // Gathers the other part of the prefix.
                std::string prefixSufix = _root->prefix.substr(i + 1, lenPrefix - (i - 1));

                // Creates a new node for this sufix of the prefix.
                Node *newPrefix = new Node(prefixSufix);
                delete newPrefix->childs;
                newPrefix->childs = _root->childs;

                // Updates the code.
                newPrefix->code = _root->code;

                // Updates the root's prefix and reallocates its childs.
                _root->prefix = commonPrefix;
                _root->childs = new std::map<char, Node *>;
                (*_root->childs)[prefixSufix[0]] = newPrefix;

                // Updates the code.
                _root->code = this->universalCode;
                this->universalCode++;

                return;
            }
        }

        i++;
    }

    // If they disagree at some point, gathers the common prefix and updates the tree.
    std::string wordSufix = word.substr(i, lenWord - i);
    std::string prefixSufix = _root->prefix.substr(i, lenPrefix - i);

    // Creates a new node for the sufix of the prefix.
    Node *newPrefix = new Node(prefixSufix);
    delete newPrefix->childs;
    newPrefix->childs = _root->childs;

    // Updates the code.
    newPrefix->code = _root->code;
    _root->code = -1;

    // Updates the root's prefix and reallocates its childs.
    _root->prefix = commonPrefix;
    _root->childs = new std::map<char, Node *>;
    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);
    (*_root->childs)[prefixSufix[0]] = newPrefix;

    // Updates the code.
    (*_root->childs)[wordSufix[0]]->code = this->universalCode;
    this->universalCode++;

    return;
}

void CompactTrie::insertDec(std::string word, std::string wordCode)
{
    // If the word is empty, ends the method (the root is the empty string).
    if (word == "")
    {
        return;
    }
    // Verifies if the word matches any prefix in the root.
    // If it does not match, just add the word as a new child.
    // If it matches, starts the search for the right spot.
    if ((*this->root->childs).find(word[0]) == (*this->root->childs).end())
    {
        (*this->root->childs)[word[0]] = new Node(word);

        // Updates the code.
        (*this->root->childs)[word[0]]->codeStr = wordCode;
    }
    else
    {
        this->insertAuxDec((*this->root->childs)[word[0]], word, wordCode);
    }
}

void CompactTrie::insertAuxDec(Node *_root, std::string word, std::string wordCode)
{
    int lenWord = word.length();
    int lenPrefix = _root->prefix.length();

    int i = 0;
    std::string commonPrefix = "";
    // Checks until which point do they match.
    while (word[i] == _root->prefix[i])
    {
        commonPrefix += word[i];

        if (lenWord == lenPrefix)
        {
            // If the word matches exactly the prefix and if it hasn't happened before,
            // ...we update the prefix's code to count the occurence of the word.
            if (i == lenWord - 1)
            {
                if (((*_root->childs).size() > 0) && (_root->code == -1))
                {
                    // Updates the code.
                    _root->codeStr = wordCode;
                }
                // Repeated words are simply not counted.
                return;
            }
        }
        else if (lenWord > lenPrefix)
        {
            // If the word matches exactly the prefix and exceeds it,
            // ...we search the childs with the word's sufix.
            if (i == lenPrefix - 1)
            {
                std::string wordSufix = word.substr(i + 1, lenWord - (i + 1));
                // If the prefix has no childs (i.e. is the sufix of a word).
                if ((*_root->childs).size() == 0)
                {
                    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);

                    // Updates the code.
                    (*_root->childs)[wordSufix[0]]->codeStr = wordCode;

                    return;
                }
                // If the word's sufix does not match any child.
                else if ((*_root->childs).find(wordSufix[0]) == (*_root->childs).end())
                {
                    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);

                    // Updates the code.
                    (*_root->childs)[wordSufix[0]]->codeStr = wordCode;

                    return;
                }
                // If it matches a child, continues the search within its subtree.
                else
                {
                    this->insertAuxDec((*_root->childs)[wordSufix[0]], wordSufix, wordCode);
                    return;
                }
            }
        }
        else
        {
            // If the word matches exactly the prefix but is smaller,
            // ...we rearrange the tree accordingly.
            if (i == lenWord - 1)
            {
                // Gathers the other part of the prefix.
                std::string prefixSufix = _root->prefix.substr(i + 1, lenPrefix - (i - 1));

                // Creates a new node for this sufix of the prefix.
                Node *newPrefix = new Node(prefixSufix);
                delete newPrefix->childs;
                newPrefix->childs = _root->childs;

                // Updates the code.
                newPrefix->codeStr = _root->codeStr;

                // Updates the root's prefix and reallocates its childs.
                _root->prefix = commonPrefix;
                _root->childs = new std::map<char, Node *>;
                (*_root->childs)[prefixSufix[0]] = newPrefix;

                // Updates the code.
                _root->codeStr = wordCode;

                return;
            }
        }

        i++;
    }

    // If they disagree at some point, gathers the common prefix and updates the tree.
    std::string wordSufix = word.substr(i, lenWord - i);
    std::string prefixSufix = _root->prefix.substr(i, lenPrefix - i);

    // Creates a new node for the sufix of the prefix.
    Node *newPrefix = new Node(prefixSufix);
    delete newPrefix->childs;
    newPrefix->childs = _root->childs;

    // Updates the code.
    newPrefix->codeStr = _root->codeStr;
    _root->codeStr = "";

    // Updates the root's prefix and reallocates its childs.
    _root->prefix = commonPrefix;
    _root->childs = new std::map<char, Node *>;
    (*_root->childs)[wordSufix[0]] = new Node(wordSufix);
    (*_root->childs)[prefixSufix[0]] = newPrefix;

    // Updates the code.
    (*_root->childs)[wordSufix[0]]->codeStr = wordCode;

    return;
}

bool CompactTrie::search(std::string word)
{
    // If the tree is empty, the word can't be in it.
    if (this->root == nullptr)
    {
        return false;
    }
    // If the word is empty, it is at the root.
    else if (word == "")
    {
        return true;
    }
    // If the tree is not empty, verifies if the word match any prefix in it.
    // If it does not match, the pattern does not occur.
    // If it matches, starts the search in a subtree.
    else
    {
        if ((*this->root->childs).find(word[0]) == (*this->root->childs).end())
        {
            return false;
        }
        else
        {
            return this->searchAux((*this->root->childs)[word[0]], word);
        }
    }
}

bool CompactTrie::searchAux(Node *_root, std::string word)
{
    int lenWord = word.length();
    int lenPrefix = _root->prefix.length();

    int i = 0;
    // Checks until which point do they match.
    while (word[i] == _root->prefix[i])
    {
        // If the word matches exactly the prefix , it occurs.
        if (i == lenWord - 1)
        {
            return true;
        }
        // If the word matches exactly the prefix and exceeds it,
        // ...we search the childs with the word's sufix.
        if (i == lenPrefix - 1)
        {
            std::string wordSufix = word.substr(i + 1, lenWord - (i + 1));
            // If the prefix has no childs (i.e. is the sufix of a word),
            // or if the word's sufix does not match any child.
            // ...then the word does not occur.
            if (((*_root->childs).size() == 0) || ((*_root->childs).find(wordSufix[0]) == (*_root->childs).end()))
            {
                return false;
            }
            // If it matches a child, continues the search within its subtree.
            else
            {
                return this->searchAux((*_root->childs)[wordSufix[0]], wordSufix);
            }
        }
        i++;
    }
    // If they disagree at some point, it does not occur.
    return false;
}

int CompactTrie::getCode(std::string word)
{
    // If the prefix is the empty string.
    if (this->root->prefix == word)
    {
        return this->root->code;
    }
    else
    {
        // Searches for the word in the tree.
        return this->getCodeAux((*this->root->childs)[word[0]], word);
    }

    return -1;
}

int CompactTrie::getCodeAux(Node *_root, std::string word)
{
    int lenWord = word.length();
    int lenPrefix = _root->prefix.length();

    int i = 0;
    // Checks until which point do they match.
    // Assuming the word is in the tree,
    // ...it will match the prefix or exceed it.
    while (word[i] == _root->prefix[i])
    {
        // If the word is equal or smaller than the prefix, it is on the current node.
        if (i == lenWord - 1)
        {
            return _root->code;
        }
        // If the word exceeds the prefix, continue the search within its childs.
        if (i == lenPrefix - 1)
        {
            std::string wordSufix = word.substr(i + 1, lenWord - (i + 1));
            return this->getCodeAux((*_root->childs)[wordSufix[0]], wordSufix);
        }
        i++;
    }

    return -1;
}

std::string CompactTrie::getCodeDec(std::string word)
{
    // If the prefix is the empty string.
    if (this->root->prefix == word)
    {
        return this->root->codeStr;
    }
    else
    {
        // Searches for the word in the tree.
        return this->getCodeAuxDec((*this->root->childs)[word[0]], word);
    }

    return "";
}

std::string CompactTrie::getCodeAuxDec(Node *_root, std::string word)
{
    int lenWord = word.length();
    int lenPrefix = _root->prefix.length();

    int i = 0;
    // Checks until which point do they match.
    // Assuming the word is in the tree,
    // ...it will match the prefix or exceed it.
    while (word[i] == _root->prefix[i])
    {
        // If the word is equal or smaller than the prefix, it is on the current node.
        if (i == lenWord - 1)
        {
            return _root->codeStr;
        }
        // If the word exceeds the prefix, continue the search within its childs.
        if (i == lenPrefix - 1)
        {
            std::string wordSufix = word.substr(i + 1, lenWord - (i + 1));
            return this->getCodeAuxDec((*_root->childs)[wordSufix[0]], wordSufix);
        }
        i++;
    }

    return "";
}

int CompactTrie::getUniversalCode()
{
    return this->universalCode;
}