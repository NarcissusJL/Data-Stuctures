// Implementation of the Graph class
// Zuoyu Wang & Huiyang Li

#include "graph.h"
#include "pq.h"
#include <climits>
#include <fstream>
#include <iostream>

using namespace std;

Graph::Graph(string filename)
{
    ifstream is(filename);
    int weight;
    is >> nvertices;
    vertices = new Vertex *[nvertices];
    for (int i = 0; i < nvertices; i++)
    {
        vertices[i] = new Vertex(i);
        for (int j = 0; j < nvertices; j++)
        {
            is >> weight;
            if (weight > 0)
            {
                vertices[i]->adjlist.append(new Vertex(j, weight));
            }
        }
    }
    is.close();
}

Graph::Graph(const Graph &g)
{
    copy(g);
}

Graph::~Graph()
{
    destroy();
}

Graph &Graph::operator=(const Graph &g)
{
    if (this != &g)
    {
        destroy();
        copy(g);
    }
}

void Graph::dfs()
{
    bool visited[nvertices] = {};
    // assume the DFS always started from the 0 vertex in the graph
    recurdfs(0, visited);
    cout << endl;
}

void Graph::recurdfs(int root, bool visited[])
{
    cout << root << " ";
    visited[root] = true;
    List<Vertex> adjlist = vertices[root]->adjlist;
    for (int i = 0; i < adjlist.length(); i++)
        if (!visited[adjlist[i]->v])
            recurdfs(adjlist[i]->v, visited);
}

bool Graph::cycle()
{
    bool visited[nvertices] = {};
    // detect cycle in different DFS trees
    for (int i = 0; i < nvertices; i++)
    {
        // stop recursion for the ith vertex if it has been visited
        if (!visited[i])
        {
            if (recurcycle(i, visited, -1))
                return true;
        }
    }

    return false;
}

bool Graph::recurcycle(int v, bool visited[], int parent)
{
    // mark current vertex v as visited
    visited[v] = true;
    List<Vertex> adjlist = vertices[v]->adjlist;
    for (int i = 0; i < adjlist.length(); i++)
    {
        int u = adjlist[i]->v;
        if (!visited[u])
        {
            // recur for the vertex u if it is not yet visited
            if (recurcycle(u, visited, v))
                return true;
        }
        else if (u != parent)
        {
            // if vertex u has been visited but not the parent, there must be a cycle
            return true;
        }
    }

    return false;
}

void Graph::prim(int root)
{
    MinPriorityQueue<Vertex> pq;
    bool visited[nvertices] = {};

    // initialize the pq and the visited array
    visited[root] = true;
    List<Vertex> adjlist = vertices[root]->adjlist;
    for (int i = 0; i < adjlist.length(); i++)
    {
        Vertex *v = adjlist[i];
        v->source = root;
        pq.insert(v);
    }

    cout << "The mst generated by prim algorithm: " << endl;
    for (int i = 1; i < nvertices; i++)
    {
        Vertex *next = pq.extractMin();
        // filter edges that have both endpoints lying in the visited set
        while (visited[next->v])
            next = pq.extractMin();
        visited[next->v] = true;
        cout << next->source << "-" << next->v << ": " << next->weight << endl;
        adjlist = vertices[next->v]->adjlist;
        for (int j = 0; j < adjlist.length(); j++)
        {
            Vertex *v = adjlist[j];
            if (!visited[v->v])
            {
                // insert the adjacent vertex to pq only it is not visited yet
                v->source = next->v;
                pq.insert(v);
            }
        }
    }
}

void Graph::copy(const Graph &g)
{
    nvertices = g.nvertices;
    vertices = new Vertex *[nvertices];
    for (int i = 0; i < nvertices; i++)
        vertices[i] = new Vertex(*g.vertices[i]);
}

void Graph::destroy()
{
    if (vertices)
    {
        for (int i = 0; i < nvertices; i++)
            delete vertices[i];
        delete[] vertices;
        nvertices = 0;
    }
}
