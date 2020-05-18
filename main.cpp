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
                maxTrafficB = max(maxTrafficB, trafficLoadB[x][y]);
            }
        }
        cout << "\n*********************************************";
        cout << "\ngraph: " << t + 1 << " with " << nodesSeq[t] << " and max degree " << degreesSeq[t] ;
        cout << "\nmaxTraffic  for random Jellyfish      : " << 1/maxTrafficR ;
        cout << "\nmaxTraffic for incrementing Jellyfish: " << 1/maxTrafficI ;
        cout << "\nmaxTraffic for bipartite Jellyfish   : " << 1/maxTrafficB ;

        cout << "\n";

        // Random permutation of traffic matrix. We randomly select p nodes to send traffic to for each node
        vector<vector<double>> trafficMatrix(n, vector<double>(n,0.0));
        // set up random device (default)
        random_device r;
        default_random_engine e1(r());
        uniform_int_distribution<int> uniform_dist(0, n-1);
        int  p = n/4;

        for(int u = 0; u < n ; u++)
        {
            for (int k = 0; k < p; k++)
            {
                int v = uniform_dist(e1);
                if (v == u)
                {
                    k--;
                }
                else
                {
                    trafficMatrix[u][v] = 1.0;
                }
            }
        }

        vector<vector<double>> trafficLoadRPerm(n, vector<double>(n));
        vector<vector<double>> trafficLoadIPerm(n, vector<double>(n));
        vector<vector<double>> trafficLoadBPerm(n, vector<double>(n));

        getECMPTraffic(trafficLoadRPerm , n, adjRN,trafficMatrix);
        getECMPTraffic(trafficLoadIPerm , n, adjIN,trafficMatrix);
        getECMPTraffic(trafficLoadBPerm , n, adjBN, trafficMatrix);

        double maxTrafficRPerm = -1.0;
        double maxTrafficIPerm = -1.0;
        double maxTrafficBPerm = -1.0;
        for(int x=0; x < n; x++)
        {
            for(int y = 0; y < n ; y++)
            {
                maxTrafficRPerm = max(maxTrafficRPerm, trafficLoadRPerm[x][y]);
                maxTrafficIPerm = max(maxTrafficIPerm, trafficLoadIPerm[x][y]);
                maxTrafficBPerm = max(maxTrafficBPerm, trafficLoadBPerm[x][y]);
            }
        }

        cout << "\n";
        cout << "\n permutation Traffic:";
        cout << "\nmax Permutation Traffic for random Jellyfish      : " << 1/maxTrafficRPerm ;
        cout << "\nmax Permutation Traffic for incrementing Jellyfish: " << 1/maxTrafficIPerm ;
        cout << "\nmax Permutation Traffic for bipartite Jellyfish   : " << 1/maxTrafficBPerm ;
    }
    return 0;
}
