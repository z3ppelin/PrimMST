/**
 * Prim 's minimum spanning tree algorithm implementation in O(m * log n);
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.09.07
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/PrimMST
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "PrimMST.h"

/**
 * Main function.
 * @param     argc       Command line arguments no.
 * @param     argv       Command line arguments.
 * @return               Success/error code. (0 is a success, anything else is error).
 */
int main(int argc, char** argv) {
	printf("------ Begin Prim 's MST ------\n");

	int i, startVertex;
	int *visited = NULL;
	long long sum = 0;
	Node *node = NULL;
	Graph graph;
	MinHeap heap;
	HeapNode *heapNode = NULL, *heapNode2;
	clock_t start, end;
	char error[128];
	double readTime = 0.00, algoTime = 0.00;

	/* read undirected graph, initialize variables */
	start = clock();
	if (argc == 1) {
		err("Err. The input file must be given as an argument.\n");
	}
	if (!readGraphFromFile(&graph, argv[1], error)) {
		err(error);
	}
	visited = (int*) malloc(sizeof(int) * graph.n);

	heap.lastFreePos = 0;
	heap.heapDimension = graph.n;
	heap.heapNodes = (HeapNode**) malloc(sizeof(HeapNode*) * graph.n);
	heap.positions = (int*) malloc(sizeof(int) * graph.n);

	srand(time(NULL));
	startVertex = rand() % graph.n;
	for (i = 0; i < graph.n; i++) {
		visited[i] = 0;

		heapNode = (HeapNode*) malloc(sizeof(HeapNode));
		heapNode->vertex = i;
		if (i == startVertex) {
			heapNode->tailVertex = startVertex;
			heapNode->weight = 0;
		} else {
			heapNode->tailVertex = -1;
			heapNode->weight = INT_MAX;

		}
		insertHeap(&heap, heapNode);
	}
	end = clock();
	readTime = (double) (end - start) / CLOCKS_PER_SEC;

	/* print read graph */
	//printf("The read graph:\n");
	//printGraph(graph);
	//printf("\n");
	
	/* start Prim 's algorithm */
	start = clock();
	for (i = 0; i < graph.n; i++) {
		heapNode = extractMin(&heap);
		visited[heapNode->vertex] = 1;
		sum += heapNode->weight;

		node = graph.edges[heapNode->vertex];
		while (NULL != node) {
			if (!visited[node->vertex]) {
				if (heap.heapNodes[heap.positions[node->vertex]]->weight > node->weight) {
					heapNode2 = deleteHeap(&heap, heap.positions[node->vertex]);
					heapNode2->tailVertex = heapNode->vertex;
					heapNode2->weight = node->weight;
					insertHeap(&heap, heapNode2);
				}
			}
			node = node->next;
		}
	}
	end = clock();
	algoTime = (double) (end - start) / CLOCKS_PER_SEC;

	/* print results */
	printf("Overral cost of minimum spanning tree is: %lld\n\n", sum);

	printf("Elapsed: %f seconds with initializations, reading graph.\n", readTime);
	printf("Elapsed: %f seconds to calculate overall cost of a minimum spanning tree.\n", algoTime);

	/* free left allocated memory */
	free(visited);
	freeGraph(&graph);
	freeMinHeap(&heap);
	free(heapNode);
	free(heapNode2);
	free(node);

	printf("------- End Prim 's MST -------\n");
	return EXIT_SUCCESS;
}

/**
 * Prints graph.
 * @param   graph   The graph to print.
 */
void printGraph(const Graph graph) {
	printf("Graph has %d vertices and %d edge(s).\n", graph.n, graph.m);
	Node* node;
	int i;
	for (i = 0; i < graph.n; i++) {
		printf("Vertex %d has edge(s) with: ", i + 1);
		node = graph.edges[i];
		if (NULL == node) {
			printf("nobody");
		} else {
			while (NULL != node) {
				printf("%d(%d) ", node->vertex + 1, node->weight);
				node = node->next;
			}
		}
		printf("\n");
	}
}

/**
 * Prints error and exits program.
 * @param    msg    The error to print.
 */
void err(const char* msg) {
	printf(msg);
	printf("------- End Prim 's MST -------\n");
	exit(EXIT_FAILURE);
}

/**
 * Inserts a new element in a heap maintaining the min-heap property.
 * @param		heap			The min heap.
 * @param		hn  			The new element to insert.
 */
void insertHeap(MinHeap* heap, HeapNode* hn) {
	if (heap->lastFreePos == heap->heapDimension) {
		err("Heap overflow");
	}
	heap->heapNodes[heap->lastFreePos] = hn;
	heap->positions[hn->vertex] = heap->lastFreePos;
	int childPos = heap->lastFreePos, parentPos = (childPos - 1) / 2;
	HeapNode* aux;
	/* bubble up */
	while (parentPos >= 0 && heap->heapNodes[parentPos]->weight > heap->heapNodes[childPos]->weight) {
		aux = heap->heapNodes[parentPos];
		heap->heapNodes[parentPos] = heap->heapNodes[childPos];
		heap->heapNodes[childPos] = aux;
		heap->positions[heap->heapNodes[childPos]->vertex] = childPos;
		heap->positions[heap->heapNodes[parentPos]->vertex] = parentPos;
		childPos = parentPos;
		parentPos = (childPos - 1) / 2;
	}
	heap->lastFreePos++;
}

/**
 * Extract the element with the smalles key in a heap and restores min-heap property.
 * @param	heap		The min heap.
 * @return	            The element with the smallest key in the heap.
 */
HeapNode* extractMin(MinHeap* heap) {
	if (heap->lastFreePos == 0) {
		err("Empty heap");
	}
	// It 's basicly a deletion from the first position without needing to bubble up
	HeapNode* hn = heap->heapNodes[0], *aux;
	heap->heapNodes[0] = heap->heapNodes[heap->lastFreePos - 1];
	heap->positions[heap->heapNodes[0]->vertex] = 0;
	heap->heapNodes[heap->lastFreePos - 1] = NULL;
	heap->positions[hn->vertex] = -1;
	heap->lastFreePos--;

	/* bubble down */
	int parentPos = 0, leftChildPos = (parentPos + 1) * 2 - 1, rightChildPos = leftChildPos + 1, minChildPos;
	while ((leftChildPos < heap->lastFreePos && heap->heapNodes[parentPos]->weight > heap->heapNodes[leftChildPos]->weight)
			|| (rightChildPos < heap->lastFreePos && heap->heapNodes[parentPos]->weight > heap->heapNodes[rightChildPos]->weight)) {
		minChildPos = leftChildPos;
		if (rightChildPos < heap->lastFreePos && heap->heapNodes[leftChildPos]->weight > heap->heapNodes[rightChildPos]->weight) {
			minChildPos = rightChildPos;
		}
		aux = heap->heapNodes[parentPos];
		heap->heapNodes[parentPos] = heap->heapNodes[minChildPos];
		heap->heapNodes[minChildPos] = aux;
		heap->positions[heap->heapNodes[minChildPos]->vertex] = minChildPos;
		heap->positions[heap->heapNodes[parentPos]->vertex] = parentPos;
		parentPos = minChildPos;
		leftChildPos = (parentPos + 1) * 2 - 1;
		rightChildPos = leftChildPos + 1;
	}
	return hn;
}

/**
 * Deletes an element in a heap and restores min-heap property.
 * @param	heap	The min heap.
 * @param	pos		The element 's position in the heap.
 * @return          The deleted element.
 */
HeapNode* deleteHeap(MinHeap* heap, const int pos) {
	if (pos < 0 || pos >= heap->lastFreePos) {
		err("Invalid position");
	}
	HeapNode* hn = heap->heapNodes[pos], *aux;
	heap->heapNodes[pos] = heap->heapNodes[heap->lastFreePos - 1];
	heap->positions[heap->heapNodes[pos]->vertex] = pos;
	heap->heapNodes[heap->lastFreePos - 1] = NULL;
	heap->positions[hn->vertex] = -1;
	heap->lastFreePos--;

	int parentPos = (pos - 1) / 2, childPos = pos;
	if (parentPos >= 0
        && NULL != heap->heapNodes[childPos]
        && heap->heapNodes[parentPos]->weight > heap->heapNodes[childPos]->weight) { // bubble up
		while (parentPos >= 0 && heap->heapNodes[parentPos]->weight > heap->heapNodes[childPos]->weight) {
			aux = heap->heapNodes[parentPos];
			heap->heapNodes[parentPos] = heap->heapNodes[childPos];
			heap->heapNodes[childPos] = aux;
			heap->positions[heap->heapNodes[childPos]->vertex] = childPos;
			heap->positions[heap->heapNodes[parentPos]->vertex] = parentPos;
			childPos = parentPos;
			parentPos = (childPos - 1) / 2;
		}
	} else { // bubble down
		parentPos = pos;
		int leftChildPos = (parentPos + 1) * 2 - 1, rightChildPos = leftChildPos + 1, minChildPos;
		while ((leftChildPos < heap->lastFreePos && heap->heapNodes[parentPos]->weight > heap->heapNodes[leftChildPos]->weight)
				|| (rightChildPos < heap->lastFreePos && heap->heapNodes[parentPos]->weight > heap->heapNodes[rightChildPos]->weight)) {
			minChildPos = leftChildPos;
			if (rightChildPos < heap->lastFreePos && heap->heapNodes[leftChildPos]->weight > heap->heapNodes[rightChildPos]->weight) {
				minChildPos = rightChildPos;
			}
			aux = heap->heapNodes[parentPos];
			heap->heapNodes[parentPos] = heap->heapNodes[minChildPos];
			heap->heapNodes[minChildPos] = aux;
			heap->positions[heap->heapNodes[minChildPos]->vertex] = minChildPos;
			heap->positions[heap->heapNodes[parentPos]->vertex] = parentPos;
			parentPos = minChildPos;
			leftChildPos = (parentPos + 1) * 2 - 1;
			rightChildPos = leftChildPos + 1;
		}
	}
	return hn;
}

/**
 * Reads graph from file.
 * @param      file      The file where to read graph from.
 * @param      err       An error message, if any occcurred during reading.
 * @return               1 if everything went fine, 0 otherwise.
 */
int readGraphFromFile(Graph* graph, char* file, char* err) {
	FILE *inputFile = fopen(file, "rt");
	int i, vertex1, vertex2, weight;
	Node** edges;
	char buffer[10000];
	Node *node;
	if (NULL == inputFile) {
		strcpy(err, "Err. Could not open file.\n");
		return 0;
	}
	if (NULL == fgets(buffer, sizeof buffer, inputFile)) {
		fclose(inputFile);
		strcpy(err, "Err. Could not read number of vertices & edges the graph has.\n");
		return 0;
	}
	if (sscanf(buffer, "%d %d", &graph->n, &graph->m) != 2) {
		fclose(inputFile);
		strcpy(err, "Err. Could not read number of vertices & edges the graph has.\n");
		return 0;
	}

	edges = (Node**) malloc(sizeof(Node*) * graph->n);
	for (i = 0; i < graph->n; i++) {
		edges[i] = NULL;
	}
	for (i = 0; i < graph->m; i++) {
		if (NULL == fgets(buffer, sizeof buffer, inputFile)) {
			fclose(inputFile);
			strcpy(err, "Err. Number of edges does not match with the ones declared in the file.\n");
			return 0;
		}
		if (sscanf(buffer, "%d %d %d", &vertex1, &vertex2, &weight) != 3) {
			fclose(inputFile);
			strcpy(err, "Err. Could not read an edge.\n");
			return 0;
		}
		vertex1--;
		vertex2--;
		node = (Node*) malloc(sizeof(Node));
		node->vertex = vertex2;
		node->weight = weight;
		if (NULL == edges[vertex1]) {
			node->next = NULL;
		} else {
			node->next = edges[vertex1];
		}
		edges[vertex1] = node;
		/* graph is undirected, so keep the viceversa nodes */
		node = (Node*) malloc(sizeof(Node));
		node->vertex = vertex1;
		node->weight = weight;
		if (NULL == edges[vertex2]) {
			node->next = NULL;
		} else {
			node->next = edges[vertex2];
		}
		edges[vertex2] = node;
	}
	fclose(inputFile);
	graph->edges = edges;
	return 1;
}

/**
 * Frees memory occupied by pointer members of a graph.
 * @param   graph    The graph to free pointer members.
 */
void freeGraph(Graph* graph) {
	Node *node1, *node2;
	int i;
	for (i = 0; i < graph->n; i++) {
		node1 = graph->edges[i];
		while (NULL != node1) {
			node2 = node1;
			node1 = node1->next;
			free(node2);
		}
	}
	free(graph->edges);
}

/**
 * Frees memory occupied by pointer members of a min-heap.
 * @param   graph    The min-heap to free pointer members.
 */
void freeMinHeap(MinHeap* heap) {
	int i;
	for (i = 0; i < heap->heapDimension; i++) {
		free(heap->heapNodes[i]);
	}
	free(heap->heapNodes);
	free(heap->positions);
}
