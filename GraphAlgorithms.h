//
// Created by muzamil on 5/17/20.
//

#ifndef JELLY_FISH_PROJECT_GRAPHALGORITHMS_H
#define JELLY_FISH_PROJECT_GRAPHALGORITHMS_H

#include <iostream>
#include <vector>
using namespace std;
#define INFINITY 400

// return node all i adj
vector<int>& getIthNeighbours(const vector<vector<int>>& adj, int i)
{
    vector<int>* adjacentNodes = new vector<int>();
    for (int k = 0; k < adj.size(); k++)
    {
        if (adj[i][k] == 1)
        {
            adjacentNodes->push_back(k);
        }
    }
    return *adjacentNodes;
}
// populate adjN with subset of neighbours toward destinations
void getIJPreferredNeighbours(vector<vector<int>>& adj,
                              int n,
                              vector<vector<int>>& minDistance,
                              vector<vector<vector<int>>>& adjN)
{
    // consider all sources i
    for (int i = 0; i < n; i++)
    {
        // consider all destinations j
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                vector<int> iNeighbours = getIthNeighbours(adj, i);
                // find subset of iNieighbours that are part of shortest paths
                // to j
                for (int k = 0; k < iNeighbours.size(); k++)
                {
                    int neighbour = iNeighbours[k];
                    int a = minDistance[i][neighbour];
                    int b = minDistance[neighbour][j];
                    int c = minDistance[i][j];
                    if (a + b == c)
                    {
                        adjN[i][j].push_back(neighbour);
                    }
                }
            }
        }
    }
}
// function to get ECMP-SP traffic in recursive manner for a path from k to j
void increaseCapacity(int k,
                      int j,
                      vector<vector<vector<int>>>& adjN,
                      vector<vector<double>>& trafficload,
                      double trafficKJ)
{
    if( k != j && trafficKJ != 0.0)
    {
        double load = trafficKJ / (adjN[k][j].size() * 1.0);
        for (int t = 0; t < adjN[k][j].size(); t++)
        {
            int neighbour = adjN[k][j][t];
            trafficload[k][neighbour] += load;
            trafficload[neighbour][k] += load;
            // recurse till you reach destination j
            double newTraffic = load;
            increaseCapacity(neighbour, j, adjN, trafficload, newTraffic);
        }
    }
}
// populate traffic load between all to all ECMP
void getECMPTraffic(vector<vector<double>>& trafficLoad,
                    int n,
                    vector<vector<vector<int>>>& adjN)
{
    for (int i = 0; i < n - 1; i++)
    {
        // j is destination
        for (int j = i + 1; j < n; j++)
        {
            // case there is a link that leads to j
            // set traffic to be 1 if i is not j
            double traffic;
            if (i != j)
                traffic = 1;
            else
                traffic = 0;
            if (adjN[i][j].size() > 0)
            {
                increaseCapacity(i, j, adjN, trafficLoad, traffic);
            }
        }
    }
}
// algorithm for shortest distance between source node and all reachable nodes
// in G
void dijkstra(vector<vector<int>>& adj,
              int n,
              int source,
              vector<int>& distance)
{
    vector<vector<int>> dist(n, vector<int>(n));
    vector<int> pred(n), visited(n);
    int count, minDistance, nextNode, i, j;
    // initiate the distance matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (adj[i][j] == 0)
                dist[i][j] = INFINITY;
            else
                dist[i][j] = adj[i][j];
    for (i = 0; i < n; i++)
    {
        distance[i] = dist[source][i];
        pred[i] = source;
        visited[i] = 0;
    }
    distance[source] = 0;
    visited[source] = 1;
    count = 1;
    while (count < n - 1)
    {
        minDistance = INFINITY;
        for (i = 0; i < n; i++)
            if (distance[i] < minDistance && !visited[i])
            {
                minDistance = distance[i];
                nextNode = i;
            }
        visited[nextNode] = 1;
        for (i = 0; i < n; i++)
            if (!visited[i])
                if (minDistance + dist[nextNode][i] < distance[i])
                {
                    distance[i] = minDistance + dist[nextNode][i];
                    pred[i] = nextNode;
                }
        count++;
    }
    /*for (i = 0; i < n; i++)
    {
        if (i != source)
        {
            cout << "\nD(" << source << "," << i << "): " << distance[i];
        }
    }*/
}
#endif  // JELLY_FISH_PROJECT_GRAPHALGORITHMS_H
