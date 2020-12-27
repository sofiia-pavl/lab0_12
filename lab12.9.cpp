#include <iostream>
#include <fstream>
#include <string>
using namespace std;
typedef string Info;
struct Node // вузол дерева
{
	Node* left,
		* right;
	Info info;
};
struct Elem // елемент стеку,
{ // в якому зберігаються
	Elem* link; // батьківські вузли
	Node* info;
};
Node* CreateTree(int nodeCount);
void PrintTree(Node* root, int level);
void SaveToFile(fstream& f, Node* root);
void LoadFromFile(fstream& f, Node*& root);
void Push(Elem*& top, Node* node);
Node* Pop(Elem*& top);
Node* FindMax(Node* root);
int Count(Node* root);
void RightRotation(Node*& root);
void LeftRotation(Node*& root);
void BalanceCount(Node*& root);
Node* BinarySearchDelete(Node* root, Info value);
int main()
{
	//int N;
	//cout << "Enter nodes count: "; cin >> N;
	Node* root;
	Info value;
	//root = CreateTree( N );
	//PrintTree( root, 0 );
	fstream f;
	//f.open("tree.txt", ios::out);
	//SaveToFile(f, root);
	//f.close();
	f.open("tree.txt", ios::in);
	LoadFromFile(f, root);
	f.close();
	PrintTree(root, 0);
	cout << "max = " << FindMax(root)->info << endl;
	cout << endl;
	BalanceCount(root);
	cout << endl;
	PrintTree(root, 0);
	cout << "delete value: "; cin >> value;
	root = BinarySearchDelete(root, value);
	cout << endl;
	PrintTree(root, 0);
	cout << endl;
	cout << endl;
	return 0;
}
Node* CreateTree(int nodeCount)
{
	if (nodeCount == 0)
		return NULL;
	else
	{
		Node* newNode = new Node;

			cout << " Enter node value: ";
		cin >> newNode->info;

		int leftCount = nodeCount / 2;
		int rightCount = nodeCount - leftCount - 1;
		newNode->left = CreateTree(leftCount);
		newNode->right = CreateTree(rightCount);
		return newNode;
	}
}
void PrintTree(Node* root, int level)
{
	if (root != NULL)
	{
		PrintTree(root->right, level + 1);
		for (int i = 1; i <= level; i++)
			cout << " ";
		cout << root->info << endl;
		PrintTree(root->left, level + 1);
	}
}
void SaveToFile(fstream& f, Node* root)
{
	if (root != NULL)
	{
		f << root->info << endl;
		SaveToFile(f, root->left);
		SaveToFile(f, root->right);
	}
	else
		f << "<END>" << endl;
}
void LoadFromFile(fstream& f, Node*& root)
{
	Node* newNode, * current;
	Elem* top = NULL;
	Info c;
	if (!(f >> c)) // 1
	{
		root = NULL;
		return;
	}
	current = new Node; // 2
	current->info = c;
	current->left = NULL;
	current->right = NULL;
	root = current; // 2.1

	Push(top, current); // 2.2
	do
	{
			while (f >> c && c != "<END>") // 3
			{
				newNode = new Node;
				newNode->info = c;
				newNode->left = NULL;
				newNode->right = NULL;
				current->left = newNode; // 3.1
				current = current->left; // 3.2

				Push(top, current); // 3.3
			}

		while (f >> c)
			if (c == "<END>") // 4
			{
				if (top != NULL)
					current = Pop(top); // 4.1
			}
			else
				break;
		if (f.eof()) break;
		if (top != NULL) // 5
		{
			current = Pop(top); // 5.1
			newNode = new Node;
			newNode->info = c;
			newNode->left = NULL;
			newNode->right = NULL;
			current->right = newNode; // 5.2
			current = current->right; // 5.3
			Push(top, current); // 5.4
		}
	} while (!f.eof()); // 6
}
void Push(Elem*& top, Node* node)
{
	Elem* tmp = new Elem;
	tmp->info = node;
	tmp->link = top;
	top = tmp;
}
Node* Pop(Elem*& top)
{
	Elem* tmp = top->link;
	Node* node = top->info;
	delete top;
	top = tmp;
	return node;
}
Node* FindMax(Node* root)
{
	while (root->right != NULL)
		root = root->right;
	return root;
}
int Count(Node* root)
{
	if (root == NULL)
		return 0;
	else
		return 1 + Count(root->left) + Count(root->right);
}
void RightRotation(Node*& root)
{
	Node* tmp1 = root->left;
	Node* tmp2 = tmp1->right;
	tmp1->right = root; //1
	root->left = tmp2; //2
	root = tmp1; //3
}
void LeftRotation(Node*& root)
{
	Node* tmp1 = root->right;
	Node* tmp2 = tmp1->left;
	tmp1->left = root; //1
	root->right = tmp2; //2
	root = tmp1; //3
}
void BalanceCount(Node*& root)
{
	if (root != NULL)
	{
		while (Count(root->left) > Count(root->right) + 1)
			RightRotation(root);
		while (Count(root->left) + 1 < Count(root->right))
			LeftRotation(root);
		BalanceCount(root->left);
		BalanceCount(root->right);
	}
}
Node* BinarySearchDelete(Node* root, Info value)
{
	if (NULL == root) return NULL; // не знайшли вузол
	if (root->info == value) // знайшли вузол
	{
		if (NULL == root->left && NULL == root->right) // листовий вузол
		{
			delete root;
			return NULL;
		}
		if (NULL == root->right && root->left != NULL) // вузол з одним
		{ // (лівим) нащадком
			Node* temp = root->left;
			delete root;
			return temp;
		}
		if (NULL == root->left && root->right != NULL) // вузол з одним
		{ // (правим) нащадком
			Node* temp = root->right;
			delete root;
			return temp;
		}
		// вузол з двома
		root->info = FindMax(root->left)->info; // нащадками
		root->left = BinarySearchDelete(root->left, root->info);
		return root;
	}
	// не знайшли вузол -
	if (value < root->info) // бінарний пошук в
	{ // лівому під-дереві
		root->left = BinarySearchDelete(root->left, value);
		return root;
	}
	if (value > root->info) // бінарний пошук в
	{ // правому під-дереві
		root->right = BinarySearchDelete(root->right, value);
		return root;
	}
	return root;
}