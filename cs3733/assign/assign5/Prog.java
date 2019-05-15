import java.util.ArrayList;

public class Prog {
	ArrayList <PCB> cpuQueue = new ArrayList<PCB>( );

	public static void main(String[ ] args) {
		String algorithm = null;
		String file = null;
		int quantumMS = -1;
		int totalProcesses = 0;

		// Parse Command Line
		for (int i = 0; i < args.length; i++) {
			switch(args[i]) {
				case "-alg":
					algorithm = args[i + 1];
					i++;
					break;
				case "-quantum":
					quantumMS = Integer.parseInt(args[i + 1]);
					i++;
					break;
				case "-input":
					file = args[i + 1];
					i++;
					break;
				default:
					System.out.println("Invalid Command Line Argument");
					break;
			}
		}

		// File read
		FileReadThread read = new FileReadThread(file);
		Thread readThread = new Thread(read);
		readThread.start( );

		// Loop through completed processes

		// Print output
		printOutput(file, algorithm, quantumMS);
	}

	public static void printOutput(String file, String alg, int q) {
		System.out.printf("Input File Name\t\t\t\t: " + file + "\n");
		System.out.printf("CPU Scheduling Alg\t\t\t: " + alg + "\n");
		System.out.printf("CPU Utilization\t\t\t\t: " + "\n");
		System.out.printf("Throughput\t\t\t\t: " + "\n");
		System.out.printf("Avg. Turnaround Time\t\t\t: " + "\n");
		System.out.printf("Avg. Waiting Time in R queue\t\t: " + "\n");
	}
}
