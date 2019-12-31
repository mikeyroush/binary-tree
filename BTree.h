#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

enum color {red, black};

template <typename T>
struct Node
{
	T value;
	Node<T>* left;
	Node<T>* right;
    Node<T>* parent;

    int search_time;
    color node_color; //don't need to worry about this for the regular BTree

    Node(T val = T(), Node<T>* l = NULL, Node<T>* r = NULL, Node<T>* p = NULL, color col = color::red)
		: value(val), left(l), right(r), parent(p), search_time(0), node_color(col) {}

	Node(const Node<T>* other)
	{
		value = other->value;
		node_color = other->node_color;
		search_time = other->search_time;
	}

	bool is_leaf(){return (left == 0 && right == 0);}
};

template <typename T>
struct BTree
{
protected:
	int size;
    int levels;
	
public:
	Node<T>* root;

    BTree() : root(NULL),size(0){}
	BTree(const BTree<T>& other);
	BTree<T>& operator=(const BTree<T>& other);
	//BTree<T>& operator=(const BTree<T> other);

	Node<T>* get_root(){return root;}
	const Node<T>* get_root() const {return root;} 
	virtual Node<T>* insert(T obj);
	Node<T>* search(T obj);
	void update_search_times();
	float get_average_search_time();
    ostream& inorder(ostream& out);
    virtual ostream& print_level_by_level(ostream& out);
    int get_size() const {return size;}

private:
	void copy_helper(Node<T>* copy_to, const Node<T>* copy_from) const;
	virtual Node<T>* insert_helper(T obj, Node<T>* node);
	Node<T>* search_helper(T obj, Node<T>* node);
	ostream& inorder_helper(ostream& out, Node<T>* node);
	void update_search_times_helper(Node<T>* node, int depth);
	int get_total_search_time(Node<T>* node);
	virtual void pretty_print_node(ostream& out,Node<T>* node);
    vector<Node<T>*> level_by_level_helper(vector<Node<T>*>& treeVector, Node<T>* node, int pos);
};

template <typename T>
ostream& operator<<(ostream& out, BTree<T>& tree)
{
    out << "Value \tSearch Cost" << endl;
    out << tree.get_root() << endl;
	return out;
}

template <typename T>
ostream& operator<<(ostream& out, Node<T>* node)
{
    if (node != nullptr){
        out << "  " << node->value << "\t     " << node->search_time << endl;
        out << node->left;
        out << node->right;
    }
    return out;
}

template <typename T>
void BTree<T>::pretty_print_node(ostream& out,Node<T>* node)
{
	out << node->value << "["<<node->search_time<<"] ";
    //
}

template <typename T>
istream& operator>>(istream& in, BTree<T>& tree)
{
    int x;
    in >> x;
    tree.insert(x);
    return in;
}

template <typename T>
BTree<T>::BTree(const BTree<T>& other)
{
    *this = other;
}

template <typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& other)
{
    if (other.root == nullptr) root = nullptr;
    else copy_helper(root, other.root);
    return *this;
}

/*template <typename T>
BTree<T>& BTree<T>::operator=(const BTree<T> other)
{
 
		complete this assignment operator
		make sure when copying nodes to copy
			the value
			the relative parent, and left and right children
			the node_color
				not important for this part but will be used later
 
}*/

template <typename T>
void BTree<T>::copy_helper(Node<T>* copy_to, const Node<T>* copy_from) const
{
    //Base Case: reach end of branch
    if (copy_from == nullptr) copy_to = nullptr;
    else {
        copy_to = new Node<T>;
        copy_to->value = copy_from->value;
        copy_to->parent = copy_from->parent;
        copy_to->node_color = copy_from->node_color;
        copy_helper(copy_to->left, copy_from->left);
        copy_helper(copy_to->right, copy_to->right);
    }
}

template <typename T>
Node<T>* BTree<T>::insert_helper(T obj, Node<T>* curr)
{
    // if tree is empty, return a new node and increment size
    if (curr == nullptr){
        size += 1;
        Node<T> *new_node = new Node<T>(obj);
        return new_node;
    }
    
    // Otherwise, recur down the left...
    if (obj < curr->value){
        Node<T>* lchild = insert_helper(obj, curr->left);
        curr->left = lchild;
        lchild->parent = curr;
    }
    //...or the right of tree
    else if (obj > curr->value){
        Node<T>* rchild = insert_helper(obj, curr->right);
        curr->right = rchild;
        rchild->parent = curr;
    }
    return curr;
}

template <typename T>
Node<T>* BTree<T>::insert(T obj)
{
    //cout << "size: " << size << endl;
    //cout << "obj: " << obj << endl << endl;
    if (root == nullptr){
        root = insert_helper(obj, get_root());
        return root;
    }
    else{
        Node<T> *new_node = insert_helper(obj, get_root());
        return new_node;
    }
}

template <typename T>
Node<T>* BTree<T>::search_helper(T obj, Node<T>* node)
{
    //Base case: node reaches null or finds obj
    if (node == nullptr || node->value == obj)
        return node;
    
    //obj is less than node->value
    if (obj < node->value)
        return search_helper(obj, node->left);
    
    //obj is greater thn node->value
    return search_helper(obj, node->right);
}

template <typename T>
Node<T>* BTree<T>::search(T obj)
{
    Node<T> *new_node = search_helper(obj, get_root());
    return new_node;
}	

template <typename T>
void BTree<T>::update_search_times_helper(Node<T>* node, int depth)
{
    if (node != nullptr){
        node->search_time = depth + 1;
        update_search_times_helper(node->left, depth+1);
        update_search_times_helper(node->right, depth+1);
        if (levels < depth +1)
            levels = depth +1;
    }
}

template <typename T>
void BTree<T>::update_search_times()
{
    if (root != nullptr){
        update_search_times_helper(get_root(), 0);
    }
}

template <typename T>
ostream& BTree<T>::inorder_helper(ostream& out, Node<T>* node)
{
    if (node != nullptr){
        inorder_helper(out, node->left);
        out << node->value << "[" << node->search_time << "] ";
        inorder_helper(out, node->right);
    }
    return out;
}

template <typename T>
ostream& BTree<T>::inorder(ostream& out)
{
	/*
		print your nodes in infix order
		if our tree looks like 

		4
		2 6
		1 3 5 7

		we should get

		1 2 3 4 5 6 7 
	*/
    if (root != nullptr){
        out << "Value[SearchCost]" << endl;
        inorder_helper(out, get_root());
        out << endl;
    }
    
	return out;
}

template <typename T>
vector<Node<T>*> BTree<T>::level_by_level_helper(vector<Node<T> *>& treeVector, Node<T> *node, int pos){
    if (node != nullptr){
        //cout << "pos: " << pos << endl;
        treeVector.at(pos) = node;
        //cout << "value at pos: " << treeVector.at(pos)->value << endl;
        level_by_level_helper(treeVector, node->left, 2*pos);
        level_by_level_helper(treeVector, node->right, 2*pos+1);
    }
    return treeVector;
}

template <typename T>
ostream& BTree<T>::print_level_by_level(ostream& out)
{
	/*
		print the tree using a BFS 
		output should look like this if we dont have a full tree

		4
		2 6
		1 X 5 7
		X X X X X X X 9

		it will be helpfull to do this part iterativly, 
		so do the BFS with the std stack data structure.

		it may also be helpfull to put the entire tree into a vector 
		(probably google this if you dont know how to do it)
	*/
    vector<Node<T>*> treeVector(pow(2,levels),nullptr);
    treeVector = level_by_level_helper(treeVector, root, 1);
    Node<T>* curr;
    for (int i = 0; i <= levels-1; ++i){
        for(int j = pow(2,i); j < pow(2,i+1); ++j){
            curr = treeVector.at(j);
            if (curr == nullptr)
                out << "X ";
            else
                out << curr->value << "[" << curr->search_time << "] ";
        }
        out << endl;
    }
    
    return out;
}

template <typename T>
int BTree<T>::get_total_search_time(Node<T>* node)
{
    if (node != nullptr)
        return get_total_search_time(node->left) + node->search_time + get_total_search_time(node->right);
    return 0;
}

template <typename T>
float BTree<T>::get_average_search_time()
{
    float tot = get_total_search_time(get_root());
    float avg = tot / size;
    //avg *= 100;
    //avg = ceil(avg);
    //avg /= 100;
    //cout << "avg: " << avg << endl;
    //cout << "total: " << tot << endl;
    //cout << "size: " << size << endl;
    return avg;
}
