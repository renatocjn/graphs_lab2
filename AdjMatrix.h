/*
 * AdjMatrix.h
 *
 *  Created on: Sep 11, 2011
 *      Author: correa
 */

#ifndef ADJMATRIX_H_
#define ADJMATRIX_H_

#include <cstring>
using namespace std;

static inline int inline_ceillog2(int n) {
	int c = 0;
	int l = 0;
	while (n > 1) {
		c |= (n & 1);
		n >>= 1;
		l++;
	}
	l += c;
	return l;
}

static inline char edgeMask(const int j) {
	return 1 << (char) (j & 0x07);
}

// Abstract class

class AdjMatrix {
	char * 			matrix;
	long 			matrixsize;
	int				logrowsize;
	// number of vertices
	int  			nv;
	// number of edges
	long long *		ne;
	bool			islinked;

	long edgeIndex(const int i, const int j);
protected:
	int	nechanges;
	void copyTo(AdjMatrix * orig);
	void linkTo(AdjMatrix * orig);
	AdjMatrix() {};
	AdjMatrix(int sz);
public:
	virtual ~AdjMatrix();
	// vertex and edge manipulation
	virtual int 			nverts();
	virtual long long int 	nedges();
	virtual bool 			addEdge(const int i, const int j);
	virtual bool 			delEdge(const int i, const int j);
	virtual void 			addAllEdges();
	virtual void 			delAllEdges();
	// adjacency manipulation
	virtual bool 			hasEdge(const int i, const int j);
	virtual int 	vertex(const int i);
	// first vertex of i's neighbor array is the vertex i if the adj matrix has the loop (i,i)
	int 			adjToArray(const int i, const int asz, int * const ang, int * const surplus_p);
	int 			adjToArrays(const int asz, int * const ad, int * const ang, int * const surplus_p);
	int 			adjSize(const int i, int * const deg);
	int 			adjSizes(int * const deg);
};

inline AdjMatrix::AdjMatrix(int sz) {
	logrowsize = (inline_ceillog2(sz)-3);
	matrixsize = ((long) 1 << logrowsize)*sz;
	matrix = new char[matrixsize];
	nv = sz;
	ne = new long long;
	*ne = 0;
	nechanges = 0;
	islinked = false;
}

inline AdjMatrix::~AdjMatrix() {
	if (!islinked) {
		delete ne;
		delete matrix;
	}
}

inline void AdjMatrix::copyTo(AdjMatrix * orig) {
	matrixsize = orig->matrixsize;
	logrowsize = orig->logrowsize;
	memcpy(matrix, (void *) orig->matrix, matrixsize);
	nv = orig->nverts();
	*ne = orig->nedges();
}

inline void AdjMatrix::linkTo(AdjMatrix * orig) {
	matrix = orig->matrix;
	matrixsize = orig->matrixsize;
	logrowsize = orig->logrowsize;
	nv = orig->nverts();
	ne = orig->ne;
	islinked = true;
}

inline long AdjMatrix::edgeIndex(const int i, const int j) {
	return ((long) i << logrowsize)+(j >> 3);
}

inline int AdjMatrix::nverts() {
	return nv;
}

inline long long int AdjMatrix::nedges() {
	return *ne;
}

inline bool AdjMatrix::hasEdge(const int i, const int j) {
	return matrix[edgeIndex(i, j)] & edgeMask(j);
}

inline bool AdjMatrix::addEdge(const int i, const int j) {
	bool nhe = !AdjMatrix::hasEdge(i, j);
	if (nhe) {
		(*ne)++;
		nechanges++;
		matrix[edgeIndex(i, j)] |= edgeMask(j);
	}
	return nhe;
}

inline bool AdjMatrix::delEdge(const int i, const int j) {
	bool he = AdjMatrix::hasEdge(i, j);
	if (he) {
		(*ne)--;
		nechanges++;
		matrix[edgeIndex(i, j)] &= ~edgeMask(j);
	}
	return he;
}

inline void AdjMatrix::addAllEdges() {
	memset((void *) matrix, 0xFF, matrixsize);
	int nv = AdjMatrix::nverts();
	*ne = nv*nv;
	nechanges++;
}

inline void AdjMatrix::delAllEdges() {
	memset((void *) matrix, 0, matrixsize);
	*ne = 0;
	nechanges++;
}

inline int AdjMatrix::vertex(const int i) {
	return i;
}

#endif /* ADJMATRIX_H_ */
