import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class FileReadThread implements Runnable {
	BufferedReader input;

	public FileReadThread(String name) {
		try {
			FileReader file = new FileReader(name);
			input = new BufferedReader(file);
		} catch (FileNotFoundException e) {
			e.printStackTrace( );
		}
	}

	@Override
	public void run( ) {
		String line = "";
		String parsedLine[ ];

		try {
			while ((line = input.readLine( )) != null) {
				line = line.trim( );
				parsedLine = line.split("\\s+");

				switch (parsedLine[0]) {
					case "proc":
						PCB pcb = new PCB( );
						break;
					case "sleep":
						Thread.sleep(Integer.parseInt(parsedLine[1]));
						break;
					case "stop":
						return;
					default:
						System.out.println("Error Reading Line");
						System.exit(-1);
				}
			}
		} catch (IOException e) {
			e.printStackTrace( );
		}
	}
}
