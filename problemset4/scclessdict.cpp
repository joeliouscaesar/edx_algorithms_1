#include<iostream>
#include<array>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<stack>
#include<fstream>
#include<cassert>
#include<algorithm>

using namespace std;

/*
	This took a long time! 
	
	This computes strongly connected components for a directed graph,
	the algorithm does depth first search twice. 
	
	It reads the graph in two parts because of memory, which was 
	an issue earlier. This might not have mattered though, the 
	bigger culprit ended up being the recursive calls 
	for the first and second rounds of dfs. This implements
	it iteratively which is WAY faster. It could be made more 
	efficient by using an array to hold the node pointers instead 
	of a hash table, but the hash table has the advantage of 
	being able to read in a subset of the edges. 
	
	My IO method is probably slow too so I could look into that. 
	
	
*/

struct Node{
	int scc, id; //scc
	vector<int> edges;
	
	//constructor that doesn't take another node
	Node(int id0){
		scc = -1;
		id = id0; 
	}
	
	Node(int id0, int othernode){
		scc = -1;
		id = id0; 
		edges.push_back(othernode); 
	}

	//prepares Node for round 2 of DFS
	void clean_edge_vector(){
		//clears points to and guarantees memory reallocation
		vector<int>(0).swap(edges); 
	}
	
	//adds an edge
	void add_edge(int othernode){
		edges.push_back(othernode); 		
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


int main(){
	
	string filepath = "./SCC.txt"; 
	int maxlines = 6000000; 
	
	/********************
	* Reading in edges
	********************/
	fstream newfile; 
	newfile.open(filepath.c_str(),ios::in); 
	unordered_map<int, struct Node*> nodes; 
	
	int head = 0; 
	int tail = (head + 1) % 2; 
	int numedges = 0;
	cout<<"head "<<head<<" tail "<<tail<<endl;
	
	//reading in Nodes, 
	if(newfile.is_open()){
		  
		cout<<"file open"<<endl;  
		  
		string li; 
		vector<int> split_line;
	
		while(numedges < maxlines){
			
			if(!getline(newfile, li)){
				//no more lines
				break;
			} 
			numedges++; 
			
			split_line = parse_line(li);
			assert(split_line.size() == 2); 
			
			//check if the nodes are in our dict yet
			auto nodepairptr = nodes.find(split_line[head]); 
			if(nodepairptr == nodes.end()){
				pair<int,struct Node*> newpair(split_line[head], new Node(split_line[head], split_line[tail])); 
				//this might return an iterator to the pair but alas
				nodes.insert(newpair); 
				
			}else{
				(*nodepairptr).second->add_edge(split_line[tail]);
			}
			
			//TODO for the other node
			nodepairptr = nodes.find(split_line[tail]); 
			if(nodepairptr == nodes.end()){
				pair<int,struct Node*> newpair(split_line[tail], new Node(split_line[tail])); 
				//this might return an iterator to the pair but alas
				nodes.insert(newpair); 
			}
		}
		
	}
	newfile.close();
	
	cout<<"file closed, numlines: "<<numedges<<endl; 
	cout<<"num nodes: "<<nodes.size()<<endl; 

	/********************
	* First DFS
	********************/

	cout<<"beginning first dfs"<<endl; 

	// "stack" with our finishing times 
	int* finishing_times = new int[nodes.size()]; 
	int nodesfinished = 0; 
	
	//actual stack for our dfs calls 
	stack<struct Node*> dfscalls;
	struct Node* topnode = nullptr;  
	struct Node* secondnode = nullptr; 
	for(auto nodec = nodes.begin(); nodec != nodes.end(); nodec++){
		
		//add to the stack if not explored
		if((*nodec).second->scc < 0){
			dfscalls.push((*nodec).second); 
		}
		
		while(!dfscalls.empty()){
			
			topnode = dfscalls.top(); 
			topnode->scc = 0; 
			auto edgeiter = topnode->edges.begin();
			
			//loop through edges until we find a non-explored edge
			while(edgeiter != topnode->edges.end()){
				secondnode = (*(nodes.find(*edgeiter))).second;  
				if(secondnode->scc < 0){
					//if not explored add it, break 
					dfscalls.push(secondnode); 
					break;
					
				}else{
					edgeiter++; 
				}
			}
			
			if(edgeiter == topnode->edges.end()){
				//we've exhausted this node's edges, so pop
				finishing_times[nodesfinished] = topnode->id;
				nodesfinished++; 
				dfscalls.pop(); 
				
			}else{
				//we added an edge so continue
				continue; 
			}
		}
	} 

	/**************************
	* Prepping for second DFS
	**************************/
	cout<<"done with first DFS, prepping second"<<endl; 
	
	//clear the old edges
	for(auto k = nodes.begin(); k != nodes.end(); k++){
		(*k).second->clean_edge_vector(); 
	}

	//read in edges going the opposite way
	newfile.open(filepath.c_str(),ios::in);  

	head = 1; 
	tail = (head + 1) % 2; 
	cout<<"head "<<head<<" tail "<<tail<<endl;
	numedges = 0; 
	
	//reading in Nodes, 
	if(newfile.is_open()){
		  
		cout<<"file open"<<endl;  
		  
		string li; 
		vector<int> split_line;
	
		while(numedges < maxlines){
			
			if(!getline(newfile, li)){
				//no more lines
				break;
			} 
			numedges++; 
			 
			split_line = parse_line(li);
			assert(split_line.size() == 2); 
			
			//check if the nodes are in our dict yet
			auto nodepairptr = nodes.find(split_line[head]); 
			(*nodepairptr).second->add_edge(split_line[tail]);
			
		}
		
	}
	newfile.close();
	
	//sanity check 
	cout<<"file closed, new numedges: "<<numedges<<endl; 
	
	/********************
	* Second DFS
	********************/	
	cout<<"beginning second DFS"<<endl; 
	
	//have to do this in the reverse finishing times order
	for(int k = nodes.size() - 1; k >=0; k--){
		
		topnode = (*nodes.find(finishing_times[k])).second; 
		
		//add to the stack if not explored
		if(topnode->scc == 0){
			dfscalls.push(topnode); 
		}
		
		while(!dfscalls.empty()){
			
			topnode = dfscalls.top(); 
			topnode->scc = finishing_times[k];
			
			auto edgeiter = topnode->edges.begin();
			//loop through edges until we find a non-explored edge
			while(edgeiter != topnode->edges.end()){
				secondnode = (*(nodes.find(*edgeiter))).second;  
				if(secondnode->scc == 0){
					//if not explored add it, break 
					dfscalls.push(secondnode); 
					break;
					
				}else{
					edgeiter++; 
				}
			}
			
			if(edgeiter == topnode->edges.end()){
				//we've exhausted this node's edges, so pop
				dfscalls.pop(); 
				
				
			}else{
				//we added an edge so continue
				continue; 
			}
		}
	} 


	/************************
	* Counting biggest scc's
	************************/
	
	cout<<"done second dfs, counting scc sizes"<<endl; 
	
	//making room 
	delete[] finishing_times; 

	for(auto k = nodes.begin(); k != nodes.end(); k++){
		//clearing all the edge vectors
		(*k).second->clean_edge_vector(); 
	}
	
	//dictionary, keys are scc, values are counts 
	unordered_map<int,int> scc_counts; 
	auto scc_count_loc = scc_counts.find(0);
	struct Node* currnode = nullptr; 
	//counting the number of values in each scc
	for(auto k = nodes.begin(); k != nodes.end(); k++){
		
		currnode = (*k).second; 
		
		scc_count_loc = scc_counts.find(currnode->scc);
		if(scc_count_loc == scc_counts.end()){
			//not found
			pair<int,int> newsccpair(currnode->scc,1);
			scc_counts.insert(newsccpair);
			
		}else{
			//found 
			(*scc_count_loc).second++;
		}
		
		delete currnode; 
		
	}	
	
	array<int,5> largest_sccs{0,0,0,0,0}; 
	int val, t; 
	for(auto k = scc_counts.begin(); k != scc_counts.end(); k++){
		//loop over scc's, count the biggest
		val = (*k).second; 
		t = 5; 
		while(t > 0){
			if(largest_sccs[t - 1] < val){
				if(t < 5){
					largest_sccs[t] = largest_sccs[t-1]; 
				}
				largest_sccs[t-1] = val; 
				t--; 
			}else{
				break; 
			}
		}
	}
	
	cout<<"5 largest sccs: ";
	for(int k = 0; k < 5; k++){
		cout<<largest_sccs[k]<<" ";
	}
	cout<<endl; 
	
	/********************
	* Cleaning up 
	********************/

	//cleaned up in the previous section
	return(0);
	
}	