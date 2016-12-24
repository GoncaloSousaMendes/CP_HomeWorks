package cp.articlerep;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentSkipListSet;


public class Main {
	// BEGIN of data for sanity check 
	public static final boolean SANITY_CHECK = true;
	public static final Set<Integer> addedIds = new ConcurrentSkipListSet<Integer>();
	// END of data for sanity check 

	private static String[] wordArray;
	
	private static void populateWordArray(String fileName) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		String line;
		int i=0;
		
		String firstLine = br.readLine();
		int dictSize = Integer.parseInt(firstLine);
		wordArray = new String[dictSize];
		
		while((line = br.readLine()) != null && i < dictSize) {
			wordArray[i] = line;
			i++;
		}
		
		if (i < dictSize) {
			dictSize = i;
		}
		
		br.close();
	}
		
	
	public static void main(String[] args) throws InterruptedException, IOException {
		
		if (args.length < 2) {
			System.out.println("Usage: "+Main.class.getCanonicalName()+" time_in_milisecs num_threads");
			System.exit(0);
		}
		
		// get command line arguments
		Integer time = Integer.parseInt(args[0]);
		Integer numThreads = Integer.parseInt(args[1]);
		
		StopVar stopVar = new StopVar();
		Worker[] workers = new Worker[numThreads];
 		
		System.out.println("Starting application...");
		
		populateWordArray("resources/dictionary.txt");
		
		// launch the threads
		for (int i=0; i < numThreads; i++) {
			workers[i] = new Worker(wordArray,stopVar);
			workers[i].start();
		}
		
		// wait for the specified time
		Thread.sleep(time);
		stopVar.stop = true;
		
		// claculate statistics
		int sumOps = 0;
		int sumPutOps = 0;
		int sumRemOps = 0;
		int sumGetOps = 0;
		int inconsistency = 0;
		
		for (int i=0; i < numThreads; i++) {
			workers[i].join();
			sumOps += workers[i].getNumOps();
			sumPutOps += workers[i].getSuccPutOps();
			sumRemOps += workers[i].getSuccRemOps();
			sumGetOps += workers[i].getSuccGetOps();
			inconsistency += workers[i].getFalses();
		}
		
		
		 System.out.println("map size:" + Worker.getSharedMap().getSize() + " set size:" + addedIds.size());
		
		 int numberOfRep = 0;
		// do sanity sheck
		if (SANITY_CHECK) {
			for (Integer id : addedIds) {
				if (Worker.getSharedMap().get(id) == null) {
					System.err.println("Id "+id+" does not exist in hash map!");
				}
			}

			List<Integer> elements = Worker.getSharedMap().getElements();
			Iterator<Integer> it = elements.iterator();
			HashMap<Integer, Integer> countElements = new HashMap<Integer, Integer> (wordArray.length);
			
			while(it.hasNext()){
				int p = it.next();
				if (!addedIds.contains(p) )
					System.err.println("Id "+p+" does not exist in main set!");
				
				//verificar se cada id só está uma vez na hash, caso esteja mais que uma vez,
				//vários acessos foram feitos 'ao mesmo tempo'
				//por cada id, vamos contar as ocorrencias na hashtable
				//zero ocorrencias
				if(!countElements.containsKey(p))
					countElements.put(p, 0);
				else {
					int v = countElements.get(p);
					countElements.put(p, v++);
					numberOfRep++;
				}
					
				
				
			}
		}
		
		
		Thread.sleep(100);
		// print statistics
		System.out.println("There where "+ inconsistency + " inconsistencys");
		System.out.println("There where "+ numberOfRep + " repetitions on the hash table");
		System.out.println("Total operations: "+sumOps);
		System.out.println("Total successful puts: "+sumPutOps+" ("+Math.round(sumPutOps/(double)sumOps*100.0)+"%)");
		System.out.println("Total successful removes: "+sumRemOps+" ("+Math.round(sumRemOps/(double)sumOps*100.0)+"%)");
		System.out.println("Total successful gets: "+sumGetOps+" ("+Math.round(sumGetOps/(double)sumOps*100.0)+"%)");
		System.out.println("Throughput: "+(Math.round(sumOps/(time/1000.0)))+" ops/sec");
		
		System.out.println("Finished application.");
		
	}
}
