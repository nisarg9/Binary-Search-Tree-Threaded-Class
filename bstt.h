//Nisarg Thakkar
//Project 4 Threaded Binary Search Tree

/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  NODE* LeftMost;  //Check instate order for inorder traversal in next and begin
  int   Size = 0;  // # of nodes in the tree (0 if empty)
  
  void _inorder(NODE* cur, ostream& output) const  //go to left, do the thing, go to right (recursive)
  {
	  if (cur == nullptr)
		  return;
	  
	  else
	  {
	  //(key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
		  
		  _inorder(cur->Left, output);
		  if ((!(cur->isThreaded)) || (cur->isThreaded && cur->Right == nullptr))
			  output << "(" << cur->Key << "," << cur->Value << ")" << endl;
		  else
			  output << "(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
		  if (!(cur->isThreaded))
			  _inorder(cur->Right, output);
	  }
  }
  
  void _preorder(NODE* cur)  //pre_order do the thing (insert in this case, helps making deep copy, go to left, go to right
  {
	  if (cur == nullptr)
	  {  return;}
	
	  
	insert(cur->Key, cur->Value);
	
	_preorder(cur->Left);
	
	if (!(cur->isThreaded))
	{		
	  _preorder(cur->Right);
	 }
	  
  }
  void _des(NODE* cur)  //destructor helper function, go to left, go to right, start deleting (bottom up)
  {
	  if (cur == nullptr)
		  return;
	  _des(cur->Left);  //to left
	  if (!(cur->isThreaded))  //to right, if not threaded
		_des(cur->Right);
	  delete(cur);   //delete
  }


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
	LeftMost = nullptr;
    Size = 0;
  }

  
  //
  // copy constructor
  //Copies other to this, first initalizes all variables
  bstt(const bstt& other)
  {
		this->Size = 0;
		this->Root = nullptr;
		this->LeftMost = nullptr;
	
	_preorder(other.Root);
  }

	//
  // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  //
  virtual ~bstt()
  {
    _des(Root);
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
    clear();
    
	_preorder(other.Root);
	
	//copy constructor
	//bsst this(other);

    return *this;
  }

  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
  void clear()
  {
  
  if (Root != nullptr)
    _des(Root);
    Size = 0;
	Root = nullptr;
	LeftMost = nullptr;
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    NODE* cur = Root;
	
	while(cur != nullptr)
	{
		if(key == cur->Key)
		{
			value = cur->Value;  //if key found, returned
			return true;
		}
			
		if(key < cur->Key)     //otherwise go to left node recursively
			cur = cur->Left;
		else
		{
			if (cur->isThreaded)     
				cur = nullptr;
			else
				cur = cur->Right;   //and go to right
		}
	}
    
		return false; //if here no keys found
  }

  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {
	  //search tree first
	
		
    NODE* prev = nullptr;
	NODE* cur = Root;
	
	while (cur != nullptr)
    {
	  if (key == cur->Key)  // already in tree
        return;

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else  //search right
      {
		if (!(cur->isThreaded))
		{
			prev = cur;
			cur = cur->Right;
		}
		else   //if threaded, is like nullptr sets cur to nullptr
		{
			prev = cur;
			cur = nullptr;
		}
      }
    }//while
		
	//if not found assigns new node
	NODE* n = new NODE();
	n->Key = key;
	n->Value = value;
	n->Left =nullptr;
	n->Right = nullptr;
	n->isThreaded = true;
	
	if (prev == nullptr)
		Root = n;   //if first node entered, becomes Root
		
	else if (key < prev->Key)  //if inserted to left, link in with existing structure
	{
		prev->Left = n;   
		n->Right = prev;
	}
	else             //if inserted to right, link in with existing tree
	{
		n->Right = prev->Right;
		prev->Right = n;
		
		prev->isThreaded = false;
	}
	
	Size++;   //add size
	
  }

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    bool temp;
	ValueT val;
	
	temp = search(key, val);   //if key exist, temp will be true and value returned
	if(temp)
		return val;

    return ValueT{ };    //otherwise return defaultvalue for that type
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    NODE* cur = Root;
	NODE* temp = nullptr;
	
	while(cur != nullptr)  //searches if key exists
	{
		if(key == cur->Key)   //key found
		{
			temp = cur;
			cur = nullptr;
		}
			
		else if(key < cur->Key)
			cur = cur->Left;
			
		else
		{
			if (cur->isThreaded)
				cur = nullptr;				
			else
				cur = cur->Right;
		}
	}
	if(temp != nullptr) // key found
	{
		if((temp->isThreaded && temp->Right != nullptr) || (!(temp->isThreaded))) //threaded and not at end (pointing to null) or not threaded
			return temp->Right->Key;				 //returns right inorder key
	}
	return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    if(this->Root == nullptr)
	{	
		LeftMost = nullptr;
		return; 
	}
	
	NODE* cur = this->Root;
	
	while(cur->Left != nullptr)   //sets the smallest, leftmost pointer to leftmost to start inorder traversal
	{
		cur = cur->Left;
	}
	
	LeftMost = cur;
	
  }

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
	while (LeftMost != nullptr)
	{ 
		key = LeftMost->Key;
		if(LeftMost->isThreaded)    //takes care of leaf nodes, sets leftmost to its next inorder node
		{
			LeftMost = LeftMost->Right;
		}
		else     //takes care of internal nodes
		{
			LeftMost = LeftMost->Right;      //going to non threaded right and going its extreme left
			while (LeftMost->Left != nullptr)
			{
				LeftMost = LeftMost->Left;
			}
		}
		return true;		//key found state changed
	}
    return false;  //False is returned when the internal state has reached null, meaning no more keys are available. 
  }

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;
	_inorder(Root, output);  //calls helper function to print inorder with ostream type
    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //


    output << "**************************************************" << endl;
  }
	
};

