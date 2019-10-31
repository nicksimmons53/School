// Library Imports
import java.io.*;
import java.math.*;

//
public class CacheSimulator {
  public static void main(String[ ] args) {
    String file = "";
    String repPolicy = "";
    int PHYS_MEM_BITS = 32;
    int cacheSize = 0;
    int blockSize = 0;
    int associativity = 0;
    double mBlocks;
    int tagBits;
    int indexBits;
    int mIndexes;
    int totalMemBits;

    // Parse Command Line and Convert Values
    for (int i = 0; i < args.length; i++) {
      switch(args[i]) {
        case "-f":
          file = args[i+1];
          break;
        case "-s":
          cacheSize = Integer.parseInt(args[i+1]);
          break;
        case "-b":
          blockSize = Integer.parseInt(args[i+1]);
          break;
        case "-a":
          associativity = Integer.parseInt(args[i+1]);
          break;
        case "-r":
          repPolicy = args[i+1];
          break;
      }
    }

    // Calculate Values
    mBlocks = ((powersOfTwo(cacheSize) + 10) - powersOfTwo(blockSize));
    mBlocks = Math.pow(2.0, mBlocks)/1024;
    indexBits = (powersOfTwo(cacheSize) + 10) - 
    tagBits = PHYS_MEM_BITS - powersOfTwo(blockSize);

    // Program Output
    System.out.println("Cache Simulator CS 3853 Fall 2019 - Group #");
    System.out.println( );

    System.out.printf("Cmd Line: java CacheSimulator %s\n", String.join(" ", args));
    System.out.printf("Trace File: %s\n", file);
    System.out.printf("Cache Size: %d\n", cacheSize);
    System.out.printf("Block Size: %d \n", blockSize);
    System.out.printf("Associativity: %d\n", associativity);
    System.out.printf("R-Policy: %s\n", repPolicy);
    System.out.println( );

    // Generic Values
    System.out.println("Generic: ");
    System.out.printf("Cache Size: %d KB\n", cacheSize);
    System.out.printf("Block Size: %d bytes\n", blockSize);
    System.out.printf("Associativity: %d\n", associativity);
    System.out.printf("Policy: %s\n", repPolicy);

    // Calculated Values
    // NEED TO CALCULATE VALUES
    System.out.println("----- Calculated Values -----");
    System.out.printf("Total #Blocks: %d KB\n", (int)mBlocks);
    System.out.println("Tag Size: bits");
    System.out.println("Index Size: bits, Total Indices: KB");
    System.out.println("Implementation Memory Size: bytes");

    // Parse File
    parseFile(file);

    // Results
    System.out.println("----- Results -----");
    System.out.println("Cache Hit Rate: %");
  }

  //
  public static void parseFile(String inFile) {
    try (BufferedReader br = new BufferedReader(new FileReader(inFile))) {
      String line;
      while ((line = br.readLine( )) != null) {
        // System.out.println(line);
      }
    } catch (Exception e) {
      System.err.print(e);
    }
  }

  public static int powersOfTwo(int sizeInKB) {
    switch (sizeInKB) {
      case 1024:
        return 10;
      case 512:
        return 9;
      case 256:
        return 8;
      case 128:
        return 7;
      case 64:
        return 6;
      case 32:
        return 5;
      case 16:
        return 4;
      case 8:
        return 3;
      case 4:
        return 2;
      case 2:
        return 1;
      case 1:
        return 0;
      default:
        System.out.println("Unknown Value");
        return -1;
    }
  }
}
