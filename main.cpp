#include <bits/stdc++.h>
using namespace std;

//struct for node of a tree
typedef struct node
{
    string data;
    struct node *left;
    struct node *right;
} node;

bool isNumber(string s);
int findType(string ch);
bool isOperator(string str);
int precedance(string a);

void inOrder(node *tree);
void preOrder(node *node);

string removeSpaces(string str);
vector<string> getOperands(string ip_string);
vector<string> inToPost(vector<string> infix);
vector<string> postfix(string in_string);

node *constructTree(string eqn);

node *copyNode(node *Node);
node *exponentDerivative(node *Node);
node *findDerivative(node *Node);
node *derivative(string eqn);

void getDerivative();

int main()
{
    getDerivative();
    return 0;
}

/***********************************************************************************************/

//returns true if the string passes is a number
bool isNumber(string s)
{
    auto it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

//returns -1 if the given string is operator or returns 1 (operand string)
int findType(string ch)
{

    if (ch == "+" || ch == "-" || ch == "*" || ch == "/" || ch == "^")
    {
        //cout << __LINE__ << endl;
        return -1; //returns -1 if string is operator
    }
    else
    {
        //cout << __LINE__ << endl;
        return 1;
    }
}

//determines if the given string is a operand or not
bool isOperator(string str)
{
    return (str == "+" || str == "-" || str == "/" || str == "*" || str == "^");
}

//returns the precedance of the operator
int precedance(string a)
{
    if (a == "^")
        return 3;
    else if (a == "*" || a == "/")
        return 2;
    else if (a == "+" || a == "-")
        return 1;
}

/***********************************************************************************************/

/***********************************************************************************************/

//prints the inorder traversal of the tree
void inOrder(node *tree)
{
    if (tree != NULL)
    {

        inOrder(tree->left);
        cout << tree->data << " ";
        inOrder(tree->right);
    }
    else
    {
        return;
    }
}

//prints the preorder traversal of the tree
void preOrder(node *node)
{
    if (node == NULL)
        return;

    printf("%s ", node->data);

    preOrder(node->left);

    preOrder(node->right);
}

/***********************************************************************************************/

//removes all the whitespace characters(" ","\n","\t") from the string
string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

//the function getOperands takes the input string
//and returns a vector of strings which contains all the operands and operators in infix order
vector<string> getOperands(string ip_string)
{
    vector<string> operands; //defining a vector to contain all the operand and operators
    int i = 0, start = 0, end = 0;
    string s1;
    for (i = 0; i < ip_string.length(); i++)
    {
        if (ip_string[i] == ' ') //parsing the string whenever a space is encountered
        {
            end = i - start;                   // length of the string (operator / operand)
            s1 = ip_string.substr(start, end); // creating a substring (operator / operand)
            s1 = removeSpaces(s1);
            if (s1.length() != 0)
            {
                operands.push_back(s1); // if the string is non-empty then append it into the vector
            }
            //cout << "start : " << start << "str1:" << s1 << "end : " << end << endl;
            start = i + 1;
        }
    }
    s1 = ip_string.substr(start, ip_string.length() - start);
    s1 = removeSpaces(s1);
    operands.push_back(s1);
    return operands;
}

//converts the given infix string into a postfix string
vector<string> inToPost(vector<string> infix)
{
    int i;
    string temp;
    stack<string> stk;
    vector<string> post;
    for (i = 0; i < infix.size(); i++)
    {
        if (isOperator(infix[i])) //operator
        {
            while (!stk.empty() && precedance(infix[i]) <= precedance(stk.top()))
            {
                temp = stk.top();
                stk.pop();
                post.push_back(temp);
            }
            stk.push(infix[i]);
        }
        else if (infix[i] == "(") // ( bracket
        {
            stk.push(infix[i]);
        }
        else if (infix[i] == ")") // ) bracket
        {
            while (!stk.empty() && stk.top() != "(")
            {
                temp = stk.top();
                stk.pop();
                post.push_back(temp);
            }
            if (!stk.empty() && stk.top() != "(")
            {
                printf("\n Invalid statement!!");
                exit(0);
            }
            else
                stk.pop();
        }
        else //operand
        {
            post.push_back(infix[i]);
        }
    }

    while (!stk.empty())
    {
        temp = stk.top();
        stk.pop();
        post.push_back(temp);
    }

    // for (int j = 0; j < post.size(); ++j)
    //     cout << " " << post[j];
    return post;
}

//driver code which takes input as a string and returns a vector of type string of postfix expression
vector<string> postfix(string in_string)
{
    vector<string> operands;
    vector<string> post;
    operands = getOperands(in_string);
    post = inToPost(operands);
    return post;
}

/***********************************************************************************************/

/***********************************************************************************************/

//takes a postfix expression as a vector of strings and constructs a expression tree using stack
node *constructTree(string eqn)
{
    vector<string> post = postfix(eqn);
    stack<node *> stk;
    node *new1, *p1, *p2;
    auto i = post.begin();
    string s = *i;
    int flag;
    for (i++; i != post.end(); i++)
    {
        //cout << "s-->" << s << "<--" << __LINE__ << endl;
        flag = findType(s);
        if (flag == 1)
        {
            //cout << __LINE__ << endl;
            new1 = new node();
            new1->data = s;
            new1->left = NULL;
            new1->right = NULL;
            stk.push(new1);
        }
        else
        {
            //cout << __LINE__ << endl;
            p1 = stk.top();
            stk.pop();
            p2 = stk.top();
            stk.pop();
            new1 = new node();
            new1->data = s;
            new1->left = p2;
            new1->right = p1;
            // cout<<"right"<<p1->data<<endl;
            // cout<<"left"<<p2->data<<endl;
            // cout<<"s"<<s<<endl;
            stk.push(new1);
        }
        s = *i;
        // cout << "top stack : " << ((stk.top()->data)) << endl;
        // cout << "stack size : " << stk.size() << endl
        //      << endl;
    }
    // cout << "End of loop " << endl;
    // cout << __LINE__ << endl;
    p1 = stk.top();
    stk.pop();
    p2 = stk.top();
    stk.pop();
    new1 = new node();
    new1->data = s;
    new1->left = p2;
    new1->right = p1;
    stk.push(new1);
    // cout<<"right"<<p1->data<<endl;
    // cout<<"left"<<p2->data<<endl;
    // cout<<"s"<<s<<endl;

    // cout << "top stack" << (stk.top()->data) << endl;
    // cout << "stack size : " << stk.size() << endl
    //      << endl;

    return stk.top();
}

/***********************************************************************************************/

/***********************************************************************************************/

//function to copy the tree as it is
node *copyNode(node *Node)
{
    if (Node == NULL)
        return NULL;
    else
    {
        node *temp = new node();
        temp->data = Node->data;
        temp->left = copyNode(Node->left);
        temp->right = copyNode(Node->right);
        return temp;
    }
}

//function defined for calculating the derivatives of functions which includes exponential function
//case 1 : e ^ x
//case 2 : a ^ x
//case 3 : f(x) ^ x
node *exponentDerivative(node *Node)
{
    node *newNode, *newNodeL, *newNodeR, *newNodeChL, *newNodeChR;
    string opr1 = Node->left->data, opr2 = Node->right->data;
    stringstream ss;
    int num1;
    if (opr1 == "e")
    {
        newNode = new node();
        newNode = copyNode(Node);
    }
    else if (isNumber(opr1))
    {
        newNode = new node();
        newNodeL = new node();
        newNodeR = new node();

        newNode->data = "*";
        newNode->left = newNodeL;
        newNode->right = newNodeR;

        newNodeR->data = "log(" + opr1 + ")";
        newNodeR->left = NULL;
        newNodeR->right = NULL;

        newNodeChL = new node();
        newNodeChR = new node();

        newNodeL->data = "^";
        newNodeL->left = newNodeChL;
        newNodeL->right = newNodeChR;

        newNodeChL->data = opr1;
        newNodeChL->left = NULL;
        newNodeChL->right = NULL;

        newNodeChR->data = opr2;
        newNodeChR->left = NULL;
        newNodeChR->right = NULL;
    }
    else
    {
        newNode = new node();
        newNodeL = new node();
        newNodeR = new node();

        newNode->data = "*";
        newNode->left = newNodeL;
        newNode->right = newNodeR;

        newNodeL->data = opr2;
        newNodeL->left = NULL;
        newNodeL->right = NULL;

        newNodeChL = new node();
        newNodeChR = new node();

        newNodeR->data = "^";
        newNodeR->left = newNodeChL;
        newNodeR->right = newNodeChR;

        newNodeChL->data = opr1;
        newNodeChL->left = NULL;
        newNodeChL->right = NULL;

        num1 = stoi(Node->right->data);
        num1 -= 1;
        ss << num1;
        string str = ss.str();

        newNodeChR->data = str;
        newNodeChR->left = NULL;
        newNodeChR->right = NULL;
    }

    return newNode;
}

//function which takes root of expression tree as input and recursively finds the derivative of given
//expression tree
node *findDerivative(node *Node)
{
    node *newNode, *newNodeL, *newNodeR, *newNodeChL, *newNodeChR;
    stringstream ss;
    int num1;
    string operand;
    if (isOperator(Node->data))
    //the node is operator node and recursively find left and right child's derivative
    {
        if (Node->data == "+" || Node->data == "-")
        {
            newNode = new node();
            newNode->data = Node->data;
            newNode->left = findDerivative(Node->left);
            newNode->right = findDerivative(Node->right);
        }
        else if (Node->data == "*") //multiplication rule
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "+";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "*";
            newNodeL->left = copyNode(Node->left);
            newNodeL->right = findDerivative(Node->right);

            newNodeR->data = "*";
            newNodeR->left = findDerivative(Node->left);
            newNodeR->right = copyNode(Node->right);
        }

        //exponentDerivative function called for finding the derivative of functions which include
        //exponential functions
        else if (Node->data == "^")
        {
            newNode = exponentDerivative(Node);
        }

        return newNode;
    }
    else //the node is operand node return a node* to its derivative
    {
        operand = Node->data;
        if (isNumber(operand)) // return 0 as derivative of constant
        {
            newNode = new node();
            newNode->data = "0";
            newNode->left = NULL;
            newNode->right = NULL;
        }

        else if (operand == "sinx") // sinx --> cosx
        {
            newNode = new node();
            newNode->data = "cosx";
            newNode->left = NULL;
            newNode->right = NULL;
        }
        else if (operand == "cosx") // cosx --> (-1) * sinx
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "-1";
            newNodeL->left = NULL;
            newNodeL->right = NULL;

            newNodeR->data = "sinx";
            newNodeR->left = NULL;
            newNodeR->right = NULL;
        }
        else if (operand == "tanx") //tanx --> (secx) ^ 2
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "^";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "secx";
            newNodeL->left = NULL;
            newNodeL->right = NULL;

            newNodeR->data = "2";
            newNodeR->left = NULL;
            newNodeR->right = NULL;
        }
        else if (operand == "logx") //logx --> (x) ^ (-1) i.e. 1/x
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "^";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "x";
            newNodeL->left = NULL;
            newNodeL->right = NULL;

            newNodeR->data = "-1";
            newNodeR->left = NULL;
            newNodeR->right = NULL;
        }
        else if (operand == "secx") //secx --> tanx * secx
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "tanx";
            newNodeL->left = NULL;
            newNodeL->right = NULL;

            newNodeR->data = "secx";
            newNodeR->left = NULL;
            newNodeR->right = NULL;
        }

        else if (operand == "cosecx") //cosecx --> (-1) * cosecx * cotx
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeR->data = "cosecx";
            newNodeR->left = NULL;
            newNodeR->right = NULL;

            newNodeChL = new node();
            newNodeChR = new node();

            newNodeL->data = "*";
            newNodeL->left = newNodeChL;
            newNodeL->right = newNodeChR;

            newNodeChL->data = "-1";
            newNodeChL->left = NULL;
            newNodeChL->right = NULL;

            newNodeChR->data = "cotx";
            newNodeChR->left = NULL;
            newNodeChR->right = NULL;
        }
        else if (operand == "cotx") //cotx --> (-1) * ((cosecx) ^ 2)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "-1";
            newNodeL->left = NULL;
            newNodeL->right = NULL;

            newNodeChL = new node();
            newNodeChR = new node();

            newNodeR->data = "^";
            newNodeR->left = newNodeChL;
            newNodeR->right = newNodeChR;

            newNodeChL->data = "cosecx";
            newNodeChL->left = NULL;
            newNodeChL->left = NULL;

            newNodeChR->data = "2";
            newNodeChR->left = NULL;
            newNodeChR->right = NULL;
        }
        return newNode;
    }
}

node *derivative(string eqn)
{
    node *Node = constructTree(eqn);
    node *deri;
    deri = findDerivative(Node);
    return deri;
}

/***********************************************************************************************/

/***********************************************************************************************/

void getDerivative()
{
    string in_string;
    vector<string> post;
    cout << "\nEnter the equation : ";
    fflush(stdin);
    getline(cin, in_string);

    // post = postfix(in_string);
    // cout << "\nPostfix equation : ";
    // for (int j = 0; j < post.size(); ++j)
    //     cout << post[j] << " ";
    // cout << "\n";

    // node *tree;
    // tree = constructTree(in_string);
    // cout << "\nInorder traversal : ";
    // inOrder(tree);
    // cout << "\n";

    node *deri = new node();
    deri = derivative(in_string);
    cout << "\nInorder traversal of the derivative : ";
    inOrder(deri);
}

/***********************************************************************************************/
