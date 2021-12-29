/*
	This file does the median maintenance algorithm. 
	Procedure is as follows:
	
	1) Make two heaps, one for small elements, one for large elements
	2) Read an integer from the file
		a) if it is > min element from the large number heap, add it there
		b) otherwise, add it to the small number heap
	3) If one heap has 2 more elements, get the largest (or smallest) number 
		from the heap with more elements and add it to the other heap. 
	4) 
		a) If large number heap has more elements, return its smallest number
		b) Otherwise return the small heap's largest number 
	
	We keep track of these medians as we add numbers, and then return the 
	sum of all the medians mod 10000
	
	
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>

using namespace std; 

//gets the number from this line 
int getnum(string s){
	int num = 0;
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
	
	string filepath = "./Median.txt"; 
		
	//heaps, c++ std heaps have largest elements on top 
	//for the big heap we want the smallest element on top
	vector<int> smallheap; 
	vector<int> bigheap; 
	
	fstream fi;
	fi.open(filepath,ios::in); 
	
	if(!fi.is_open()){
		cout<<"file never opened"<<endl; 
		return(1);
	}
	
	cout<<"file opened"<<endl; 
	string li; 
	
	int mediantot = 0; 
	
	while(getline(fi,li)){
		
		int num = getnum(li); 
		// cout<<"num: "<<num<<endl; 
		
		//add this num to a heap 
		if(bigheap.size() == 0){
			//this should only enter the first two iterations
			smallheap.push_back(num); 
			push_heap(smallheap.begin(), smallheap.end()); 
		}
		else if(num > (-1)*bigheap.front()){
			bigheap.push_back((-1)*num); 
			push_heap(bigheap.begin(), bigheap.end()); 
		}else{
			smallheap.push_back(num);
			push_heap(smallheap.begin(), smallheap.end()); 
		} 
		
		int wow = (bigheap.size()) - (smallheap.size());
		
		//move elements to maintain balanced heaps 
		if(wow > 1){
			
			//moves smallest element from big heap to the small heap
			int temp = bigheap.front()*(-1);
			pop_heap(bigheap.begin(), bigheap.end()); 
			bigheap.pop_back();
			
			smallheap.push_back(temp); 
			push_heap(smallheap.begin(), smallheap.end()); 			
		
		}else if(((-1)*wow) > 1){
			
			//moves largest element from small heap to the big heap
			int temp = smallheap.front()*(-1);
			pop_heap(smallheap.begin(), smallheap.end()); 
			smallheap.pop_back();
			
			bigheap.push_back(temp); 
			push_heap(bigheap.begin(), bigheap.end()); 
			
		}
		
		//getting the median 
		if(bigheap.size() > smallheap.size()){
			// cout<<"median: "<<(bigheap.front()*(-1))<<endl;
			mediantot += bigheap.front()*(-1);  
		}else{
			// cout<<"median: "<<(smallheap.front())<<endl; 
			mediantot += smallheap.front();  
		}
		mediantot %= 10000; 
		
	}
	
	//closing up 
	fi.close();
	cout<<"closed file"<<endl; 
	
	cout<<"median total mod 10000: "<<mediantot<<endl; 
	
	
	return(0); 
	
}

