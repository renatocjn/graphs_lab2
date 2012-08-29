
/*
 * Graph.cpp
 *
 *  Created on: Sep 11, 2011
 *      Author: correa
 */

#include "Graph.h"

template <class T> class Complement;

class Homomorphism : public Graph {
	friend class Graph;
	friend class Complement<Homomorphism>;
	friend class Complement<Graph>;

	int 			r;
	int *			R;
	long long 		re;
	int				lastnechanges;
	bool			alloc;

	Homomorphism(Homomorphism * bg);
	Homomorphism(const int ar, const int * aR, Graph * bg);
	Homomorphism(const int ar, int * aR, Homomorphism * const bg);

	int 					nverts();
	virtual long long int 	nedges();
	virtual bool 			addEdge(const int i, const int j);
	virtual bool 			delEdge(const int i, const int j);
	void 					addAllEdges();
	void 					delAllEdges();
	virtual bool 			hasEdge(const int i, const int j);
	int 					vertex(const int i);
	virtual Graph * 		clone();
	virtual Graph * 		homomorphism(const int ar, const int * aR);
	virtual Graph * 		complement();
public:
	virtual ~Homomorphism();
};

template <class T>
class Complement : public T {
	friend class Graph;
	friend class Homomorphism;

	T * 		baseg;
	long long 	cne;
	int			lastnechanges;
	Graph *		homom;

	Complement() {};
	Complement(T * bg);

	long long int 	nedges();
	bool 			hasEdge(const int i, const int j);
	bool 			addEdge(const int i, const int j);
	bool 			delEdge(const int i, const int j);
	Graph *			clone();
	Graph *			complement();
	Graph *			homomorphism(const int ar, const int * aR);
public:
	~Complement();
};

// AdjMatrix

int AdjMatrix::adjToArray(const int i, const int asz, int * const ng, int * const surplus_p) {
	int ia = 0, j;
	*surplus_p = asz;
	if (hasEdge(i, i)) {
		(*surplus_p)--;
		if (*surplus_p >= 0)
			ng[ia++] = i;
	}
	for (j = 0; j < nverts(); j++)
		if (i != j && hasEdge(i, j)) {
			(*surplus_p)--;
			if (*surplus_p >= 0)
				ng[ia++] = j;
		}

	return (*surplus_p < 0) ? GSTERR_NEGATIVE_SURPLUS : 0;
}

int AdjMatrix::adjToArrays(const int asz, int * const ad, int * const ang, int * const surplus_p) {
	int ia = 0, aa = 0, j;
	int i;
	*surplus_p = asz;
	for (i = 0; i < nverts(); i++) {
		ad[i] = 0;
		if (hasEdge(i, i)) {
			(*surplus_p)--;
			if (*surplus_p >= 0)
				ang[aa++] = i;
			ad[i]++;
		}
		for (j = 0; j < nverts(); j++)
			if (i != j && hasEdge(i, j)) {
				(*surplus_p)--;
				if (*surplus_p >= 0)
					ang[aa++] = j;
				ad[i]++;
			}
	}

	return (*surplus_p < 0) ? GSTERR_NEGATIVE_SURPLUS : 0;
}

int AdjMatrix::adjSize(const int i, int * const deg) {
	int j;
	(*deg) = 0;
	for (j = 0; j < nverts(); j++)
		if (hasEdge(i, j))
			(*deg)++;

	return 0;
}

int AdjMatrix::adjSizes(int * const deg) {
	memset(deg, 0, nverts()*sizeof(int));

	int iu, iv;
	for (iu = 0; iu < nverts(); iu++) {
		for (iv = iu+1; iv < nverts(); iv++)
			if (hasEdge(iu, iv)) {
				deg[iu]++;
				deg[iv]++;
			}
	}

	return 0;
}

// Graph

Graph::Graph(int n) : AdjMatrix(n) {
	AdjMatrix::nechanges = 0;
}

Graph::Graph(Graph * bg) {
	linkTo(bg);
}

Graph * Graph::clone() {
	Graph * dest = new Graph(nverts());
	dest->copyTo(this);
	return dest;
}

Graph * Graph::homomorphism(const int r, const int * R) {
	return new Homomorphism(r, R, this);
}

Graph * Graph::complement() {
	return new Complement<Graph>(this);
}

// Homomorphism

inline Homomorphism::Homomorphism(const int ar, int * aR, Homomorphism * const bg) : Graph(bg), R(aR) {
	r = ar;
	re = -1;
	alloc = true;
}

inline Homomorphism::Homomorphism(const int ar, const int * aR, Graph * const bg) : Graph(bg) {
	r = ar;
	R = new int[r];
	memcpy(R, aR, r*sizeof(int));
	re = -1;
	alloc = true;
}

inline Homomorphism::Homomorphism(Homomorphism * const bg) : Graph(bg), R(bg->R) {
	re = bg->re;
	r = bg->r;
	alloc = false;
}

inline Homomorphism::~Homomorphism() {
	if (alloc) {
		delete R;
	}
}

inline int Homomorphism::nverts() {
	return r;
}

inline long long int Homomorphism::nedges() {
	if (re < 0 || lastnechanges != AdjMatrix::nechanges) {
		int i, j;
		re = 0;
		for (i = 0; i < nverts(); i++)
			for (j = 0; j < nverts(); j++)
				if (hasEdge(i, j))
					re++;
		lastnechanges = AdjMatrix::nechanges;
	}
	return re;
}

inline bool Homomorphism::addEdge(const int i, const int j) {
	bool ret = Graph::addEdge(R[i], R[j]);
	if (re >= 0 && ret && lastnechanges == AdjMatrix::nechanges - 1) {
		re++;
		lastnechanges = AdjMatrix::nechanges;
	}
	return ret;
}

inline bool Homomorphism::delEdge(const int i, const int j) {
	bool ret = Graph::delEdge(R[i], R[j]);
	if (re >= 0 && ret && lastnechanges == AdjMatrix::nechanges - 1) {
		re--;
		lastnechanges = AdjMatrix::nechanges;
	}
	return ret;
}

inline void Homomorphism::addAllEdges() {
	int i, j;
	for (i = 0; i < nverts(); i++)
		for (j = 0; j < nverts(); j++)
			addEdge(i, j);
}

inline void Homomorphism::delAllEdges() {
	int i, j;
	for (i = 0; i < nverts(); i++)
		for (j = 0; j < nverts(); j++)
			delEdge(i, j);
}

inline bool Homomorphism::hasEdge(const int i, const int j) {
	return Graph::hasEdge(R[i], R[j]);
}

inline int Homomorphism::vertex(const int i) {
	return Graph::vertex(R[i]);
}

inline Graph * Homomorphism::clone() {
	int * RR = new int[nverts()];
	memcpy(RR, R, nverts()*sizeof(int));
	Homomorphism * ret = (Homomorphism *) Graph::clone()->homomorphism(r, RR);
	ret->re = re;
	ret->lastnechanges = lastnechanges;
	return ret;
}

inline Graph * Homomorphism::homomorphism(const int ar, const int * aR) {
	int i;
	int * Raux = new int[ar];
	for (i = 0; i < ar; i++)
		Raux[i] = R[aR[i]];
	return new Homomorphism(ar, Raux, this);
}

inline Graph * Homomorphism::complement() {
	return new Complement<Homomorphism>(this);
}


// Complement

template <class T>
inline Complement<T>::Complement(T * bg) : T(bg), homom(NULL) {
	baseg = bg;
}

template <class T>
inline Complement<T>::~Complement() {
	if (homom != NULL)
		delete homom;
}

template <class T>
inline long long int Complement<T>::nedges() {
	int nv = T::nverts();
	cne = nv*nv-T::nedges();
	return cne;
}

template <class T>
inline bool Complement<T>::hasEdge(const int i, const int j) {
	return !T::hasEdge(i, j);
}

template <class T>
inline bool Complement<T>::addEdge(const int i, const int j) {
	return T::delEdge(i, j);
}

template <class T>
inline bool Complement<T>::delEdge(const int i, const int j) {
	return T::addEdge(i, j);
}

template <class T>
inline Graph * Complement<T>::clone() {
	return T::clone()->complement();
}

template <class T>
inline Graph * Complement<T>::complement() {
	return baseg;
}

template <class T>
inline Graph * Complement<T>::homomorphism(const int ar, const int * aR) {
	if (homom == NULL)
		homom = T::homomorphism(ar, aR);
	return homom->complement();
}
