#include "PrefixTreeNode.h"

PrefixTreeNode::PrefixTreeNode()
{
    children = { nullptr };
}


PrefixTreeNode::~PrefixTreeNode()
{
    for (int i = 0; i < 26; i++)
    {
        delete[] children[i];
    }

    delete[] children;
}
