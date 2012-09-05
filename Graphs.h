#ifndef GRAPHS_H
#define GRAPHS_H

#include "Graph.h"
#include "Weight.h"
#include <queue>
#include <stack>
#include <cstdlib>

Graph * readDimacsGraph(FILE * graphFile, const graphType t);

Iterator * breadth(Graph *g, int v); //busca em largura

Iterator * depth(Graph *g, int v); // busca em profundedade

Iterator * connectedComp(Graph *g); // componentes conexos

Iterator * spanTree(Graph *g, Weight *w);  // arvore geradora maxima

Iterator * breadth(Graph *g, int v, Weight *w); // busca em largura ponderado

#endif // GRAPHS_H
