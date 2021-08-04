#include <iostream>
#include <cstdlib>
#include "header.h"
#define MAX 1000
using namespace std;
int V;
int adj[MAX][MAX];
void complete(); //To simulate a complete graph 
void empty();	//To simlulate an empty graph
void ncycle();	//To simlute a N cycle Graph
int create();	//To ask user the type of graph
void complete()
{
    for (int i=0;i<V;i++)  
        for (int j=0;j<V;j++) 
        { 
            if(i!=j)
            	adj[i][j]=1;
        } 
}
void empty()
{
    for (int i=0;i<V;i++)  
        for (int j= 0;j<V;j++) 
                adj[i][j]=0;
}
void ncycle()
{
    for (int i=0;i<V;i++)  
        for (int j=0;j<V;j++) 
        { 
            if(i!=j && (abs(i-j)==1|| abs(i-j)==(V-1) ))
                adj[i][j]=1;
        } 
}

int create()
{
	int n2;
	cout<<"\nEnter number of vertices:";
	cin>>V;
	cout<<"Graph types:\n1.Complete Graph\n2.N-cycle Graph\n3.Empty Graph\nEnter type of graph:";
	cin>>n2;
	switch(n2)
	{
	case 1: complete();
		break;
	case 2: ncycle();
		break;
    	case 3: empty();
    		break;
	default:cout<<"\nInvalid choice!";
	}
	return 1 ;
}
	
