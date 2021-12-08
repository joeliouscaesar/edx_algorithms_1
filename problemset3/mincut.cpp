/*
	Okay, I was thinking we could used a linked list
	for the edges, and then keep an array of nodes
	
	1) Start with a linked list of edges
	2) Delete self-loops in the list of edges
	3) Select edge at random, pick one vertex to mush with the other,
		add it and all of its children as children of the non-mushed 
		vertex. 
	4) Loop through the list of edges, if we find the mushed vertex change it.
		Delete any self loops as well.
	5) Subtract 1 from the number of vertexes. 
	6) Repeat until there are 2 vertices left. 
	7) Count the number of non self-loop edges left, that's our cut edge count
	
	Params: Pass in the file name as the first param
	
*/


#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<unordered_set>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

//edge data type
struct Edge{
	
	//indexes for nodes
	int* vertices; 
	
	//indexes of the next_edge, prev_edge in the list 
	Edge* next_edge;
	Edge* prev_edge; 
	
	//default constructor
	Edge(int a, int b){
		vertices = new int(2);
		vertices[0] = a; vertices[1] = b;
		next_edge = nullptr; 
		prev_edge = nullptr; 
	}
	
	//(but not a constructor)
	void print(){
		cout<<"edge: ("<<vertices[0]<<", "<<vertices[1]<<")"<<endl;
	}
	
	~Edge(){
		// cout<<"deleting edge ("<<vertices[0]<<","<<vertices[1]<<")"<<endl;
		delete[] vertices;
	}
	
};

//node data type
struct Node{
	
	//unordered set of the child nodes
	unordered_set<int> children;
	
	void acquire_children(Node *node){
		
		//iterator variable, we'll use as an indicator for 
		//whether that num was found
		auto notfound = children.end();
		
		//loop through the children of the other node
		for(auto k = node->children.begin(); k != node->children.end(); k++){
			//check if this element is a child of this children set, if not 
			//throw it in!
			notfound = children.find(*k); 
			
			if(notfound == children.end()){
				//wasn't found, so we add 
				children.insert(*k);
			} 
			
		}
		
	}
	
	~Node(){
		// cout<<"deleting Node"<<endl; 
	}
};

//function for parsing line of our input file
vector<int> parse_line(string word){
	vector<int> out; 
	int num, i,j;
	num = -1; 
	i = 0;
	while(i < word.length()){
		//found a digit
		if(word[i] >= 48 & word[i] <= 57){
			num = ((int) word[i])-48;
			j = i+1;
			while(j < word.length()){
				if(word[j] >= 48 & word[j] <= 57){
					num = num*10+((int) word[j]) - 48;
				}
				else{
					//we're done with the num in this case
					i = j;
					break;
				}
				//in this case we keep moving this pointer
				j++;
			}
			
			//set i to where we left off, add num
			i = j;
			out.push_back(num); 
		}
		i++;
	}
	return(out);
}

//generates a random number in 0 to range-1
int rand_in_range(int range){
	return(rand() % range);
}

//runs at iteration of the karger min cut algo
int karger_cut_algo(string file){
	
	//for the random number generator
	srand(time(NULL));
	
	////////////////////////////////////////////
	//Reading the input file
	////////////////////////////////////////////

	//vector of Node pointers
	std::vector<Node*> nodes;
	//vector of Edge pointers
	std::vector<Edge*> edges; 
	Edge* ed; 
	
	//number of edges/vertices
	int num_edges, num_vertices;
	num_edges = 0; 
	num_vertices = 0;
	
	//reading in the file!
	fstream newfile;
	newfile.open(file.c_str(),ios::in);
	if(newfile.is_open()){
		string li; 
		vector<int> split_line;
		
		while(getline(newfile,li)){
			
			//handle the line 
			split_line = parse_line(li);
			
			//adding a node to the node vector
			nodes.push_back(new Node());
			num_vertices++;
			
			//adding edges
			for(int i = 1; i < split_line.size(); i++){
				
				//do this so we don't have duplicates
				if(split_line[i] < split_line[0]){
					continue;
				}
				
				//setting the pointers
				if (num_edges > 0){
					ed = edges.back();
				}

				edges.push_back(new Edge(split_line[0],split_line[i]));
				
				if(num_edges > 0){
					edges.back()->prev_edge = ed;
					ed->next_edge = edges.back();	
				}
				
				num_edges++;
			}
		}
		newfile.close();
	}else{
		// cout<<"couldn't open file: "<<file <<endl;
		return(-1);
	}
	
	// cout<<"num_edges: "<<num_edges<<" size of edges: "<<edges.size()<<endl;
	// cout<<"num_vertices: "<<num_vertices<<" size of nodes: "<<nodes.size()<<endl;

	////////////////////////////////////////////
	//Karger Algorithm
	////////////////////////////////////////////
	
	//pointers to the first edge, current edge
	Edge* first_edge; 
	Edge* this_edge;
	first_edge = edges.front();	
		
	//edge integer variables
	int edge_to_mush, edge_num_iter;
	
	//node integer variables
	int node_to_mush, node_thats_mushing; 
	
	while (num_vertices > 2){
		
		//draw the edge_to_mush
		edge_to_mush = rand_in_range(num_edges);
		
		// 1) Select edge at random, pick one vertex to mush with the other,
		// 	add it and all of its children as children of the non-mushed 
		// 	vertex. 
		
		this_edge = first_edge; 
		for(edge_num_iter = 0; edge_num_iter < edge_to_mush; edge_num_iter++){
			//sets this_edge to the next edge
			this_edge = this_edge->next_edge;  
		}
	
		//choosing the min to mush
		if(this_edge->vertices[0] < this_edge->vertices[1]){
			node_to_mush = this_edge->vertices[0];
			node_thats_mushing = this_edge->vertices[1];
		}else{
			node_to_mush = this_edge->vertices[1];
			node_thats_mushing = this_edge->vertices[0];
		}
		
		//transferring children
		nodes[node_thats_mushing]->acquire_children(nodes[node_to_mush]); 
		
		// 2) Loop through the list of edges, if we find the mushed vertex change it.
		// 	Delete any self loops as well. Update num_edges as we go. 
		
		for(this_edge = first_edge; this_edge != nullptr; this_edge = this_edge->next_edge){
				
			if(this_edge->vertices[0] == node_to_mush){
				this_edge->vertices[0] = node_thats_mushing;
			}
			else if(this_edge->vertices[1] == node_to_mush){
				this_edge->vertices[1] = node_thats_mushing; 
			}
			
			if(this_edge->vertices[0] == this_edge->vertices[1]){
				
				//setting the other edges in our linked list 
				//deleting this edge
				
				if(this_edge == first_edge){
					//if it's the first edge
					first_edge = this_edge->next_edge;
					this_edge->next_edge->prev_edge = nullptr;
				}
				else if(this_edge->next_edge == nullptr){
					//if it's the last edge
					this_edge->prev_edge->next_edge = nullptr;
				}
				else{
					//if it's a middle edge
					this_edge->prev_edge->next_edge = this_edge -> next_edge; 
					this_edge->next_edge->prev_edge = this_edge -> prev_edge;
				}
				
				// delete this_edge;
				num_edges--; 
			}
			
		}
		
		// 3) Subtract 1 from the number of vertexes. 
		num_vertices--; 
		
	}

	//cleaning up , deletes all of the edges and nodes
	for(auto k = edges.begin(); k != edges.end(); k++){
		delete (*k);  
	}
	
	for(auto k = nodes.begin(); k != nodes.end(); k++){
		delete (*k);
	}
	
	//output!
	// cout<<"num edges across cut: "<<num_edges<<endl;
	
	return(num_edges);
}


int main(int argc, char* argv[]){
	
	cout<<"entered main"<<endl; 
	string file;
	
	if(argc == 1){
		cout<<"not passed file, exiting"<<endl;
		return(1);
	}else{
		cout<<"file: "<<argv[1]<<endl;
		file = argv[1]; 
	}
	
	int val; 
	int min = karger_cut_algo(file);
	for(int i = 0; i < 10000; i++){
		val = karger_cut_algo(file);
		if(val < min)
			min = val; 
	}
	
	cout<<"min after 10000 iterations:"<<min<<endl;
	
	return(0); 
}