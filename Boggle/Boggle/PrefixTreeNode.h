#pragma once
#include <string>

class PrefixTreeNode
{
public:

    std::string word;
    PrefixTreeNode** children;

    PrefixTreeNode();
    ~PrefixTreeNode();
};

