//Created by Stanislaw Morawski
//Set implemented on Splay Tree
//
//Apart from standard operations this implementation lets you perform operation shift(int x, int value)
//in amortized O(log n) time, where n is the size of the structure.
//Operation shift(x, value) works assuming that value >= 0. It adds value to all elements greater or equal than x.
//
//Other operations are:
//insert(x) - inserting x into structure if it wasn't there
//erase(x) - erasing x from the structure if it was there
//find(x) - returns true if x is in the structure, false otherwise
//sortedValues - returns vector<int> containing all values from the structure
//
//insert, erase and find are all working in amortized O(log n) time,
//while sortedValues works lineary


#include <cstdio>
#include <vector>
using namespace std;

namespace {

	struct node{
		int key;
		int left_shift_value, right_shift_value;	
		node * ls, * rs, * parent;
		node(int value) : key(value), left_shift_value(0), right_shift_value(0), ls(NULL), rs(NULL), parent(NULL){};
		};

	node * root = NULL;

	void pushDownShiftingValues(node * x){
		if (x == NULL or x -> parent == NULL) return;
		if (x == x -> parent -> ls){
			x -> key += x -> parent -> left_shift_value;
			if (x -> ls != NULL)
				x -> left_shift_value += x -> parent -> left_shift_value;
			if (x -> rs != NULL)
				x -> right_shift_value += x -> parent -> left_shift_value;
			x -> parent -> left_shift_value = 0;
			}
		else{
			x -> key += x -> parent -> right_shift_value;
			if (x -> ls != NULL)
				x -> left_shift_value += x -> parent -> right_shift_value;
			if (x -> rs != NULL)
				x -> right_shift_value += x -> parent -> right_shift_value;
			x -> parent -> right_shift_value = 0;
			}
		}

	void rotatePointersAndSetShiftingValues(node * x_parent, node * x){
		if (x_parent -> ls == x){
			//rotate pointers
			x_parent -> ls = x -> rs;
			if (x->rs != NULL)
				x -> rs -> parent = x_parent;
			x -> rs = x_parent;
			//set correct shifting_values on new edges
			x_parent -> left_shift_value = x -> right_shift_value;
			x -> right_shift_value = 0;
			}
		else{
			//rotate pointers
			x_parent -> rs = x -> ls;
			if (x -> ls != NULL)
				x -> ls -> parent = x_parent;
			x -> ls = x_parent;
			//set correct shifting_values on new edges
			x_parent -> right_shift_value = x -> left_shift_value;
			x -> left_shift_value = 0;
			}
		x_parent -> parent = x;
		}

	void rotate(node * x){
		if (x -> parent == NULL) return;
		node * x_parent = x -> parent;
		node * x_grand_parent = x_parent -> parent;
		x -> parent = x_grand_parent;
		if (x_grand_parent != NULL)
			x_parent==x_grand_parent -> ls ? x_grand_parent -> ls = x : x_grand_parent -> rs = x;	
		pushDownShiftingValues(x);
		rotatePointersAndSetShiftingValues(x_parent, x);
		}

	node * findClosest(int value){
		if (root == NULL) return root;
		node * result = root;
		while(result -> key != value){
			pushDownShiftingValues(result -> ls);
			pushDownShiftingValues(result -> rs);
			if (result -> key > value){
				if (result -> ls != NULL){
					//value -= result -> left_shift_value;
					result = result -> ls;
					}
				else break;
				}
			else {
				if (result -> rs != NULL){
					//value -= result -> right_shift_value;
					result = result -> rs;
					}
				else break;
				}
			}
		return result;
		}

	void splay(int value){
		node * x = findClosest(value);
		while(x -> parent != NULL and x -> parent -> parent != NULL)
			if ((x == x -> parent -> ls and x -> parent == x -> parent -> parent -> ls) or 
				(x == x -> parent -> rs and x -> parent == x -> parent -> parent -> rs)){
				rotate(x -> parent);
				rotate(x);
			}
			else{
				rotate(x);
				rotate(x);
			}
		if (x -> parent != NULL)
			rotate(x);
		root = x;
		}

	void dfs(node * x, int collective, vector<int>& V){
		if (x == NULL) return;
		dfs(x -> ls, collective + x -> left_shift_value, V);
		V.push_back(x -> key + collective);
		dfs(x -> rs, collective + x -> right_shift_value, V);
		return;
		}
	}

bool find(int value){
	if (root == NULL) return false;
	splay(value);
	return root -> key == value ? true:false;
	}

void insert(int value){
	if (root == NULL){
		root = new node(value);
		return;
		}
	splay(value);
	if (root -> key == value)
		return;
	node * new_node = new node(value);
	if (root -> key < value){
		new_node -> ls = root;
		new_node -> rs = root -> rs;
		root -> rs = NULL;
		if (new_node -> rs != NULL)
			new_node -> rs -> parent = new_node;
		new_node -> right_shift_value = root -> right_shift_value;
		root -> right_shift_value = 0;
		}
	else{
		new_node -> rs = root;
		new_node -> ls = root -> ls;
		root -> ls = NULL;
		if (new_node -> ls != NULL)
			new_node -> ls -> parent = new_node;
		new_node -> left_shift_value = root -> left_shift_value;
		root -> left_shift_value = 0;
		}
	root -> parent = new_node;
	root = new_node;
	}

void shift(int key, int value){
	if (root == NULL) return;
	splay(key);
	if (root -> key >= key)
		root -> key += value;
	if (root -> rs != NULL)
		root -> right_shift_value += value;
	}


vector<int> sortedValues(){
	vector<int> V;
	dfs(root, 0, V);
	return V;
	}

void erase(int value){
	if (root == NULL) return;
	splay(value);
	if (root -> key != value) return;
	node * left_tree = root -> ls, * right_tree = root -> rs;
	if (left_tree == NULL and right_tree == NULL)
		root = NULL;
	else if (right_tree != NULL and left_tree == NULL){
		pushDownShiftingValues(right_tree);
		root = right_tree;
		root -> parent = NULL;	
		}
	else{
		pushDownShiftingValues(left_tree);
		root = left_tree;
		root -> parent = NULL;
		if (right_tree != NULL){
			pushDownShiftingValues(right_tree);
			splay(right_tree -> key);
			root -> rs = right_tree;
			right_tree -> parent = root;
			}
		}
	}

int main(){
	int n;
	scanf(" %d", &n);
	for( int i=0; i<n; i++){
		int typ, x;
		scanf(" %d %d", &typ, &x);
		if (typ == 5){
			if (find(x))
				printf("JEST\n");
			else printf("NIEMA\n");
			}
		else if (typ>0)
			insert(x);
		else if (typ<0)
			erase(x);
		else{
			int val;
			scanf(" %d", &val);
			shift(x, val);
			}
		vector<int> VVV= sortedValues();
		for(int i=0; i< VVV.size(); i++)
			printf("%d ", VVV[i]);
		printf("\n");
		}
	return 0;
	}
