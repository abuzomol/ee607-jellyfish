//
// Created by muzamil on 5/16/20.
//

#ifndef JELLY_FISH_PROJECT_TOPOLOGIES_H
#define JELLY_FISH_PROJECT_TOPOLOGIES_H

#include <iostream>
#include <vector>

using namespace std;

// returns the degree of v along with nodes non-adjacent
auto getNonAdj(vector<vector<int>>& adj, int u)
{
    int count = 0;
    auto nonAdj = new vector<int>();
    for (auto i = 0; i < adj.size(); i++)
    {
        if (adj[u][i] > 0)
            count++;
        else
        {
            if (i != u) nonAdj->push_back(i);
        }
    }
    return make_pair(count, nonAdj);
}

// Jellyfish: create random topology for each graph
void randomJellyfish(vector<vector<int>>& adj,
                     int n,
                     int maxDegree,
                     vector<int>& degrees,
                     vector<vector<int>>& nonAdjList)
{
    // set up random device (default)
    random_device r;
    default_random_engine e1(r());
    // go through every node one by one
    for (int u = 0; u < n; u++)
    {
        // number of trials in case not too many vertices of degree less than d
        // are found
        int trials = 0;

        // while degree of u is still no maxDegree
        while (degrees[u] > 0 && trials < 100000)
        {
            int b = nonAdjList[u].size() - 1;
            // create uniform random distribution over non adjacent neighbours
            uniform_int_distribution<int> uniform_dist(0, b);
            int r = uniform_dist(e1);
            int v = nonAdjList[u][r];
            // check if a degree of random selected node is not maxDegree yet
            if (degrees[v] > 0)
            {
                adj[u][v] = 1;
                adj[v][u] = 1;
                degrees[u]--;
                degrees[v]--;

                // update nonadjacent lists
                nonAdjList[u].erase(nonAdjList[u].begin() + r);
                // find index of u in nonAdj[v]
                auto uPos = find(nonAdjList[v].begin(), nonAdjList[v].end(), u);
                nonAdjList[v].erase(uPos);
            }
            else  // case the node v is maxDegree
            {
                // check if u has degree maxDegree - 2
                if (degrees[u] == 2)
                {
                    int w = 0;
                    while (w < n && (adj[v][w] == 0 || adj[u][w] == 1)) w++;
                    adj[v][w] = 0;
                    adj[w][v] = 0;
                    adj[u][v] = 1;
                    adj[v][u] = 1;
                    adj[u][w] = 1;
                    adj[w][u] = 1;
                    degrees[u] -= 2;
                    // update non adjacent lists of u, v, and w
                    nonAdjList[u].erase(nonAdjList[u].begin() + r);
                    auto uPos =
                        find(nonAdjList[v].begin(), nonAdjList[v].end(), u);
                    nonAdjList[v].erase(uPos);
                    auto wPos =
                        find(nonAdjList[u].begin(), nonAdjList[u].end(), w);
                    nonAdjList[u].erase(wPos);
                    nonAdjList[v].push_back(w);
                }
            }
            trials++;
        }
    }
    // code to fix cases where some nodes with degree less than maxDegree
    vector<int> lowerDegrees;
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (degrees[i] > 0)
        {
            lowerDegrees.push_back(i);
            count++;
        }
    }
    //cout << "\ngraph count: " << count;
    for (int j = 0; j < count; j++)
    {
        int u = lowerDegrees[j];
        // cout << "\n degree: " << degrees[u] << " " <<maxDegree;
        for (int k = j + 1; k < lowerDegrees.size(); k++)
        {
            int v = lowerDegrees[k];
            if (adj[u][v] == 0)
            {
                adj[u][v] = 1;
                adj[v][u] = 1;
                degrees[u]--;
                degrees[v]--;

                // update nonadjacent lists
                auto vPos = find(nonAdjList[u].begin(), nonAdjList[u].end(), k);
                nonAdjList[u].erase(vPos);
                // find index of u in nonAdj[v]
                auto uPos = find(nonAdjList[v].begin(), nonAdjList[v].end(), u);
                nonAdjList[v].erase(uPos);
                break;
            }
        }
    }

    count = 0;
    for (int i = 0; i < adj.size(); i++)
    {
        if (degrees[i] > 0)
        {
            count++;
        }
    }
    //cout << "\ngraph count: " << count;
}

void incrementingJellyfish(vector<vector<int>>& adj,
                           int n,
                           int maxDegree,
                           vector<int>& degrees,
                           vector<vector<int>>& nonAdjList)
{
    // set up random device (default)
    random_device r;
    default_random_engine e1(r());
    // empty adjacency list
    vector<vector<int>> adjList(n);

    // create a complete graph of maxDegree+1 nodes
    for (int i = 0; i < maxDegree + 1; i++)
    {
        for (int j = 0; j < maxDegree + 1; j++)
        {
            if (i != j)
            {
                adj[i][j] = 1;
                adjList[i].push_back(j);
            }
        }
    }
    // add more links to the complete graph above
    for (int u = maxDegree + 1; u < n; u++)
    {
        for (int j = 0; j < maxDegree; j += 2)
        {
            // select node w at random from 0 to u - 1
            int b1 = u - 1;
            uniform_int_distribution<int> uniform_dist1(0, b1);
            int v = uniform_dist1(e1);
            // select node v at random neighbours of w
            int b2 = adjList[v].size() - 1;
            uniform_int_distribution<int> uniform_dist2(0, b2);
            int r = uniform_dist2(e1);
            int w = adjList[v][r];
            // unlink v-w
            adj[v][w] = 0;
            adj[w][v] = 0;
            // link u-v
            adj[u][v] = 1;
            adj[v][u] = 1;
            // link u-w
            adj[u][w] = 1;
            adj[w][u] = 1;

            adjList[v].erase(adjList[v].begin() + r);
            auto vPos = find(adjList[w].begin(), adjList[w].end(), v);
            adjList[w].erase(vPos);
            // update adjacency lists
            adjList[v].push_back(u);
            adjList[u].push_back(v);
            adjList[u].push_back(w);
            adjList[w].push_back(u);
        }
    }
}

void bipartiteJellyfish(vector<vector<int>>& adj,
                        int n,
                        int maxDegree,
                        vector<int>& degrees,
                        vector<vector<int>>& nonAdjList)
{
    // set up random device (default)
    random_device r;
    default_random_engine e1(r());
    vector<vector<int>> adjList(n);
    // create bipartite graph of first maxDegree nodes and last maxDegree nodes
    for (int i = 0; i < maxDegree; i++)
    {
        for (int j = 0; j < maxDegree; j++)
        {
            adj[i][n - j - 1] = 1;
            adj[n - j - 1][i] = 1;
            adjList[i].push_back(n - j - 1);
            adjList[n - j - 1].push_back(i);
        }
    }
    // add one more node from left and one more left from right
    for (int u = maxDegree; u < n/2 ; u++)
    {
        // add link u and n-1-u
        adj[u][n - 1 - u] = 1;
        adj[n - 1 - u][u] = 1;
        adjList[u].push_back(n - 1 - u);
        adjList[n - u - 1].push_back(u);

        // unlink a random edge in the bipartite graph
        int b1 = u - 1;
        uniform_int_distribution<int> uniform_dist1(0, b1);

        //create a list of vNodes from left, wNodes from right
        vector<int> vNodes(maxDegree - 1);
        vector<int> wNodes(maxDegree - 1);

        for (int i = 0; i < maxDegree - 1; i++)
        {
            // select v at random from left
            vector<int>::iterator it;
            int v;
            //choose a distinct v
            do
            {
                v = uniform_dist1(e1);
                it = find(vNodes.begin(), vNodes.end(), v);
            } while (it != vNodes.end());
            vNodes[i] = v;
            // select node w at random from neighbours of v
            int b2 = adjList[v].size() - 1;
            uniform_int_distribution<int> uniform_dist2(0, b2);
            vector<int>::iterator itt;
            int w, r;
            //choose a distinct w
            do
            {
                r = uniform_dist2(e1);
                w = adjList[v][r];
                itt = find(wNodes.begin(), wNodes.end(), w);
            } while (itt != wNodes.end());
            wNodes[i] = w;

            // unlink v-w
            adj[v][w] = 0;
            adj[w][v] = 0;
            // link u-w
            adj[u][w] = 1;
            adj[w][u] = 1;
            // link v and n-1-u
            adj[n-1-u][v] = 1;
            adj[v][n-1-u] = 1;

            adjList[v].erase(adjList[v].begin() + r);
            auto vPos = find(adjList[w].begin(), adjList[w].end(), v);
            adjList[w].erase(vPos);
            // update adjacency lists
            adjList[v].push_back(n-1-u);
            adjList[n-1-u].push_back(v);
            adjList[u].push_back(w);
            adjList[w].push_back(u);
        }
    }
}


#endif  // JELLY_FISH_PROJECT_TOPOLOGIES_H
