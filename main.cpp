#include <iostream>
#include "Graph.h"
#include "Graphs.cpp"
#include <cstdio>

using namespace std;

int main()
{
    char * filename = "/home/renatocjn/Dropbox/Programas/Lab2/graph_lab2/instances/teste.clq";
    FILE * fp = fopen(filename, "r");
    Graph * g = readDimacsGraph(fp, GT_GRAPH);

    srand(time(NULL));
    int r = rand()%g->nverts();

    cout << endl << "nverts: " << g->nverts() << endl;
    cout << "nedges: " << g->nedges() << endl;
    cout << "rand: " << r + 1 << endl;

    Iterator * it = depth(g, r);

    cout << "\t** begin **" << endl;
    while(it->hasNext())
    {
        cout << it->next()+1 << endl;
    }
    cout << "\t ** end **" << endl;

    return 0;
}

