#include "BTree.h"
#include <iostream>
#include <fstream>
#include <sstream>

template<typename T>
BTree<T> read_file(string file_name);

using namespace std;

int main()
{
    /*//Print average search cost for 12 input files at a time
     for (int i = 1; i <= 12; ++i){
        ostringstream oss;
        oss << i;
        string in = oss.str();
        in += "l";
        cout << in << endl;
        BTree<int> new_tree = read_file<int>(in);
        new_tree.update_search_times();
        cout << "Average cost: " << new_tree.get_average_search_time() << endl << endl;
    }*/
    
    //Declare Input/Output
    string inFileName = "4p";
    string outFileName = inFileName;
    outFileName += "out";
    
    //Make/populate new tree
	BTree<int> new_tree = read_file<int>(inFileName);
    new_tree.update_search_times();
    
    //Open output file
    ofstream out;
    out.open(outFileName);
    
    //Check if open
    if (!out.is_open()){
        cerr << "Error: Could not open output file" << endl;
        return 1;
    }
    
    //Print with operator<<
    out << new_tree;
    
    //Print in order with Average Search Cost
    new_tree.inorder(out);
    out << "Average search cost: " << new_tree.get_average_search_time() << endl << endl;
    
    //Print level by level
    new_tree.print_level_by_level(out);
    if (new_tree.get_size() < 16){
        new_tree.print_level_by_level(cout);
        cout << "Average search cost: " << new_tree.get_average_search_time() << endl << endl;
    }
    
}

template<typename T>
BTree<T> read_file(string file_name)
{
    ifstream in;
    BTree<T> new_tree;
    in.open(file_name);
    if(!in.is_open()){
        cerr << "Error: Could not open input file" << endl;
        //return 1;
    }
    while (!in.eof()){
        if (in.good())
            in >> new_tree;
    }
    return new_tree;
}
