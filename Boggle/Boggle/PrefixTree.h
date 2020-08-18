#pragma once
#include <string>
#include "PrefixTreeNode.h"

class PrefixTree
{
public:
    PrefixTree();
    ~PrefixTree();

    PrefixTreeNode* root;

    void Insert(std::string word) const;
};

