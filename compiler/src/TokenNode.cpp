#include "TokenNode.h"

TokenNode::~TokenNode()
{
}

compiler::TOKEN TokenNode::getToken()
{
    return this->_token;
}
