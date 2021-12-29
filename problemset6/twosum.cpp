/*
	This file computes the number of target values, t, 
	in the interval [-10000, 10000] s.t. there exist integers 
	x, y from our data file that sum to t 

	This is pretty slow, it would probably be faster looping over 
	the set and then looping over t, but either way. 
	
*/

#include<iostream>
#include<unordered_set>
#include<fstream>
#include<cassert>

using namespace std; 

//gets the number from this line 
long long int getnum(string s){
	long long int num = 0;
	bool negative = false; 
	for(int i = 0; i < s.length(); i++){
		if(s[i] == 45){
			//found a hyphen, probably negative
			negative = true;
		}else if(s[i] >= 48 & s[i] <= 57){
			//add to num
			num = num*10+(s[i]-48); 
		}
	}
	if(negative){
		return((-1)*num); 
	}else{
		return(num); 
	}
}


int main(){
	
	/**********************
	* IO
	**********************/
	
	string filepath = "./algo1-programming_prob-2sum.txt"; 
	fstream fi; 
	fi.open(filepath, ios::in); 
	if(!fi.is_open()){
		cout<<"file never opened"<<endl;
		return(1); 
	}	
	
	cout<<"file is open"<<endl;  
	
	//boolean array indicating if this target value was found
	bool targetfound[20001] = {}; 
	for(int i = 0; i < 20001; i++){
		assert(targetfound[i] == false); 
	}
	
	//string for each line
	string li;
	//unordered set of observed numbers
	unordered_set<long long int> observed_numbers; 

	while(getline(fi,li)){
		
		//gets the number
		long long int x = getnum(li); 
		
		//add this number as observed
		observed_numbers.insert(x); 
		
	}
	
	fi.close(); 
	cout<<"closed file"<<endl; 
	
	/**********************
	* Finding pairs
	**********************/

	//loop over target values
	for(long long int t = -10000; t <= 10000; t++){
		
		if(t % 1000 == 0){
			cout<<"t: "<<t<<endl;
		}
				
		for(auto x = observed_numbers.begin(); 
			x != observed_numbers.end(); 
			x++){
			
			if(2*(*x) == t){
				//none with the same number twice
				continue; 
			}
			
			if(observed_numbers.find(t-(*x)) != observed_numbers.end()){
				//we have a pair
				targetfound[(t+10000)] = true; 
				break;
			}
		}
	} 
	
	/**********************
	* Counting hits
	**********************/
	
	int targethitcount = 0; 
	for(int i = 0; i < 20001; i++){
		if(targetfound[i]){
			targethitcount++; 
		}
	}
	
	cout<<"target hits: "<<targethitcount<<endl; 
	return(0); 
}


