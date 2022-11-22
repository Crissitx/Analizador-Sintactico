#include <iostream>
#include <vector>
using namespace std;

class Node
{
private:

public:
    string data;
    int tokenType;
    Node* child1;
    Node* child2;
    Node* child3;
    vector<string> children;
    Node(string val);
    ~Node();
};

Node::Node(string val)
{
    data = val;
    tokenType = -1;
    child1 = NULL;
    child2 = NULL;
    child3 = NULL;
    children = {};
}

Node::~Node()
{
}
