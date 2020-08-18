#include "PrefixTree.h"
#include <iostream>

PrefixTree::PrefixTree()
{
    root = new PrefixTreeNode();
}

PrefixTree::~PrefixTree()
{
    delete root;
}

void PrefixTree::Insert(std::string word) const
{
    auto currentNode = root;

    for (char letter : word)
    {
        auto letterIndex = letter - 'a';

        if (currentNode->children == nullptr)
        {
            currentNode->children = new PrefixTreeNode*[26] { nullptr };
        }

        if (currentNode->children[letterIndex] == nullptr)
        {
            currentNode->children[letterIndex] = new PrefixTreeNode();
        }

        currentNode = currentNode->children[letterIndex];
    }

    currentNode->word = word;
}
