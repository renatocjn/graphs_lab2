#include <iostream>
#include "Graph.h"
#include "Graphs.cpp"
#include <cstdio>

using namespace std;

int main()
{
    char * filename = "/home/renatocjn/Dropbox/Programas/Lab2/graph_lab2/instances/C125.9.clq";
    FILE * fp = fopen(filename, "r");
    Graph * g = readDimacsGraph(fp, GT_GRAPH);

    srand(time(NULL));
    int r = rand()%g->nverts();

    cout << endl << "vert: " << g->nverts() << endl;
    cout << "arest: " << g->nedges() << endl;
    cout << "rand: " << r << endl;

    Iterator * it = breadth(g, r);
    cout << "\t** begin **" << endl;
    while( it->hasNext() )
    {
//        cout << "entrou" << endl;//TESTE
        cout << it->next() << endl;
    }
    cout << "\t ** end **" << endl;

    return 0;
}

