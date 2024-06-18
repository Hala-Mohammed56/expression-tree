#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 150

// Linked list structure to store equations
typedef struct linkedNode
{
    char Data[MAX];
    struct linkedNode* Next;
} LinkedList;

// Tree node structure for expression tree
typedef struct TreeNode
{
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} Tree;

// Global stacks for expression tree construction
Tree* stack1[MAX];
int top1 = -1;

Tree* stack2[MAX];
int top2 = -1;

// Function to push a node onto stack1
void push1(Tree* data)
{
    top1++;
    stack1[top1] = data;
}

// Function to pop a node from stack1
Tree* pop1()
{
    Tree* value = stack1[top1];
    top1--;
    return value;
}

// Function to push a node onto stack2
void push2(Tree* data)
{
    top2++;
    stack2[top2] = data;
}

// Function to pop a node from stack2
Tree* pop2()
{
    Tree* value = stack2[top2];
    top2--;
    return value;
}

// Function to make stack1 empty
void makeStackEmpty()
{
    top1 = -1;
}

// Function to check if stack1 is empty
int isEmptyLetter()
{
    return top1 == -1;
}

// Function to create an empty linked list
LinkedList* MakeEmptyLinkedList(LinkedList* L);

// Function to insert a new equation into the linked list
void Insert(LinkedList* L, char data[MAX]);

// Function to delete the entire linked list
void DeleteList(LinkedList* L);

// Function to print the contents of the linked list
void PrintList(LinkedList* L);

// Function to read equations from a file and insert into the linked list
void Read_equations(LinkedList* L);

// Function to create a new tree node with given data
Tree* CreateTreeNode(char data);

// Function to clean up an equation by removing unnecessary characters
void Clean_The_equation(char* equation);

// Function to determine the priority of an operator
int priority(char operation);

// Function to check if a character is an operator
int isOperator(char c);

// Function to convert an infix expression to postfix
void Infix_To_Postfix(char* infix, char* postfix);

// Function to construct an expression tree from a postfix expression
void stcak_ET(char* expr);

// Function to evaluate an expression tree
int EvaluateExpressionTree(Tree* root);

// Function to save postfix expression and result to an output file
void SaveToFile(char* postfix, int result);

// Function to display the menu options
void View_Menu();

// Function to get the user's choice from the menu
int The_Choice();

// Function to perform the chosen menu action
void Menu_Choice(int choice, LinkedList* equationList);

// Function to handle reading equations from a file
void Menu_ReadEquations(LinkedList* equationList);

// Function to handle printing equations
void Menu_PrintEquations(LinkedList* equationList);

// Function to handle evaluating expressions using expression trees
void Menu_EvaluateExpressionTree(LinkedList* equationList);

// Function to handle printing postfix expressions
void Menu_PrintPostfixExpressions(LinkedList* equationList);

// Function to handle saving results to an output file
void Menu_SaveToFile(LinkedList* equationList);

// Main function
int main()
{
    LinkedList* equationList = MakeEmptyLinkedList(NULL);
    int choice = 0;

    do
    {
        View_Menu();
        choice = The_Choice();
        Menu_Choice(choice, equationList);
        printf("_________________________\n\n");
    } while (choice != 6);

    DeleteList(equationList);

    return 0;
}

LinkedList* MakeEmptyLinkedList(LinkedList* L)
{
    if (L != NULL)
        DeleteList(L);

    L = (LinkedList*)malloc(sizeof(LinkedList));

    if (L == NULL)
        printf("Out of memory!\n");

    L->Next = NULL;
    return L;
}

void Insert(LinkedList* L, char data[MAX])
{
    LinkedList* new_node = (LinkedList*)malloc(sizeof(LinkedList));

    if (new_node == NULL)
    {
        printf("Out of memory!\n");
        return;
    }

    strcpy(new_node->Data, data);
    new_node->Next = NULL;

    if (L->Next == NULL)
    {
        L->Next = new_node;
    }
    else
    {
        LinkedList* current_node = L;
        while (current_node->Next != NULL)
        {
            current_node = current_node->Next;
        }

        current_node->Next = new_node;
    }
}

void DeleteList(LinkedList* L)
{
    LinkedList* P, * temp;
    P = L->Next;
    L->Next = NULL;

    while (P != NULL)
    {
        temp = P->Next;
        free(P);
        P = temp;
    }
}

void PrintList(LinkedList* L)
{
    LinkedList* P = L->Next;

    if (P == NULL)
    {
        printf("Empty list\n");
    }
    else
    {
        while (P != NULL)
        {
            printf("%s\n", P->Data);
            P = P->Next;
        }
        printf("\n");
    }
}

void Read_equations(LinkedList* L)
{
    FILE* Infile = fopen("input.txt", "r");

    if (Infile == NULL)
    {
        perror("Error ... !");
        return;
    }
    else
    {
        printf("File opened successfully!\n");
    }

    char The_equation[MAX];

    while (fscanf(Infile, "%150s", The_equation) == 1)
    {
        Insert(L, The_equation);
    }

    fclose(Infile);
    printf("The File closed successfully!\n");
    printf("Done!\n");
}

Tree* CreateTreeNode(char data)
{
    Tree* new_node = (Tree*)malloc(sizeof(Tree));

    if (new_node == NULL)
    {
        printf("Out of memory!\n");
        return NULL;
    }

    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void Clean_The_equation(char* equation)
{
    int i, j;

    for (i = 0, j = 0; equation[i] != '\0'; i++)
    {
        if (!isspace(equation[i]) && (isalnum(equation[i]) || equation[i] == '+' || equation[i] == '-' ||
                                       equation[i] == '%' || equation[i] == '*' || equation[i] == '/' || equation[i] == '(' || equation[i] == ')'))
        {
            if (equation[i] == '-')
            {
                if (i == 0 || (i > 0 && !isdigit(equation[i - 1]) && equation[i - 1] != ')'))
                {
                    equation[j++] = equation[i];
                }
                else
                {
                    equation[j++] = ' ';
                }
            }
            else
            {
                equation[j++] = equation[i];
            }
        }
    }

    equation[j] = '\0';
}

int priority(char operation)
{
    if (operation == '+' || operation == '-')
        return 1;
    else if (operation == '*' || operation == '/' || operation == '%')
        return 2;
    return 0;
}

int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

void Infix_To_Postfix(char* infix, char* postfix)
{
    int i, j = 0;
    char temp;
    Clean_The_equation(infix);
    makeStackEmpty();

    for (i = 0; infix[i] != '\0'; i++)
    {
        temp = infix[i];

        if (isdigit(temp))
        {
            postfix[j++] = temp;
            while (isdigit(infix[i + 1]))
            {
                postfix[j++] = infix[++i];
            }
            postfix[j++] = ' ';
        }
        else if (temp == '(')
        {
            push1(CreateTreeNode(temp));
        }
        else if (temp == ')')
        {
            while (!isEmptyLetter() && stack1[top1]->data != '(')
            {
                postfix[j++] = pop1()->data;
                postfix[j++] = ' ';
            }
            if (!isEmptyLetter() && stack1[top1]->data == '(')
            {
                pop1(); // Discard '(' from the stack
            }
        }
        else if (isOperator(temp))
        {
            while (!isEmptyLetter() && priority(stack1[top1]->data) >= priority(temp))
            {
                postfix[j++] = pop1()->data;
                postfix[j++] = ' ';
            }
            push1(CreateTreeNode(temp));
        }
    }

    while (!isEmptyLetter())
    {
        postfix[j++] = pop1()->data;
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
}


void stcak_ET(char* expr)
{
    Tree* new_node, * op1, * op2;
    char temp[MAX];
    int i;

    for (i = 0; expr[i] != '\0'; i++)
    {
        if (isdigit(expr[i]))
        {
            new_node = CreateTreeNode(expr[i]);
            push2(new_node);
        }
        else if (isOperator(expr[i]))
        {
            new_node = CreateTreeNode(expr[i]);
            op2 = pop2();
            op1 = pop2();
            new_node->right = op2;
            new_node->left = op1;
            push2(new_node);
        }
        else if (expr[i] == ' ')
        {
            continue;
        }
    }
}

int EvaluateExpressionTree(Tree* root)
{
    if (root == NULL)
        return 0;

    if (root->left == NULL && root->right == NULL)
        return root->data - '0';

    int left_result = EvaluateExpressionTree(root->left);
    int right_result = EvaluateExpressionTree(root->right);

    switch (root->data)
    {
    case '+':
        return left_result + right_result;
    case '-':
        return left_result - right_result;
    case '*':
        return left_result * right_result;
    case '/':
        if (right_result == 0) {
            printf("Error: Division by zero!\n");
            exit(EXIT_FAILURE);
        }
        return left_result / right_result;
    case '%':
        if (right_result == 0) {
            printf("Error: Modulo by zero!\n");
            exit(EXIT_FAILURE);
        }
        return left_result % right_result;
    default:
        printf("Error: Unknown operator '%c'\n", root->data);
        exit(EXIT_FAILURE);
    }
}


void SaveToFile(char* postfix, int result)
{
    FILE* Outfile = fopen("output.txt", "a");

    if (Outfile == NULL)
    {
        perror("Error ... !");
        return;
    }
    else
    {
        printf("File opened successfully!\n");
    }

    fprintf(Outfile, "Postfix Expression: %s\n", postfix);
    fprintf(Outfile, "Result: %d\n", result);
    fprintf(Outfile, "--------------------------------------\n");

    fclose(Outfile);
    printf("The File closed successfully!\n");
    printf("Done!\n");
}

void View_Menu()
{
    printf(" 1- Read equations\n");
    printf(" 2- Print equations\n");
    printf(" 3- Evaluate using Expression tree\n");
    printf(" 4- Print postfix expressions\n");
    printf(" 5- Save to output file (postfix and results)\n");
    printf(" 6- Exit\n");
}

int The_Choice()
{
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume the newline character
    return choice;
}

void Menu_Choice(int choice, LinkedList* equationList) {
    switch (choice) {
    case 1:
        Menu_ReadEquations(equationList);
        break;
    case 2:
        Menu_PrintEquations(equationList);
        break;
    case 3:
        Menu_EvaluateExpressionTree(equationList);
        break;
    case 4:
        Menu_PrintPostfixExpressions(equationList);
        break;
    case 5:
        Menu_SaveToFile(equationList);
        break;
    case 6:
        printf("Exiting...\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
    }
}

void Menu_ReadEquations(LinkedList* equationList) {
    printf("Reading equations from the file...\n");
    Read_equations(equationList);
    printf("Equations read successfully!\n");
}

void Menu_PrintEquations(LinkedList* equationList) {
    printf("Printing equations...\n");
    PrintList(equationList);
}

void Menu_EvaluateExpressionTree(LinkedList* equationList) {
    LinkedList* current_node = equationList->Next;
    while (current_node != NULL) {
        char postfixExpression[MAX];
        strcpy(postfixExpression, current_node->Data);
        stcak_ET(postfixExpression);
        Tree* root = pop2();
        int result = EvaluateExpressionTree(root);
        printf("Result for equation \"%s\": %d\n", postfixExpression, result);
        current_node = current_node->Next;
    }
}

void Menu_PrintPostfixExpressions(LinkedList* equationList) {
    LinkedList* current_node = equationList->Next;
    while (current_node != NULL) {
        char infixExpression
[MAX];
        char postfixExpression[MAX];
        strcpy(infixExpression, current_node->Data);
        Infix_To_Postfix(infixExpression, postfixExpression);
        printf("Infix expression: %s\n", infixExpression);
        printf("Postfix expression: %s\n", postfixExpression);
        current_node = current_node->Next;
    }
}

void Menu_SaveToFile(LinkedList* equationList) {
    FILE* Outfile = fopen("output.txt", "w");
    fclose(Outfile);

    LinkedList* current_node = equationList->Next;
    while (current_node != NULL) {
        char infixExpression[MAX];
        char postfixExpression[MAX];
        strcpy(infixExpression, current_node->Data);
        Infix_To_Postfix(infixExpression, postfixExpression);
        stcak_ET(postfixExpression);
        Tree* root = pop2();
        int result = EvaluateExpressionTree(root);
        SaveToFile(postfixExpression, result);
        current_node = current_node->Next;
    }
}
