/*
	This file contains the structs we have 
	for the Dijkstra shortest path algorithm 
*/


#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

//node 
struct Node{

	int id; //node id
	int key; //dijkstra's greedy criterion
	int loc; //location in the heap
	
	//vector of pointers to other Nodes, represent edges
	vector<struct Node*> edges;
	//vector of edge lengths corresponding to the above vector
	vector<int> lengths;
	
	Node(int id, int loc){
		
		this->id = id; 
		this->loc = loc;
		
		if(id == 1){
			key = 0;
		}else{
			key = INT_MAX; 
		}
	}

}; 


// heap for Node pointers
struct NodeHeap{

private:	
	vector<struct Node*> nodes; 
	
	//function which calculates the parent
	//index for a given index
	//returns -1 if ind == 0
	int parentindex(int ind){
		if(ind == 0){
			//no parent
			return(-1); 
		}else{
			//this took a while but I feel good about this
			if(ind % 2 == 0){
				return(ind/2-1); 
			}else{
				return((ind+1)/2 -1); 
			}
		}
	}

	void relocate(int nodeind){
		//moves node in a tree, checks both directions
		//so I can use in the other functions 
		
		//first check if parent is greater than this node 
		int parent = parentindex(nodeind); 
		struct Node* temp; 
		while(parent >= 0){
			if(nodes[parent]->key > nodes[nodeind]->key){
				
				//swap 
				nodes[nodeind]->loc = parent; 
				nodes[parent]->loc = nodeind; 
				
				temp = nodes[parent];
				nodes[parent] = nodes[nodeind];
				nodes[nodeind] = temp;
				
				nodeind = parent; 
				parent = parentindex(nodeind); 
				
			}else{
				break; 
			}
		}
		
		//okay now we check children 
		int c1 = 2*(nodeind + 1)-1;
		int c2 = 2*(nodeind + 1); 
	
		while(c1 < nodes.size() || c2 < nodes.size()){
			
			if(c2 < nodes.size()){
				//get the index for the smaller node 
				c1 = nodes[c1]->key < nodes[c2]->key ? c1 : c2; 
			}
			
			if(nodes[c1]->key < nodes[nodeind]->key){
				
				//swap 
				nodes[nodeind]->loc = c1; 
				nodes[c1]->loc = nodeind; 
				
				temp = nodes[c1];
				nodes[c1] = nodes[nodeind];
				nodes[nodeind] = temp;
				
				nodeind = c1; 
				c1 = 2*(nodeind + 1)-1;
				c2 = 2*(nodeind + 1);
			}else{
				break; 
			}
		}
		
		return; 
	}

public:
	
	NodeHeap(){
		//constructs a SelfHeap
	}

	NodeHeap(vector<struct Node*> nodes){
		//ASSUMES nodes is a valid heap, e.g. nodes is sorted by key 
		this->nodes = nodes; 
	}

	//returns nullptr if no elements
	struct Node* min(){
		if(nodes.size() > 0){
			return(nodes[0]); 
		}else{
			cout<<"no minimum! 0 elements"<<endl;
			return(nullptr); 
		}
	}
		
	//removes the min node
	void pop(){
		
		if(nodes.size() == 0){
			cout<<"no nodes"<<endl; 
			return; 
		}
		
		//sets first element to be 
		//the last element
		nodes[0] = nodes.back();
		nodes[0]->loc = 0; 
		nodes.pop_back(); 
		
		//fixes our heap
		relocate(0); 
		
		return; 
	}
	
	void insert(struct Node* newnode){
		
		//adds new node to the back
		nodes.push_back(newnode); 
		newnode->loc = nodes.size() - 1; 
		relocate(nodes.size() - 1); 
		return; 
	}
	
	//updates the keys after removing a node 
	void keyupdate(struct Node* poppednode){
		
		int pnodekey = poppednode->key;
		int newkey = 0; 
		struct Node* node; 
		
		for(int i = 0; i < poppednode->edges.size(); i++){
			node = poppednode->edges[i]; 
			newkey = poppednode->key + poppednode->lengths[i]; 
			
			if(node->key > newkey){
				node->key = newkey; 
				relocate(node->loc); 
			}
		}
		return; 
		
	}
	
	void display(){
		int rowcount = 1; 
		int cumulative = rowcount; 
		int index = 0; 
		while(index < nodes.size()){
			
			if(index == cumulative){
				cout<<endl; 
				rowcount*=2; 
				cumulative+=rowcount; 
			}
			
			cout<<nodes[index]->id;
			if(nodes[index]->id < 10){
				cout<<"  ";
			}else if(nodes[index]->id < 100){
				cout<<" ";
			}
			index++; 	
		}
		cout<<endl; 
		return; 
	}
	
}; 