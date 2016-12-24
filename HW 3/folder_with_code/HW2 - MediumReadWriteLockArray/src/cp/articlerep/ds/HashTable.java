package cp.articlerep.ds;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.ReentrantReadWriteLock;




// this hash table is being used by a concurrent (multi-threaded) program
// and is not protected at all.  If you are 'lucky', everything goes all right.
// Otherwise you 'loose' elements that should be in the map (please note that
// if unprotected this data structure does not become inconsistent/corrupted)
public class HashTable<K, V> implements Map<K, V> {

	private static class Node {
		public Object key;
		public Object value;
		public Node next;
		
		public Node(Object key, Object value, Node next) {
			this.key = key;
			this.value = value;
			this.next = next;
		}
	}
	
	private ReentrantReadWriteLock [] allLocks;
	
	private Node[] table;
	private int count;
	
	
	public HashTable(int size) {
		this.table = new Node[size];
		this.count = 0;
		allLocks = new ReentrantReadWriteLock[size];
		for (int i = 0; i < size; i++)
			allLocks[i] = new ReentrantReadWriteLock ();
	}
	
	private int calcTablePos(K key) {
		return key.hashCode() % this.table.length;
	}
	

	
	
	@SuppressWarnings("unchecked")
	@Override
	public  V put(K key, V value) {
		int pos = this.calcTablePos(key);
		try{
			allLocks[pos].writeLock().lock();
			Node n = this.table[pos];

			// lookup for the node
			while (n != null && !n.key.equals(key)) {
				n = n.next;
			}
			
			// node already exists... update the value and return the old value
			if (n != null) {
				V oldValue = (V)n.value;
				n.value = value;
				return oldValue;
			}
			
			// node does not exists... create a new node and return 'null'
			Node nn = new Node(key, value, this.table[pos]);
			this.table[pos] = nn;
			count++;

			return null;
			
		}catch( Exception e ) {
			  Thread.currentThread().interrupt();
			  e.printStackTrace();
			  return null;
		}
		
		finally{
			allLocks[pos].writeLock().unlock();
		}
		
	}

	@SuppressWarnings("unchecked")
	@Override
	public V remove(K key) {
		int pos = this.calcTablePos(key);
		try{
			allLocks[pos].writeLock().lock();
			Node p = this.table[pos];
			
			// node does not exist
			if (p == null) {
				return null;
			}
			
			// is it the first node?
			if (p.key.equals(key)) {
				this.table[pos] = p.next;
				count--;
				return (V)p.value;
			}
			
			// it was not the first node... look for the right node
			Node n = p.next;
			while(n != null && !n.key.equals(key)) {
				p = n;
				n = n.next;
			}
			
			if (n == null) {
				// the node was not found
				return null;
			}
			
			// we found the node... lets remove it
			p.next = n.next;
			count--;
			
			return (V) n.value;
			
		}catch( Exception e ) {
			  Thread.currentThread().interrupt();
			  e.printStackTrace();
			  return null;
		}
		
		finally{
			allLocks[pos].writeLock().unlock();
		}
		
		
	}

	@SuppressWarnings("unchecked")
	@Override
	public V get(K key) {
		int pos = this.calcTablePos(key);
		try{
			allLocks[pos].readLock().lock();
			Node n = this.table[pos];
			
			// lookup for the node
			while(n != null && !n.key.equals(key)) {
				n = n.next;
			}
			
			// return the value if node was found, 'null' otherwise
			return (V) (n != null ? n.value : null);
			
		}catch( Exception e ) {
			  Thread.currentThread().interrupt();
			  e.printStackTrace();
			  return null;
		}
		
		finally{
			allLocks[pos].readLock().unlock();
		}
	}
	
	public int getSize(){
		return count;
	}
	
	public List<Integer> getElements(){

			List<Integer> elements = new LinkedList<Integer>();
			for (int i = 0; i< table.length; i++){
				Node n = this.table[i];
				while (n != null){
					elements.add((Integer) n.key);
					n = n.next;
				}
			}
			//System.out.println(elements.size());
			return elements;
		
	}
}
