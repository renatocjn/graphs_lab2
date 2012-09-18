
/*
 * Graph.cpp
 *
 *  Created on: Sep 11, 2011
 *      Author: correa
 */

#include "Graphs.h"

Graph *readDimacsGraph(FILE * graphFile, const graphType t) { //coisas do prof correa
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

class breadth_iterator : public Iterator
{
    Graph* g;
    queue<int> * lista;    
    bool * visited;
    int * adj;
public:
    breadth_iterator(int v, Graph* g): g(g), lista(new queue<int>)
    {
        int n = g->nverts(); //variavel para evitar chamada multipla de g->nverts()

        //inicializando a lista
        lista->push(g->vertex(v));

        //vetor para vertices vizinhos
        adj = new int[n];

        //vetor para visualizar os visitados
        visited = new bool[n];

        //setando o vetor com false
        memset(visited, false, n*sizeof(bool));

        //fazendo assim eu pulo o primeiro elemento 'v' da busca
        //this->next();
    }

    int next()
    {
        if(lista->empty()) return -1;

        //receber e remover o elemento da lista
        int next = lista->front();
        lista->pop();

        //setar visitado
        visited[next] = true;

        int surplus; //nao sei para que essa variavel...
        //nisso eu descubro o grau do vetice 'next'
        int deg; g->adjSize(next, &deg);
        g->adjToArray(next, deg, adj, &surplus);

        for (int i=0; i<deg; i++) //loop da busca em largura
        {
            if(!visited[adj[i]])
            {                
                lista->push(adj[i]);
                visited[adj[i]] = true;
            }
        }

        return next;
    }

    bool hasNext()
    {
        return !lista->empty();
    }
};

Iterator *breadth(Graph *g, int v) //busca em largura
{
    return new breadth_iterator(v, g);
}

class depth_iterator : public Iterator
{
    stack<int> * pilha;
    Graph *g;
    int *adj;
    int *colors;

    //variaveis constantes para cores
    static const int BRANCO = 0, CINZA = 1, PRETO = 2;

public:
    depth_iterator(Graph *g, int v): pilha(new stack<int>), g(g)
    {
        int n = g->nverts(); //variavel para evitar chamada multipla de g->nverts()

        //vetor para vertices vizinhos
        adj = new int[n];

        //vetor para marcar as cores
        colors = new int[n];

        //setando cores brancas
        memset(colors, BRANCO, n*sizeof(int));

        //inicializando a pilha
        pilha->push(v);

        //pulo o primeiro elemento 'v'
        //this->next();
    }


    int next()
    {
        if(pilha->empty()) return -1;

        //removo o proximo da pilha e digo que ele é cinza
        int next = pilha->top();
        pilha->pop();

        //aqui vejo quantos nós vizinhos a 'next' existem, e coloco em 'adj_size'
        int adj_size;
        g->adjSize(next, &adj_size);

        //aqui coloco os vizinhos de 'next' no vetor adj
        int surplus;
        g->adjToArray(next, adj_size, adj, &surplus);

        for(int i=0; i<adj_size; i++)//loop busca em profundidade
        {
            if(colors[adj[i]] == BRANCO)
            {
                colors[adj[i]] = CINZA;
                pilha->push(adj[i]);
            }
        }

        //finalizando o nó com preto
        colors[next] = PRETO;

        return next;
    }

    bool hasNext()
    {
        return !pilha->empty();
    }
};

Iterator *depth(Graph *g, int v) // busca em profundedade
{
    return new depth_iterator(g, v);
}
