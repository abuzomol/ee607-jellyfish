#include <iostream>
#include <random>
#include <vector>
#include "topologies.h"
#include "GraphAlgorithms.h"

using namespace std;

int main()
{
    // list of nodesSeq and degreesSeq
    vector<int> nodesSeq = {64, 100, 200, 100, 200, 300};
    vector<int> degreesSeq = {8, 8, 8, 12, 12, 12};

    // total number of runs
    int runs = nodesSeq.size();

    // generate a graph with given number of nodes and degrees
    for (int t = 0; t < runs; t++)
    {
        int n = nodesSeq[t];
        int maxDegree = degreesSeq[t];
        // create list of non neighbours
        vector<vector<int>> nonAdjList(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j) nonAdjList[i].push_back(j);
            }
        }
        // create a list of degrees initialized with maxDegree
        vector<int> degrees(n, maxDegree);
        // create empty adjacency matrix for random Jellyfish
        vector<vector<int>> adjR(n, vector<int>(n, 0));
        // create empty adjacency matrix for incrementing Jellyfish
        vector<vector<int>> adjI(n, vector<int>(n, 0));
        // create empty adjacency matrix for bipartite Jellyfish
        vector<vector<int>> adjB(n, vector<int>(n, 0));

        randomJellyfish(adjR, n, maxDegree, degrees, nonAdjList);
        incrementingJellyfish(adjI, n, maxDegree, degrees, nonAdjList);
        bipartiteJellyfish(adjB, n, maxDegree, degrees, nonAdjList);

        //matrices to store shortest distance between pair of nodes
        vector<vector<int>> minDistanceR(n, vector<int>(n, INFINITY));
        vector<vector<int>> minDistanceI(n, vector<int>(n,INFINITY));
        vector<vector<int>> minDistanceB(n, vector<int>(n,INFINITY));
        for(int i = 0; i  < n; i++)
        {
            dijkstra(adjR, n, i, minDistanceR[i]);
            dijkstra(adjI, n, i, minDistanceI[i]);
            dijkstra(adjB, n, i, minDistanceB[i]);
        }
        //matrix to store possible neighbours towards a specific destination
        vector<vector<vector<int>>> adjRN(n, vector<vector<int>>(n));
        vector<vector<vector<int>>> adjIN(n, vector<vector<int>>(n));
        vector<vector<vector<int>>> adjBN(n, vector<vector<int>>(n));

        getIJPreferredNeighbours(adjR, n, minDistanceR, adjRN);
        getIJPreferredNeighbours(adjI, n, minDistanceI, adjIN);
        getIJPreferredNeighbours(adjB, n, minDistanceB, adjBN);

        vector<vector<double>> trafficLoadR(n, vector<double>(n));
        vector<vector<double>> trafficLoadI(n, vector<double>(n));
        vector<vector<double>> trafficLoadB(n, vector<double>(n));

        getECMPTraffic(trafficLoadR , n, adjRN);
        getECMPTraffic(trafficLoadI , n, adjIN);
        getECMPTraffic(trafficLoadB , n, adjBN);

        double maxTrafficR = -1;
        double maxTrafficI = -1;
        double maxTrafficB = -1;
        for(int x=0; x < n; x++)
        {
            for(int y = 0; y < n ; y++)
            {
                maxTrafficR = max(maxTrafficR, trafficLoadR[x][y]);
                maxTrafficI = max(maxTrafficI, trafficLoadI[x][y]);
                maxTrafficB = max(maxTrafficI, trafficLoadB[x][y]);
            }
        }
        cout << "\n*********************************************";
        cout << "\ngraph: " << t + 1 << " with " << nodesSeq[t] << " and max degree " << degreesSeq[t] ;
        cout << "\nmaxTraffic for random Jellyfish      : " << maxTrafficR ;
        cout << "\nmaxTraffic for incrementing Jellyfish: " << maxTrafficI ;
        cout << "\nmaxTraffic for bipartite Jellyfish   : " << maxTrafficB ;
    }
    return 0;
}
