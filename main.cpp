#include <iostream>
#include "Graph.h"
#include "Graphs.h"
#include <cstdio>

using namespace std;

int main()
{
    char * filename = "/home/renatocjn/Dropbox/Programas/Lab2/graph_lab2/instances/teste.clq";
    FILE * fp = fopen(filename, "r");
    Graph * g = readDimacsGraph(fp, GT_GRAPH);

    srand(time(NULL));
    int r = rand()%g->nverts();

    Iterator * it = depth(g, r);

    cout << "\t*** ITERACAO INICIADA NO VERTICE " << r+1 << " ***" << endl;
    while(it->hasNext())
    {
        cout << it->next()+1 << endl;
    }

    cout << endl;

    return 0;
}

