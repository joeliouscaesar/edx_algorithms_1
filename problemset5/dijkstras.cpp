/*
	This file implements Dijkstra's shortest
	path algorithm 
	
	- Maintain a heap of Nodes, their keys are the greedy criterion
	- Maintain an array of Nodes who's shortest paths we already have
	- Take the smallest key, add it to the array, remove it from the heap
	- Loop through newest Node's edges, update the corresponding keys

*/
#include<iostream>
#include<fstream>
#include<vector>
#include<cassert>
#include "dijkstrastructs.h" 

using namespace std; 

// function which parses a line for a node
void parseline(string line, vector<struct Node*> nodes){
	
	//get the node id 
	int i = 0; 
	int num = 0; 
	while(i < line.length()){
		
		if(line[i] == 9){
			//break if we find a space or tab
			i++;
			break; 
		} 
		if(line[i] >= 48 && line[i] <= 57){
			//found a number
			num = (line[i] - 48) + num*10;   		
		}
		i++; 
	} 
	
	//index for this node in our nodes vector 
	int thisind = num - 1; 
	
	//check we're okay 
	assert(num > 0 && num < 201);
	
	//get the remaining pairs 
	num = 0; 
	
	//indicator if this is the edge num or weight
	while(i < line.length()){
		
		if(line[i] >= 48 && line[i] <= 57){
			
			//found a number
			num = (line[i] - 48) + num*10;   
			
		}else if(line[i] == 44){
			
			//found a comma, adds edge
			nodes[thisind]->edges.push_back(nodes[num - 1]);
			num = 0; 
		
		}else if(line[i] == 9){
			
			//found a tab, add weight
			nodes[thisind]->lengths.push_back(num); 
			num = 0; 
		}
		i++; 
	} 
	
	return; 

}

int main(){
	
	//constructs our node vector
	vector<struct Node*> nodes; 
	for(int i = 0; i < 200; i++){
		nodes.push_back(new Node(i+1, i)); 
	} 
	
	/*************************
	* Data input
	*************************/
	
	const char filepath[] = "./dijkstraData.txt";  
	fstream datafile; 
	datafile.open(filepath, ios::in); 
	if(datafile.is_open()){
		
		cout<<"file open"<<endl;  
		string li; 
		
		while(getline(datafile, li)){
			parseline(li, nodes); 
		}
	}
	datafile.close(); 
	cout<<"datafile closed"<<endl;
	
	//quick validity check 
	bool firstzero = nodes[0]->key == 0; 
	bool restmax = true; 
	for(int i = 1; i < nodes.size(); i++){
		if(nodes[i]->key < INT_MAX){
			restmax = false; 
			break; 
		}
	}
	
	if(firstzero){
		cout<<"top node has key 0"<<endl; 
	}
	if(restmax){
		cout<<"all other nodes have key INT_MAX"<<endl; 
	}
	
	//constructing our node heap 
	struct NodeHeap* heap = new NodeHeap(nodes); 

	/*************************
	* Algorithm 
	*************************/
	int numnodes = nodes.size(); 
	int* shortestpaths = new int[numnodes]; 
	
	struct Node* min; 
	//get the min element 
	min = heap->min(); 
	while (min != nullptr){
		//loop while the heap is non-empty 
		
		//removes min element from the heap, stores its shortest path 
		heap->pop(); 
		shortestpaths[(min->id) - 1] = min->key; 
		
		//updates keys for the other elements 
		heap->keyupdate(min); 
		
		//delete this node object 
		delete min; 
		//re-assign the min pointer 
		min = heap->min(); 
	}

	/*************************
	* Cleaning up 
	*************************/

	cout<<"Shortest Paths:"<<endl; 
	for(int i = 0; i < numnodes; i++){
		cout<<(i+1)<<": "<<shortestpaths[i]<<endl; 
	}

	delete heap; 
	delete[] shortestpaths; 
	
	return(0); 
	
}

