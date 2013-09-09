Prim 's Algorithm
===================

About
------------
In this programming problem you 'll code up Prim 's minimum spanning tree algorithm. 
The input file describes an undirected graph with integer edge costs. It has the format  
[number_of_nodes] [number_of_edges]  
[one_node_of_edge_1] [other_node_of_edge_1] [edge_1_cost]  
[one_node_of_edge_2] [other_node_of_edge_2] [edge_2_cost]  
...  
You should NOT assume that edge costs are positive, nor should you assume that they are distinct. 
Your task is to run Prim's minimum spanning tree algorithm on this graph. 
You should report the overall cost of a minimum spanning tree - an integer, which may or may not be negative.  
IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn) time implementation of Prim 's algorithm should work fine.  
OPTIONAL: For those of you seeking an additional challenge, try implementing a heap-based version. 
The simpler approach, which should already give you a healthy speed-up, is to maintain relevant edges 
in a heap (with keys = edge costs). The superior approach stores the unprocessed vertices in the heap, 
as described in lecture. Note this requires a heap that supports deletions, 
and you'll probably need to maintain some kind of mapping between vertices and 
their positions in the heap.


Running code examples
------------
**C** implementation:

You should compile the source file first.

    cd C/
    gcc -o PrimMST PrimMST.c (Linux)
    ./PrimMST ../in/inputBig.txt (Linux)
    
    compile with Visual Studio | MinGW | DevC++ (Windows)
    PrimMST.exe ../in/inputBig.txt (Windows)

**Java** implementation:

Contains also a JUnit test file for MinHeap structure.

Used java 1.6.0_33, junit 4.10 to compile source files.

    cd Java/
    java PrimMST ../in/inputBig.txt (Windows & Linux)
    java -cp "<path_to_junit_4.x_jar_file>;./" org.junit.runner.JUnitCore MinHeapTest (Windows)
    java -cp "<path_to_junit_4.x_jar_file>:./" org.junit.runner.JUnitCore MinHeapTest (Linux)

To compile yourself the source files:

    cd Java/
    javac Node.java Graph.java HeapNode.java MinHeap.java PrimMST.java (Windows & Linux)
    javac -cp "<path_to_junit_4.x_jar_file>;./" MinHeapTest.java (Windows)
    javac -cp "<path_to_junit_4.x_jar_file>:./" MinHeapTest.java (Linux)

For the input files in *in/* folder the expected results are:  
*inputBig.txt*: -3612829   
*inputMedium.txt*: 37  
*inputSmall.txt*: -27534  
