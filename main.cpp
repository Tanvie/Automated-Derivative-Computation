#include <bits/stdc++.h>
using namespace std;

// Structure for node of a tree.
typedef struct node
{
    string data; // Variable for storing operator or operand.
    struct node *left;
    struct node *right;
} node;

// Utility functions.
bool isNumber(string s);
int findType(string ch);
bool isOperator(string str);
int precedance(string a);

// Functions for printing data in the tree.
void inOrder(node *tree);
void preOrder(node *node);
string traversal(node *root);

// Functions for parsing the input string and converting the infix equation into postfix equation.
string removeSpaces(string str);
vector<string> getOperands(string ip_string);
vector<string> infixToPost(vector<string> infix);
vector<string> postfix(string in_string);

// Functions for converting postfix equation into equation tree.
node *operandNode(string str);
node *constructTree(string eqn);

// Functions for finding the derivative tree from given equation tree.
node *copyNode(node *Node);
node *exponentDerivative(node *Node);
node *findDerivative(node *Node);
node *derivative(string eqn);

string getDerivative(string in_string);

// Driver Code
int main()
{
    string in_string;
    cout << "\nEnter the equation : "; // Taking the input.
    fflush(stdin);
    getline(cin, in_string);

    string derivative;
    derivative = getDerivative(in_string); // getDerivative returns the derivative string.
    cout << "\nDerivative of given equation : ";
    cout << derivative;

    return 0;
}

/***********************************************************************************************/

// Returns true if the string passed is a NUMBER.
bool isNumber(string s)
{
    auto it = s.begin();
    while (it != s.end() && isdigit(*it)) // Iterating through the string and checking each of the character.
        ++it;
    return !s.empty() && it == s.end();
}

// Returns 1 if the given string is operand or returns -1 if string is arithmetic operator.
int findType(string ch)
{
    if (ch == "+" || ch == "-" || ch == "*" || ch == "/" || ch == "^")
        return -1; //returns -1 if string is operator.
    return 1;
}

// Determines if the given string is a operator or not.
// Returns 1 if string is operator or returns 0 if string is operand.
bool isOperator(string str)
{
    return (str == "+" || str == "-" || str == "/" || str == "*" || str == "^");
}

// Returns the precedance of the operator.
// 3-> ^
// 2-> * or /
// 1-> + or -
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

// Prints the inorder traversal of the tree.
// Left-> Root-> Right.
void inOrder(node *tree)
{
    if (tree != NULL)
    {
        inOrder(tree->left);
        cout << tree->data << " ";
        inOrder(tree->right);
    }
    else
        return;
}

// Prints the preorder traversal of the tree.
// Root-> Left-> Right.
void preOrder(node *node)
{
    if (node == NULL)
        return;

    printf("%s ", node->data);
    preOrder(node->left);
    preOrder(node->right);
}

// Function for printing the derivative equation as fully parenthesied string.
string traversal(node *root)
{
    if (root->data == "+" || root->data == "-" || root->data == "*" || root->data == "^")
    {
        // Binary operators.
        // First operand - left child of node.
        // Second operand - right child of node.
        return "[" + traversal(root->left) + " " + root->data + " " + traversal(root->right) + "]";
    }
    else
    {
        // Unary operators.
        // Operand - right child of node.
        if (root->data == "cos")
            return "cos(" + traversal(root->right) + ")";

        else if (root->data == "sin")
            return "sin(" + traversal(root->right) + ")";

        else if (root->data == "tan")
            return "tan(" + traversal(root->right) + ")";

        else if (root->data == "log")
            return "log(" + traversal(root->right) + ")";

        else if (root->data == "cosec")
            return "cosec(" + traversal(root->right) + ")";

        else if (root->data == "sec")
            return "sec(" + traversal(root->right) + ")";

        else if (root->data == "cot")
            return "cot(" + traversal(root->right) + ")";

        else
            // If the node data is number or x then return it as it is.
            // The leaf nodes of the derivative tree are always either number or x.
            return root->data;
    }
}

/***********************************************************************************************/

// Removes all the whitespace characters(" ","\n","\t") from the string.
string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

// The function getOperands takes the input string
// and returns a vector of strings which contains all the operands and operators in infix order.
vector<string> getOperands(string ip_string)
{
    vector<string> operands; //defining a vector to contain all the operand and operators
    int i = 0, start = 0, end = 0;
    string s1;
    stack<string> st;
    for (i = 0; i < ip_string.length(); i++) // Iterating through the input string.
    {
        if (ip_string[i] == '[')
        {
            // Operands are enclosed into [] hence all the characters between [] are treated as a separate string.
            st.push("[");
            while (!st.empty())
            {
                i++;
                if (ip_string[i] == '[')
                    st.push("[");
                if (ip_string[i] == ']')
                    st.pop();
            }
            i++;
        }
        if (ip_string[i] == ' ') // Parsing the string whenever a space is encountered.
        {
            end = i - start;                   // Length of the string (operator or operand).
            s1 = ip_string.substr(start, end); // Creating a substring (operator or operand) from last encountered space+1 to this space index
            if (s1.length() != 0)
                operands.push_back(s1); // If the string is non-empty then append it into the vector

            start = i + 1;
        }
    }
    s1 = ip_string.substr(start, ip_string.length() - start);
    operands.push_back(s1);
    return operands; // operands -> Vector containing the operators and operands in infix order(parsed input string).
}

// Converts the given infix equation into a postfix equation using stack.
vector<string> infixToPost(vector<string> infix)
{
    int i;
    string temp;
    stack<string> stk;
    vector<string> post;
    for (i = 0; i < infix.size(); i++)
    {
        if (isOperator(infix[i])) // Operator encountered.
        {
            while (!stk.empty() && precedance(infix[i]) <= precedance(stk.top()))
            {
                temp = stk.top();
                stk.pop();
                post.push_back(temp);
            }
            stk.push(infix[i]);
        }
        else if (infix[i] == "(") // '(' bracket
        {
            stk.push(infix[i]);
        }
        else if (infix[i] == ")") // ')' bracket
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
        else // Operand encountered.
            post.push_back(infix[i]);
    }

    while (!stk.empty())
    {
        temp = stk.top();
        stk.pop();
        post.push_back(temp);
    }

    return post; // post -> Vector containing the operators and operands in postfix order.
}

// Driver code which takes input as a string and returns a vector of type string of postfix expression.
vector<string> postfix(string in_string)
{
    vector<string> operands;
    vector<string> post;
    operands = getOperands(in_string);
    post = infixToPost(operands);
    return post; // post -> Vector containing the operators and operands in postfix order.
}

/***********************************************************************************************/

/***********************************************************************************************/

// Function takes a unary operator and it's operands and returns root node of the expression tree.
node *operandNode(string str)
{
    string br1 = "[", br2 = "]";
    node *new1;
    int occ1 = str.find(br1);
    int occ2;
    if (occ1 == string::npos)
    {
        new1 = new node();
        new1->data = str;
        new1->left = NULL;
        new1->right = NULL;
    }
    else
    {
        new1 = new node();
        string operator1 = str.substr(0, occ1);
        new1->data = operator1;

        occ2 = str.rfind(br2);
        string operand = str.substr(occ1 + 1, occ2 - occ1 - 1); // Getting the operand string.
        new1->right = constructTree(operand);                   // Constructing tree for operand. (Operand are stored as right child of the unary operators)
        new1->left = NULL;
    }

    return new1; // new1 -> Root node containing unary operator as data.
}

// Takes infix equation as input and returns root node of the expression tree.
node *constructTree(string eqn)
{
    vector<string> post = postfix(eqn); // Calling postfix for conversion from infix to postfix.
    if (post[0] == "x")                 // If data is 'x' then return a single node containing 'x' as data.
    {
        node *new1;
        new1 = new node();
        new1->data = "x";
        new1->left = NULL;
        new1->right = NULL;
        return new1;
    }
    stack<node *> stk;
    node *new1, *p1, *p2;
    auto i = post.begin();
    string s = *i;
    int flag;

    // Iterating through the postfix equation for constructing tree using stack.
    for (i++; i != post.end(); i++)
    {
        flag = findType(s);
        if (flag == 1)
        {
            // If the data is opeand then construct a node of tree and push the node on the stack.
            new1 = new node();
            new1 = operandNode(s);
            stk.push(new1);
        }
        else
        {
            // If the data is operator then pop the stack two times and link the operator with operands
            // and again push the node on the stack.
            p1 = stk.top();
            stk.pop();
            p2 = stk.top();
            stk.pop();
            new1 = new node();
            new1->data = s;
            new1->left = p2;
            new1->right = p1;
            stk.push(new1);
        }
        s = *i;
    }

    p1 = stk.top();
    stk.pop();
    p2 = stk.top();
    stk.pop();
    new1 = new node();
    new1->data = s;
    new1->left = p2;
    new1->right = p1;
    stk.push(new1);

    return stk.top(); // stk.top() -> Root node of the expression tree.
}

/***********************************************************************************************/

/***********************************************************************************************/

// Function to copy the tree.
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
        return temp; // temp -> Root node of the copied tree.
    }
}

// Function for calculating the derivatives of exponential functions.
// Case 1 : e ^ (f(x))
// Case 2 : a ^ (f(x))
// Case 3 : (f(x)) ^ a
node *exponentDerivative(node *Node)
{
    node *newNode, *newNodeL, *newNodeR;
    string opr1 = Node->left->data, opr2 = Node->right->data;
    if (opr1 == "e") // e ^ (f(x)) -> (e ^ f(x)) * f'(x)
    {
        newNode = new node();
        newNodeL = new node();

        newNode->data = "*";
        newNode->left = newNodeL;
        newNode->right = findDerivative(Node->right); // f'(x)

        newNodeL->data = "^"; //  (e ^ f(x))

        newNodeL->left = new node();
        newNodeL->left->data = "e";
        newNodeL->left->left = NULL;
        newNodeL->left->right = NULL;

        newNodeL->right = copyNode(Node->right);
    }
    else if (isNumber(opr1)) // a ^ (f(x)) -> (a ^ (f(x))) * log(a) * f'(x)
    {
        node *newNodeLL, *newNodeLR;
        newNode = new node();
        newNodeL = new node();
        newNodeLL = new node();
        newNodeLR = new node();

        newNode->data = "*";
        newNode->left = newNodeL;
        newNode->right = findDerivative(Node->right); // f'(x)

        newNodeL->data = "*";
        newNodeL->left = newNodeLL;
        newNodeL->right = newNodeLR;

        newNodeLL->data = "^"; //  (a ^ (f(x)))
        newNodeLL->left = new node();
        newNodeLL->left->data = opr1;
        newNodeLL->left->left = NULL;
        newNodeLL->left->right = NULL;

        newNodeLL->right = new node();
        newNodeLL->right = copyNode(Node->right);

        newNodeLR->data = "log"; //  log(a)
        newNodeLR->left = NULL;
        newNodeLR->right = new node();
        newNodeLR->right->data = opr1;
        newNodeLR->right->left = NULL;
        newNodeLR->right->right = NULL;
    }
    else // (f(x)) ^ a -> a * (f(x) ^ (a-1)) * f'(x)
    {
        node *newNodeLL, *newNodeLR;
        stringstream ss;
        int num1;
        newNode = new node();
        newNodeL = new node();
        newNodeLL = new node();
        newNodeLR = new node();

        newNode->data = "*";
        newNode->left = newNodeL;
        newNode->right = findDerivative(Node->left); // f'(x)

        newNodeL->data = "*";
        newNodeL->left = newNodeLL;
        newNodeL->right = newNodeLR;

        newNodeLL->data = opr2;
        newNodeLL->left = NULL;
        newNodeLL->right = NULL;

        newNodeLR->data = "^"; // (f(x) ^ (a-1))
        newNodeLR->left = copyNode(Node->left);
        newNodeLR->right = new node();
        num1 = stoi(Node->right->data);
        num1 -= 1;
        ss << num1;
        string str = ss.str();

        newNodeLR->right->data = str; // a
        newNodeLR->right->left = NULL;
        newNodeLR->right->right = NULL;
    }
    return newNode;
}

// Function which takes root of expression tree and recursively finds the derivative of given
// expression tree.
node *findDerivative(node *Node)
{
    node *newNode, *newNodeL, *newNodeR;

    if (isOperator(Node->data))
    {
        // The node is operator node and recursively find left and right child's derivative
        if (Node->data == "+" || Node->data == "-") // f(x) + g(x) -> f'(x) + g'(x)
        {
            newNode = new node();
            newNode->data = Node->data;
            newNode->left = findDerivative(Node->left);
            newNode->right = findDerivative(Node->right);
        }
        else if (Node->data == "*") // f(x) * g(x) -> (f(x) * g'(x)) + (f'(x) * g(x))
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "+";
            newNode->left = newNodeL;
            newNode->right = newNodeR;

            newNodeL->data = "*"; //  (f(x) * g'(x))
            newNodeL->left = copyNode(Node->left);
            newNodeL->right = findDerivative(Node->right);

            newNodeR->data = "*"; //  (f'(x) * g(x))
            newNodeR->left = findDerivative(Node->left);
            newNodeR->right = copyNode(Node->right);
        }

        // ExponentDerivative function called for finding the derivative of functions which include
        // all exponential functions.
        else if (Node->data == "^")
        {
            newNode = exponentDerivative(Node);
        }
    }
    else
    {
        string operand;
        operand = Node->data;
        node *newNodeL, *newNodeR;
        node *newNodeLL, *newNodeLR;
        if (isNumber(operand)) // a -> 0
        {
            newNode = new node();
            newNode->data = "0";
            newNode->left = NULL;
            newNode->right = NULL;
        }
        else if (operand == "x") // x -> 1
        {
            newNode = new node();
            newNode->data = "1";
            newNode->left = NULL;
            newNode->right = NULL;
        }
        else if (operand == "sin") // sin(f(x)) -> cos(f(x)) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); //  f'(x)

            newNodeL->data = "cos"; // cos(f(x))
            newNodeL->left = NULL;
            newNodeL->right = copyNode(Node->right);
        }
        else if (operand == "cos") // cos(f(x)) -> -1 * sin(f(x)) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "*";

            newNodeLL = new node(); // -1
            newNodeLL->data = "-1";
            newNodeLL->left = NULL;
            newNodeLL->right = NULL;

            newNodeLR = new node();
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            newNodeLR->data = "sin"; // sin(f(x))
            newNodeLR->left = NULL;
            newNodeLR->right = copyNode(Node->right);
        }
        else if (operand == "tan") // tan(f(x)) -> (sec(f(x)) ^ 2) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "^"; // (sec(f(x)) ^ 2)
            newNodeLL = new node();
            newNodeLL->data = "sec";
            newNodeLL->right = copyNode(Node->right);
            newNodeLL->left = NULL;

            newNodeLR = new node();
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            newNodeLR->data = "2";
            newNodeLR->left = NULL;
            newNodeLR->right = NULL;
        }
        else if (operand == "sec") // sec(f(x)) -> sec(f(x)) * tan(f(x)) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();
            newNodeLL = new node();
            newNodeLR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "*";
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            newNodeLL->data = "sec"; // sec(f(x))
            newNodeLL->left = NULL;
            newNodeLL->right = copyNode(Node->right);

            newNodeLR->data = "tan"; // tan(f(x))
            newNodeLR->left = NULL;
            newNodeLR->right = copyNode(Node->right);
        }
        else if (operand == "cosec") // cosec(f(x)) -> -1 * cosec(f(x)) * cot(f(x)) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();
            newNodeLL = new node();
            newNodeLR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "*";
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            newNodeLR->data = "cot"; // cot(f(x))
            newNodeLR->left = NULL;
            newNodeLR->right = copyNode(Node->right);

            node *newNodeLLL, *newNodeLLR;
            newNodeLLL = new node();
            newNodeLLR = new node();

            newNodeLL->data = "*";
            newNodeLL->left = newNodeLLL;
            newNodeLL->right = newNodeLLR;

            newNodeLLL->data = "-1"; // -1
            newNodeLLL->left = NULL;
            newNodeLLL->right = NULL;

            newNodeLLR->data = "cosec"; // cosec(f(x))
            newNodeLLR->left = NULL;
            newNodeLLR->right = copyNode(Node->right);
        }
        else if (operand == "cot") // cot(f(x)) -> -1 * (cosec(f(x)) ^ 2) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();
            newNodeLL = new node();
            newNodeLR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "*";
            newNodeL->left = newNodeLL;
            newNodeL->right = newNodeLR;

            newNodeLL->data = "-1"; // -1
            newNodeLL->left = NULL;
            newNodeLL->right = NULL;

            node *newNodeLRL, *newNodeLRR;
            newNodeLRL = new node();
            newNodeLRR = new node();
            newNodeLR->data = "^"; // (cosec(f(x)) ^ 2)
            newNodeLR->left = newNodeLRL;
            newNodeLR->right = newNodeLRR;

            newNodeLRL->data = "cosec";
            newNodeLRL->left = NULL;
            newNodeLRL->right = copyNode(Node->right);

            newNodeLRR->data = "2";
            newNodeLRR->left = NULL;
            newNodeLRR->right = NULL;
        }
        else if (operand == "log") // log(f(x)) -> (f(x) ^ -1) * f'(x)
        {
            newNode = new node();
            newNodeL = new node();
            newNodeR = new node();

            newNode->data = "*";
            newNode->left = newNodeL;
            newNode->right = findDerivative(Node->right); // f'(x)

            newNodeL->data = "^"; // (f(x) ^ -1)
            newNodeL->left = copyNode(Node->right);

            newNodeL->right = new node();
            newNodeL->right->data = "-1";
            newNodeL->right->left = NULL;
            newNodeL->right->right = NULL;
        }
    }

    return newNode; // newNode -> Root node of the derivative tree.
}

// Function taking infix equation and returning root node of the derivative tree.
node *derivative(string eqn)
{
    node *Node = constructTree(eqn);
    node *deri;
    deri = findDerivative(Node);
    return deri;
}
/***********************************************************************************************/

/***********************************************************************************************/

// Function taking infix equation and returning infix equation of the derivative.
string getDerivative(string in_string)
{
    vector<string> post;

    node *deri = new node();
    deri = derivative(in_string);
    string derivative = traversal(deri);

    return derivative;
}

/***********************************************************************************************/


// Example Inputs:
// log[34 * sin[x]] + tan[sin[x] - 67] + 12
// sin[cos[2 * x] + 12] + 123 * x
// sin[x] + cos[x] + 123 * x
// sec[4 * x] + tan[log[x] + 12] + 12

