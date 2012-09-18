/*
 * Graph.h
 *
 *  Created on: Sep 11, 2011
 *      Author: correa
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstdio>
#include "AdjMatrix.h"

typedef enum GT { GT_GRAPH, GT_COMPLEMENT } graphType;

class Iterator {
 public:
  virtual bool hasNext() = 0;
  virtual int next() = 0;
};

class Graph : public AdjMatrix {
protected:
	Graph() {};
	Graph(Graph * bg);
public:
	Graph(int n);
	~Graph() {};

	virtual Graph * 	clone();
	virtual Graph * 	homomorphism(const int ar, const int * aR);
	virtual Graph * 	complement();
};

#endif /* GRAPH_H_ */
