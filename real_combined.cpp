#include <iostream>
#include <stdio.h> 
#include <limits.h> 
#include<chrono>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include<bits/stdc++.h>
#include<string>
#include<vector> //Vector, iterator, map have been used for file read, data storing, etc. Graph operations have  
#include<map>		//been done by algorithms and dfs only
#include<iterator>
//#include "abc.h"
#define MAX 100
#define MAX1 10000
using namespace std;
using namespace std::chrono;

struct user			//structure to store user information
{
 	int id;
 	int rat;
 	string date;
};

multimap<int,int>a1;		//to map movie id and user id
multimap<int,int>a2;		//to map unique user id with values starting from 0 
int arr[MAX][MAX1]; 		//To store movie id vs user id
int adj[MAX1][MAX1];		//To store user id vs user id for graph operations
struct user mov_id[MAX][MAX];	//Structure of arrays containg user information
int cycle[MAX1]; 		//Array to store the cycle in Graph
int x,c1,c2,cnt;         	 //To store no of components in a cycle
void initial();			//to initialize all arrays
void file_operations();		//Function to read values from file into structure
void multimap1();		//Function to retrieve key and value pair and store in array for adding edges
void add_edge1();		//Function to add edge if one movie common
void add_edge2();		//Function to add edge if two movies common
void dfscy(int a,int b,int *parent,int *color);	//DFS function for cycles in a graph
void dfscc(int v, bool visited[]);  		//DFS for connected components in a graph
void connected_components();        		//Function to find connected componets
void printcycle();				//Function to print cycle
int minDis(int dist[], bool spt[]);		//To find minimum distance
void dijkstra(int g[MAX1][MAX1],int src);				//dijkstra's Algorithm
int printSol(int dist[], int i, int sp[],int src);	//Used to print solution
void printPath(int sp[], int j);		//function to print shortest path from source		

void initial()
{
	for(int i=0;i<MAX;i++)			//initialize structure
 	{
 		for(int j=0;j<MAX;j++)
 		{
 			mov_id[i][j].id=0;
 			mov_id[i][j].rat=0;
 			mov_id[i][j].date="";
 		}
 	}
 	for(int i=0;i<MAX;i++)			//initialize mov id to user id array
 		for(int j=0;j<MAX1;j++)
 			arr[i][j]=0;
 			
 	for(int i=0;i<MAX1;i++)			//initialize user id to user id array
 		for(int j=0;j<MAX1;j++)
			adj[i][j]=0;
		
}

void file_operations()
{
	fstream fs;
	fs.open("ratings_data_1.txt", ios:: in);
	if(fs.is_open())
	{
		string x;
 		int i,x1;
 		c1=0;
 		c2=0;
 		cnt=0;
 		while(getline(fs,x))
 		{
 			cnt++;		//counts number of records read
 			if(x.back()==':')
 			{
 				x.erase(x.end()-1 );
 				x1=stoi(x);
 				c2++;
 				i=0;
 			}
 			else
 			{
 				if(i<=MAX)
 				{
 				string word;
 				string row[3];
 				std:: istringstream s(x); 	//separates three records from comma
  				getline(s,row[0],',');
  				getline(s,row[1],',');
  				getline(s,row[2]);
  				int a=stoi(row[0]);
  				int b=stoi(row[1]);
  				mov_id[x1][i].id=a;
  				mov_id[x1][i].rat=b;
  				mov_id[x1][i].date=row[2];
  				i++;
  				a1.insert(pair <int,int>(x1,a));	//mapping of mov id and user id
  				a2.insert(pair <int,int>(a,c1));	//mapping of user id from zero to get unique users
  				c1++;					//counts number of users
  				if(c2==MAX)
 					break;
  				}
  			
  			}
  		}
  	fs.close();
	}
  	else
  		cout<<"\nError in file opeing!";
}

void multimap1()		//replaces user id with smaller user id starting from 0 and stores in array 
{
	multimap <int, int> :: iterator itr2;		//Resulting array arr is movie id vs small user id
	for (itr2 = a1.begin(); itr2!= a1.end(); ++itr2)
    	{
    	
       		int g1=itr2->first;
       		int g2=itr2->second;
        	multimap <int, int> :: iterator itr=a2.find(g2);
            	int g3=itr->second;
            	arr[g1][g3]=1;
    	}
}

void printmat()
{
	for(int i=0;i<c1;i++)
	{
		for(int j=0;j<c1;j++)
			cout<<adj[i][j]<<" ";
		cout<<"\n";
	}
}

void add_edge1()			// add edge if one movie in common
{
	for(int i=1;i<=c2;i++)
		for(int j=0;j<c1;j++)
			for(int k=j+1;k<c1;k++)
				if(arr[i][j]==1&& arr[i][k]==1)
				{
					adj[j][k]=1;
					adj[k][j]=1;
				}
}
void add_edge2()
{
	//add edge if two movies in common
	for(int i=0;i<c1;i++)
		for(int j=i+1;j<c1;j++)
		{
			int count=0;
			for(int k=1;k<=c2;k++)
			{
				if(arr[k][i]==1&& arr[k][j]==1)
				{
					count++;
					if(count==2)
						break;
				}
			}
			if(count>=2)
			{
				adj[i][j]=1;
				adj[j][i]=1;
			}
		}
}
void dfscy(int a,int b,int *parent,int *color)
{
	//int z=1;
    if (color[a] == 2)      //Completely visited vertex
        return;

    if (color[a] == 1)      //cycle deteced, therefore backtrack
    {
    	//cout<<"\nin cycle";
        int cur = b;
        x=0;
        cycle[x]=cur;
        while (cur != a)    //Backtrack and store the cycle nodes  
        {
        	x++;
            cur = parent[cur];
            cycle[x]=cur;
        }
       // z=0;
        return;
    }
    parent[a] = b;
    color[a] = 1;           //partially visited vertex

    for (int v=0;v<c1; v++)
    {
        if (v==parent[a]|| adj[a][v]==0)	//To check if it is previously visited
            continue;
        dfscy(v, a, parent,color);
    }
    color[a] = 2;           //completely visited
}

void connected_components()
{
	cout<<"\nConnected Components:";
    bool* visited = new bool[c1];        // Mark all the vertices as not visited
    for (int v = 0; v < c1; v++)
        visited[v] = false;

    for (int v = 0; v < c1; v++) 
    {
        if (visited[v] == false) 
        {
            //print all vertices reachable from v
            dfscc(v, visited);
            cout << "\n";
        }
    }
    delete[] visited;
}
 
void dfscc(int v, bool visited[])
{
        // Mark the current node as visited and print it
    visited[v] = true;
    cout << v << " ";
    for(int i=0;i<c1;i++)
        //recursively run dfs to find other connected components not visited
     	if (adj[v][i] == 1 && (!visited[i])) 
            dfscc(i, visited); 
}

void printcycle()
{
	if(cycle[0]!=-1)       //To check if cycle present or not
	{
		cout<<"\nCycle present!";
		cout<<"\nThe cycle is:";
		for(int i=x;i>=0;i--)
			cout<<cycle[i]<<" ";
	}
	else
		cout<<"\nCycle not present!";
	cout<<"\n";
}

int minDis(int dist[], bool spt[]) 
{ 
	//initialize min values
	int min =999, min_index; 

	for (int v = 0; v < c1; v++) 
		if (spt[v] == false && dist[v] <= min) 
			min = dist[v], min_index = v; 

	return min_index; 
} 

//function to print shortest path from source
void printPath(int sp[], int j) 
{ 
	if (sp[j] == - 1) 
		return; 

	printPath(sp, sp[j]); 
	printf("%d ", j); 
} 

//used to print solution
int printSol(int dist[],int i, int sp[],int src) 
{ 
	 //int cnt=0;
	printf("Vertex\t Distance From Src \tPath From Src"); 
	for (int i = 0; i < c1; i++) 
	{   
	    if(dist[i]<999){
		printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src); 
		printPath(sp, i); 
	    }
	}
} 

//Dijkstra function
void dijkstra(int g[MAX1][MAX1], int src) 
{ 
 	//dist[i] will hold the shortest distance from source  
	int dist[c1];

	//spt[i] is used to check whether vertices is included in shortest path from src to dest. or not... 
	bool spt[c1]; 

	//sp[i] used to store shortest path
	int sp[c1]; 

	//initialize all the distance as infinite and spt[i]=false
	for (int i = 0; i < c1; i++) 
	{ 
		sp[src] = -1; 
		dist[i] = 999; 
		spt[i] = false; 
	} 

	//initialize src to src distance as 0
	dist[src] = 0; 

	//find shortest path for all the vertices
	for (int count = 0; count < c1 - 1; count++) 
	{ 
		int u = minDis(dist, spt); 

		//mark pickes vertex as true
		spt[u] = true; 

	 	//update distance value of pickedup vertex
		for (int v = 0; v < c1; v++) 
			//update dist[i] only if it is not in spt[]
			//and only if the distance between src to v through u is less than current value of dist[v]
			if (!spt[v] && g[u][v] && dist[u] + g[u][v] < dist[v]) 
			{ 
				sp[v] = u; 
				dist[v] = dist[u] + g[u][v]; 
			} 
	} 
	printSol(dist, c1, sp,src); 
}



// Driver code
int main()
{
	initial();
	file_operations();
	multimap1();
	cout<<"MAP";
	add_edge1();
	//add_edge2();
	int src;
	cout<<"\nEnter the source for minimum distance:";
    	cin>>src;
    	int parent[MAX1],col_id[MAX1];
    	for(int i=0;i<c1;i++)        //To initialize parent and colour id arrays
    	{
 	  	parent[i]=0;
 	  	col_id[i]=0;
 	  	cycle[i]=-1;
    	}
    	connected_components();
    	dfscy(1,1,parent,col_id);
    	printcycle();
   	dijkstra(adj,src);
   	cout<<"\nNumber of records read:"<<cnt;	
    	return 0;
}

