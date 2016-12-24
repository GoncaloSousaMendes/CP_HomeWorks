package cp.articlerep.ds;

import java.util.List;

public interface Map<K, V> {
	public V put(K key, V value);
	public V remove(K key);
	public V get(K key);
	public List<Integer> getElements();
	public int getSize();
}
