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


#include <cassert>
#include <cstdio>
#include <vector>
#include <type_traits>
#include <iostream>
#include <utility>
#include <memory>

namespace _type_check {

	template<typename O, typename T>
	class CheckForOperator{

		template<typename Op, typename TT>
		static auto test(int) -> decltype(std::declval<Op>()(std::declval<TT>(), std::declval<TT>()),
											std::true_type());
		
		template<typename, typename>
		static auto test(...) -> std::false_type;
		
	public:
		using type = decltype( test<O, T>(0));
	};

	template<bool A, typename ... Ts>
	class AndHelper{
		constexpr static bool value = false;
	};

	template<typename ... Ts>
	class AndHelper<false, Ts ...>{
		public:
			constexpr static bool value = false;
	};

	template<typename T, typename ... Ts>
	class AndHelper<true, T, Ts ...>{
		public:
			constexpr static bool value = AndHelper<T::value, Ts ...>::value;
	};

	template<>
	class AndHelper<true>{
		public:
			constexpr static bool value = true;
	};


	template<typename T, typename ... Ts>
	class AND{
		public:	constexpr static bool value = AndHelper<T::value, Ts ...>::value;
	};

	template<typename T>
	class is_a_good_type{
	public:
		constexpr static bool value = AND<
			typename std::is_assignable<T&, T>,
			typename CheckForOperator<std::equal_to<>, T>::type,
			typename CheckForOperator<std::plus<>, T>::type,
			typename CheckForOperator<std::minus<>, T>::type >::value;	
	};

}


template<typename T, typename std::enable_if<_type_check::is_a_good_type<T>::value, T>::type = 0>
class MySet{
private:
	static const int _ZERO = 0;
	
	struct node{
		using node_ptr = std::shared_ptr<node>;
		T key;
		T left_shift_value, right_shift_value;	
		node_ptr ls, rs, parent;
		node(T value) : key(value), ls(nullptr), rs(nullptr), parent(nullptr){
			left_shift_value = (T) _ZERO;
			right_shift_value = (T) _ZERO;
		};
	};

	using node_ptr = std::shared_ptr<node>;
	node_ptr _root = nullptr;

	void pushDownShiftingValues(node_ptr x){
		if (!x or !x -> parent) return;
		if (x == x -> parent -> ls){
			x -> key += x -> parent -> left_shift_value;
			if (x -> ls)
				x -> left_shift_value += x -> parent -> left_shift_value;
			if (x -> rs)
				x -> right_shift_value += x -> parent -> left_shift_value;
			x -> parent -> left_shift_value = (T) _ZERO;
			}
		else{
			x -> key += x -> parent -> right_shift_value;
			if (x -> ls)
				x -> left_shift_value += x -> parent -> right_shift_value;
			if (x -> rs)
				x -> right_shift_value += x -> parent -> right_shift_value;
			x -> parent -> right_shift_value = (T) _ZERO;
			}
		}

	void rotatePointersAndSetShiftingValues(node_ptr x_parent, node_ptr x){
		if (x_parent -> ls == x){
			//rotate pointers
			x_parent -> ls = x -> rs;
			if (x->rs)
				x -> rs -> parent = x_parent;
			x -> rs = x_parent;
			//set correct shifting_values on new edges
			x_parent -> left_shift_value = x -> right_shift_value;
			x -> right_shift_value = (T) _ZERO;
			}
		else{
			//rotate pointers
			x_parent -> rs = x -> ls;
			if (x -> ls)
				x -> ls -> parent = x_parent;
			x -> ls = x_parent;
			//set correct shifting_values on new edges
			x_parent -> right_shift_value = x -> left_shift_value;
			x -> left_shift_value = (T) _ZERO;
			}
		x_parent -> parent = x;
		}

	void rotate(node_ptr x){
		if (!x -> parent) return;
		node_ptr x_parent = x -> parent;
		node_ptr x_grand_parent = x_parent -> parent;
		x -> parent = x_grand_parent;
		if (x_grand_parent)
			x_parent==x_grand_parent -> ls ? x_grand_parent -> ls = x : x_grand_parent -> rs = x;	
		pushDownShiftingValues(x);
		rotatePointersAndSetShiftingValues(x_parent, x);
		}

	node_ptr findClosest(T value){
		if (!_root) return _root;
		node_ptr result = _root;
		while(result -> key != value){
			pushDownShiftingValues(result -> ls);
			pushDownShiftingValues(result -> rs);
			if (result -> key > value){
				if (result -> ls){
					result = result -> ls;
					}
				else break;
				}
			else {
				if (result -> rs){
					result = result -> rs;
					}
				else break;
				}
			}
		return result;
		}

	void splay(T value){
		node_ptr x = findClosest(value);
		while(x -> parent and x -> parent -> parent)
			if ((x == x -> parent -> ls and x -> parent == x -> parent -> parent -> ls) or 
				(x == x -> parent -> rs and x -> parent == x -> parent -> parent -> rs)){
				rotate(x -> parent);
				rotate(x);
			}
			else{
				rotate(x);
				rotate(x);
			}
		if (x -> parent)
			rotate(x);
		_root = x;
		}

	void dfs(node_ptr x, T collective, std::vector<T>& V){
		if (!x) return;
		dfs(x -> ls, collective + x -> left_shift_value, V);
		V.push_back(x -> key + collective);
		dfs(x -> rs, collective + x -> right_shift_value, V);
		return;
		}

public:

	MySet(){};

	bool find(T value){
		if (!_root) return false;
		splay(value);
		return _root -> key == value ? true:false;
		}

	void insert(T value){
		//_ZERO = value - value;
		if (!_root){
			_root = std::make_shared<node>(value);
			return;
			}
		splay(value);
		if (_root -> key == value)
			return;
		node_ptr new_node = std::make_shared<node>(value);
		if (_root -> key < value){
			new_node -> ls = _root;
			new_node -> rs = _root -> rs;
			_root -> rs = nullptr;
			if (new_node -> rs)
				new_node -> rs -> parent = new_node;
			new_node -> right_shift_value = _root -> right_shift_value;
			_root -> right_shift_value = (T) _ZERO;
			}
		else{
			new_node -> rs = _root;
			new_node -> ls = _root -> ls;
			_root -> ls = nullptr;
			if (new_node -> ls)
				new_node -> ls -> parent = new_node;
			new_node -> left_shift_value = _root -> left_shift_value;
			_root -> left_shift_value = (T) _ZERO;
			}
		_root -> parent = new_node;
		_root = new_node;
		}

	void shift(T key, T value){
		if (!_root) return;
		splay(key);
		if (_root -> key >= key)
			_root -> key += value;
		if (_root -> rs)
			_root -> right_shift_value += value;
		}


	std::vector<T> sortedValues(){
		std::vector<T> V;
		dfs(_root, (T) _ZERO, V);
		return V;
		}

	void erase(T value){
		if (!_root) return;
		splay(value);
		if (_root -> key != value) return;
		node_ptr left_tree = _root -> ls, right_tree = _root -> rs;
		if (!left_tree and !right_tree)
			_root = nullptr;
		else if (right_tree and !left_tree){
			pushDownShiftingValues(right_tree);
			_root = right_tree;
			_root -> parent = nullptr;	
			}
		else{
			pushDownShiftingValues(left_tree);
			_root = left_tree;
			_root -> parent = nullptr;
			if (right_tree){
				pushDownShiftingValues(right_tree);
				splay(right_tree -> key);
				_root -> rs = right_tree;
				right_tree -> parent = _root;
				}
			}
		}
	};

int main(){
	int n;
	scanf(" %d", &n);
	MySet<int> S;
	for( int i=0; i<n; i++){
		int typ, x;
		scanf(" %d %d", &typ, &x);
		if (typ == 5){
			if (S.find(x))
				printf("JEST\n");
			else printf("NIEMA\n");
			}
		else if (typ>0)
			S.insert(x);
		else if (typ<0)
			S.erase(x);
		else{
			int val;
			scanf(" %d", &val);
			S.shift(x, val);
			}
		std::vector<int> VVV = S.sortedValues();
		for(int i=0; i< VVV.size(); i++)
			printf("%d ", VVV[i]);
		printf("\n");
		}
	return 0;
	}
