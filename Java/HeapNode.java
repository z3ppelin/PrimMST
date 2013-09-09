/**
 * Heap element.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.09.07
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/PrimMST
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

public class HeapNode implements Comparable {

    public int vertex;
    public int tailVertex;
    public int weight;

    /**
     * Constructor.
     *
     * @param vertex The vertex to store.
     * @param tailVertex The tail vertex.
     * @param weight The weight between tailVertex and vertex.
     */
    public HeapNode(int vertex, int tailVertex, int weight) {
        this.vertex = vertex;
        this.tailVertex = tailVertex;
        this.weight = weight;
    }
    
    /**
     * Override compareTo
     * @param obj   The object to compare to
     * @return
     */
    public int compareTo(Object obj) {
        if (!(obj instanceof HeapNode)) {
            throw new ClassCastException("A HeapNode object expected.");
        }
        if (this.weight > ((HeapNode) obj).weight) {
            return 1;
        } else if (this.weight == ((HeapNode) obj).weight) {
            return 0;
        }
        return -1;
    }
}
