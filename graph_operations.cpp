#include <iostream>
#include <stdio.h> 
#include <limits.h> 
#include<chrono>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include "header.h"
#define MAX 1000
using namespace std;
using namespace std::chrono;

int cycle[MAX]; //Array to store the cycle in Graph
int x;          //To store no of components in a cycle
void addEdge(int v, int w);
void printmat();
void dfscy(int a,int b,int *parent,int *color);	//DFS function for cycles in a graph
void dfscc(int v, bool visited[]);  		//DFS for connected components in a graph
void connected_components();        		//Function to find connected componets
void printcycle();				//Function to print cycle
int minDis(int dist[], bool spt[]);		//To find minimum distance
void dijkstra(int g[MAX][MAX],int src);				//dijkstra's Algorithm
int printSol(int dist[], int i, int sp[],int src);	//Used to print solution
void printPath(int sp[], int j);		//function to print shortest path from source		

void printmat()
{
	for(int i=0;i<V;i++)
	{
		for(int j=0;j<V;j++)
			cout<<adj[i][j]<<" ";
		cout<<"\n";
	}
}

void dfscy(int a,int b,int *parent,int *color)
{
	//int z=1;
    if (color[a] == 2)      //Completely visited vertex
        return;

    if (color[a] == 1)      //cycle deteced, therefore backtrack
    {
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

    for (int v=0;v<V; v++)
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
    bool* visited = new bool[V];        // Mark all the vertices as not visited
    for (int v = 0; v < V; v++)
        visited[v] = false;

    for (int v = 0; v < V; v++) 
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
    for(int i=0;i<V;i++)
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

	for (int v = 0; v < V; v++) 
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
	 
	printf("Vertex\t Distance From Src \tPath From Src"); 
	for (int i = 0; i < V; i++) 
	{   
	    if(dist[i]<999){
		printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src); 
		printPath(sp, i); 
	    }
	}
} 

//Dijkstra function
void dijkstra(int g[MAX][MAX], int src) 
{ 
 	//dist[i] will hold the shortest distance from source  
	int dist[V];

	//spt[i] is used to check whether vertices is included in shortest path from src to dest. or not... 
	bool spt[V]; 

	//sp[i] used to store shortest path
	int sp[V]; 

	//initialize all the distance as infinite and spt[i]=false
	for (int i = 0; i < V; i++) 
	{ 
		sp[src] = -1; 
		dist[i] = 999; 
		spt[i] = false; 
	} 

	//initialize src to src distance as 0
	dist[src] = 0; 

	//find shortest path for all the vertices
	for (int count = 0; count < V - 1; count++) 
	{ 
		int u = minDis(dist, spt); 

		//mark pickes vertex as true
		spt[u] = true; 

	 	//update distance value of pickedup vertex
		for (int v = 0; v < V; v++) 
			//update dist[i] only if it is not in spt[]
			//and only if the distance between src to v through u is less than current value of dist[v]
			if (!spt[v] && g[u][v] && dist[u] + g[u][v] < dist[v]) 
			{ 
				sp[v] = u; 
				dist[v] = dist[u] + g[u][v]; 
			} 
	} 
	printSol(dist, V, sp,src); 
}



// Driver code
int main()
{
	auto start = high_resolution_clock::now(); //Start time of execution
	int src;
	int a=create();
	cout<<"\nEnter the source for minimum distance:";
    	cin>>src;
    	if(a==1)
    	{
    		cout<<"MAtrix:\n";
    		printmat();
    		int parent[V],col_id[V];
    		for(int i=0;i<V;i++)        //To initialize parent and colour id arrays
    		{
 	  		parent[i]=0;
 	  		col_id[i]=0;
 	  		cycle[i]=-1;
    		}
    		connected_components();
    		dfscy(0,0,parent,col_id);
    		printcycle();
   		dijkstra(adj,src);
   	}
   	auto stop = high_resolution_clock::now();	//Time when execution finished
   	auto duration = duration_cast<microseconds>(stop - start); //To subtract start time-finish time to get time taken by process
  
    	cout << "\nTime taken by function: "<< duration.count() << " microseconds" << endl; 
      
    	return 0;
}
