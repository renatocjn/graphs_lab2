
/*
 * Graph.cpp
 *
 *  Created on: Sep 11, 2011
 *      Author: correa
 */

#include "Graph.h"
#include "Iterator.h"
#include "Weight.h"

Graph * Graphs::readDimacsGraph(FILE * graphFile, const graphType t) {
	char               	type  = ' ';
	char               	linestr[100];
	char *             	datastr;
	int                	i, j;
	int 				n;
	long long int		m;

	n = 0;
	m = 0;

	while (type != 'p') {
		type = fgetc(graphFile);
		if (type != EOF) {

			/* header */
			if (type == 'c') {
				datastr = fgets(linestr, 100, graphFile);
				if (datastr != NULL)
					printf("%s", linestr);
				else
					return NULL;
			}

			/* Vertices */
			if (type == 'p') {
				datastr = fgets(linestr, 100, graphFile);
				if (datastr == NULL)
					return NULL;

				datastr = strtok(linestr," ");

				datastr = strtok(NULL," ");
				n = atoi(datastr);

				datastr = strtok(NULL," ");
				m = atoll(datastr);
				if (t == GT_COMPLEMENT)
					m = ((((long long) (n))*((long long) (n)) - ((long long) n)) >> ((long long) 1)) - m;
			}
		}
	}

	////
	// Graph variables
	////

	printf("Graph with %d vertices and %lld edges.\n", n, m);
	Graph * ret = new Graph(n);

	if (t != GT_GRAPH) {
		ret->addAllEdges();
		for (i = 0; i < n; i++)
			ret->delEdge(i, i);
		m = ret->nedges() >> 1;
	}
	else
		m = 0;

	type = fgetc(graphFile);
	while (type != EOF) {
		/* Edges */
		if (type == 'e') {
			datastr = fgets(linestr, 100, graphFile);
			if (datastr == NULL)
				return NULL;

			datastr = strtok(linestr," ");
			i = atoi(datastr) - 1;

			datastr = strtok(NULL," ");
			j = atoi(datastr) - 1;

			if (t == GT_GRAPH) {
				ret->addEdge(i, j);
				ret->addEdge(j, i);
				(m)++;
			}
			else {
				ret->delEdge(i, j);
				ret->delEdge(j, i);
				(m)--;
			}
		}
		else {
			datastr = fgets(linestr, 100, graphFile);
			if (datastr != NULL)
				printf(" %s\n", linestr);
			else
				return NULL;
		}
		type = fgetc(graphFile);
	}

	if ((ret->nedges() >> 1) != m)
		printf("Ops! Reading %lld edges instead of %lld announced.\n", ret->nedges() >> 1, m);

	return ret;
}

Iterator * breadth(Graph *g, int v) //busca em largura
{
    return null;
}

Iterator * breadth(Graph *g, int v, Weight *w)
{
    return null;
}

Iterator * depth(Graph *g, int v) // busca em profundedade
{
    return null;
}

Iterator * connectedComp(Graph *g)
{
    return null;
}

Iterator * spanTree(Graph *g, Weight *w)
{
    return null;
}
