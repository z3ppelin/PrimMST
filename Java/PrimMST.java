/**
 * Implementation of Dijkstra 's Shortest Path Algorithm in O(n + log m), n =
 * number of vertices, m = number of edges.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.09.07
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/PrimMST
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

import java.io.FileInputStream;
import java.util.*;

public class PrimMST {
    /**
     * Main function. Reads graph, calculates shortest path from a starting
     * vertex to all other vertices and prints the result.
     *
     * @param String[] argv Command line arguments.
     * @return void
     */
    public static void main(String[] args) throws Exception {
        System.out.println("------ Begin Prim 's MST ------");
        long start, end, sum = 0;
        Graph graph = null;
        MinHeap heap = null;
        HeapNode hn, hn2;
        Node n;
        int startVertex;
        boolean[] visited = null;
        double readTime = 0.00, algoTime = 0.00;

        /* read directed graph, initialize variables */
        start = System.currentTimeMillis();
        try {
            if (args.length == 0) {
                throw new Exception("The input file must be given as an argument.");
            }
            graph = readGraphFromFile(args[0]);
            heap = new MinHeap(graph.n);
            visited = new boolean[graph.n];
            startVertex = (int) (Math.random() * graph.n);
            
            for (int i = 0; i < graph.n; i++) {
                visited[i] = false;

                if (startVertex == i) {
                    hn = new HeapNode(i, startVertex, 0);
                } else {
                    hn = new HeapNode(i, -1, Integer.MAX_VALUE);
                }
                try {
                    heap.insert(hn);
                } catch (Exception e) {
                    System.out.println("ERR. " + e.getMessage());
                }
            }
        } catch (Exception ex) {
            System.out.println("ERR. " + ex.getMessage());
            System.out.println("------- End Prim 's MST -------");
            System.exit(-1);
        }
        end = System.currentTimeMillis();
        readTime = (double) (end - start) / 100;

        /* print read graph */
        //System.out.println("The read graph:");
        //printGraph(graph);
        //System.out.println();

        /* start Prim 's algorithm */
        start = System.currentTimeMillis();
        for (int i = 0; i < graph.n; i++) {
            hn = heap.extractMin();
            visited[hn.vertex] = true;
            sum += hn.weight;

            n = graph.edges[hn.vertex];
            while (null != n) {
                if (!visited[n.vertex]) {
                    if (n.weight < heap.heapNodes[heap.positions[n.vertex]].weight) {
                        hn2 = heap.delete(heap.positions[n.vertex]);
                        hn2.weight = n.weight;
                        hn2.tailVertex = hn.vertex;
                        heap.insert(hn2);
                    }
                }
                n = n.next;
            }
        }
        end = System.currentTimeMillis();
        algoTime = (double) (end - start) / 100;

        /* print result */
        System.out.println("Overral cost of minimum spanning tree is: " + sum + "\n");
        
        System.out.println("Elapsed: " + readTime + " seconds with initializations, reading graph.");
        System.out.println("Elapsed: " + algoTime + " seconds to calculate overall cost of a minimum spanning tree.");
        System.out.println("------- End Prim 's MST -------\n");
    }

    /**
     * Prints graph.
     *
     * @param graph The graph to print.
     */
    public static void printGraph(Graph graph) {
        System.out.println("Graph has " + graph.n + " vertices and " + graph.m + " edge(s).");
        Node node;
        for (int i = 0; i < graph.n; i++) {
            System.out.print("Vertex " + (i + 1) + " has edge(s) with: ");
            node = graph.edges[i];
            if (null == node) {
                System.out.print("nobody");
            } else {
                while (null != node) {
                    System.out.print((node.vertex + 1) + "(" + node.weight + ") ");
                    node = node.next;
                }
            }
            System.out.println();
        }
    }

    /**
     * Reads graph from file.
     *
     * @param file The file where to read the graph from.
     * @return The read graph.
     * @throws Exception
     */
    public static Graph readGraphFromFile(String file) throws Exception {
        Scanner sc;
        StringTokenizer st, st2;
        FileInputStream fis = null;
        int n, m, vertex1, vertex2, weight;
        Node[] edges = null;
        String line;

        try {
            fis = new FileInputStream(file);
            sc = new Scanner(fis);
            if (!sc.hasNextInt()) {
                throw new Exception("Could not read number of vertices the graph has.");
            }
            n = sc.nextInt();
            if (!sc.hasNextInt()) {
                throw new Exception("Could not read number of edges the graph has.");
            }
            m = sc.nextInt();
            if (sc.hasNextLine()) sc.nextLine();
            
            edges = new Node[n];
            for (int i = 0; i < m; i++) {
                if (!sc.hasNextLine()) {
                    throw new Exception("Could not read edge on line " + (i + 1));
                }
                line = sc.nextLine();
                st = new StringTokenizer(line);
                if (st.hasMoreElements()) {
                    vertex1 = Integer.parseInt((String) st.nextElement()) - 1;
                } else {
                    throw new Exception("Could not read vertex 1 on line " + (i + 1));
                }
                if (st.hasMoreElements()) {
                    vertex2 = Integer.parseInt((String) st.nextElement()) - 1;
                } else {
                    throw new Exception("Could not read vertex 2 on line " + (i + 1));
                }
                if (st.hasMoreElements()) {
                    weight = Integer.parseInt((String) st.nextElement());
                } else {
                    throw new Exception("Could not read weight on line " + (i + 1));
                }
                
                if (null == edges[vertex1]) {
                    edges[vertex1] = new Node(vertex2, weight);
                } else {
                    edges[vertex1] = new Node(vertex2, weight, edges[vertex1]);
                }
                if (null == edges[vertex2]) { // graph is undirected
                    edges[vertex2] = new Node(vertex1, weight);
                } else {
                    edges[vertex2] = new Node(vertex1, weight, edges[vertex2]);
                } 
            }
            fis.close();
        } catch (Exception ex) {
            if (fis != null) {
                try {
                    fis.close();
                } catch (Exception e) {
                }
            }
            throw ex;
        }
        return new Graph(n, m, edges);
    }
}
