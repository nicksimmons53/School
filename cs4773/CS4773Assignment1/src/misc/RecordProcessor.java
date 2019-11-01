package misc;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Set;

/**
 * @author Prof. Robsinson
 * @author nick simmons (srz888)
 *
 * This function processes employee records and calculates financial statistics for 
 * each employee from a file. 
 */
public class RecordProcessor {
	private static StringBuffer stringBuffer = new StringBuffer( );
	private static Employee employee;
	private static Statistics employeeStatistics = new Statistics( );
	private static HashMap<String, Integer> firstNameHashMap = new HashMap<String, Integer>( );
	private static HashMap<String, Integer> lastNameHashMap = new HashMap<String, Integer>( );
	
	/**
	 * processes a file of employees and calls the specifed functions to calculate and print data
	 * @param listOfEmployeesFile
	 * @return		stringBuffer.toString
	 */
	public static String processFile(String listOfEmployeesFile) {
		Scanner employeeFile = fileOpen(listOfEmployeesFile);
		int totalNumberOfEmployees = countEmployeesInFile(employeeFile);
		employee = new Employee(totalNumberOfEmployees);
		employeeFile.close( );
		
		employeeFile = fileOpen(listOfEmployeesFile);
		totalNumberOfEmployees = readEmployeeData(employeeFile);
		if (totalNumberOfEmployees == -1) { 
			return null;
		} else if(totalNumberOfEmployees == 0) {
			System.err.println("No records found in data file");
			employeeFile.close();
			return null;
		}
		
		printAllEmployeesData( );
		calculateAverageWages( );
		printAverageWages( );

		int sumMatchingFirstNames = insertEmployeeHashMap(employee.firstName, firstNameHashMap);
		stringBuffer.append(String.format("\nFirst names with more than one person sharing it:\n"));
		printMatchingNames(employee.firstName, firstNameHashMap, sumMatchingFirstNames);

		int sumMatchingLastNames = insertEmployeeHashMap(employee.lastName, lastNameHashMap);
		stringBuffer.append(String.format("\nLast names with more than one person sharing it:\n"));
		printMatchingNames(employee.lastName, lastNameHashMap, sumMatchingLastNames);
		
		employeeFile.close();
		return stringBuffer.toString();
	}
	
	private static void printAllEmployeesData( ) {
		stringBuffer.append(String.format("# of people imported: %d\n", employee.firstName.length));
		
		stringBuffer.append(String.format("\n%-30s %s  %-12s %12s\n", "Person Name", "Age", "Emp. Type", "Pay"));
		for(int i = 0; i < 30; i++)
			stringBuffer.append(String.format("-"));
		stringBuffer.append(String.format(" ---  "));
		for(int i = 0; i < 12; i++)
			stringBuffer.append(String.format("-"));
		stringBuffer.append(String.format(" "));
		for(int i = 0; i < 12; i++)
			stringBuffer.append(String.format("-"));
		stringBuffer.append(String.format("\n"));
		
		for(int i = 0; i < employee.firstName.length; i++) {
			stringBuffer.append(String.format("%-30s %-3d  %-12s $%12.2f\n", employee.firstName[i] + " " + 
				employee.lastName[i], employee.age[i]
				, employee.payFrequency[i], employee.payAmount[i]));
		}
	}
	
	private static int readEmployeeData(Scanner employeeFile) {
		int amountOfEmployees = 0;
		while(employeeFile.hasNextLine( )) {
			String employeeLineFromFile = employeeFile.nextLine();
			if(employeeLineFromFile.length( ) <= 0) 
				break;
				
			String [ ] employeeAttributes = employeeLineFromFile.split(",");
			int singleEmployeeFromFile = 0; 
			singleEmployeeFromFile = setBeginningIndex(singleEmployeeFromFile, amountOfEmployees, employeeAttributes);
			employeeAttributes = assignEmployeeInfo(employeeFile, singleEmployeeFromFile, employeeAttributes);
			if (employeeAttributes == null)
				return -1;
			amountOfEmployees++;
		}
		return amountOfEmployees;
	}	
	
	private static String[ ] assignEmployeeInfo(Scanner employeeFile, int singleEmployeeFromFile, String [ ] employeeAttributes) {
		employee.firstName[singleEmployeeFromFile] = employeeAttributes[0];
		employee.lastName[singleEmployeeFromFile] = employeeAttributes[1];
		employee.payFrequency[singleEmployeeFromFile] = employeeAttributes[3];

		try {
			employee.age[singleEmployeeFromFile] = Integer.parseInt(employeeAttributes[2]);
			employee.payAmount[singleEmployeeFromFile] = Double.parseDouble(employeeAttributes[4]);
		} catch(Exception exception) {
			System.err.println(exception.getMessage( ));
			employeeFile.close( );
			return null;
		}
		return employeeAttributes;
	}

	private static int insertEmployeeHashMap(String[ ] employeeNameType, HashMap<String, Integer> nameHashMap) {
		int sumMatchingNames = 0;
		for(int i = 0; i < employeeNameType.length; i++) {
			if(nameHashMap.containsKey(employeeNameType[i])) {
				nameHashMap.put(employeeNameType[i], nameHashMap.get(employeeNameType[i]) + 1);
				sumMatchingNames++;
			} else {
				nameHashMap.put(employeeNameType[i], 1);
			}
		}
		return sumMatchingNames;
	}
	
	private static void printMatchingNames(String[ ] employeeNameType, HashMap<String, Integer> nameHashMap, int sumMatchingNames) {
		if(sumMatchingNames > 0) {
			Set<String> employeeNameSet = nameHashMap.keySet();
			for(String names : employeeNameSet) {
				if(nameHashMap.get(names) > 1) 
					stringBuffer.append(String.format("%s, # people with this name: %d\n", names, nameHashMap.get(names)));
			}
		} else 
			stringBuffer.append(String.format("All first names are unique"));
	}
	
	private static void calculateAverageWages( ) {
		for(int i = 0; i < employee.firstName.length; i++) {
			employeeStatistics.sumOfAges += employee.age[i];
			if(employee.payFrequency[i].equals("Commission")) {
				employeeStatistics.totalCommissionEarnings += employee.payAmount[i];
				employeeStatistics.numOfCommissionEmployees++;
			} else if(employee.payFrequency[i].equals("Hourly")) {
				employeeStatistics.totalHourlyEarnings += employee.payAmount[i];
				employeeStatistics.numOfHourlyEmployees++;
			} else if(employee.payFrequency[i].equals("Salary")) {
				employeeStatistics.totalSalaryEarnings += employee.payAmount[i];
				employeeStatistics.numOfSalaryEmployees++;
			}
		}
	}
	
	
	private static void printAverageWages( ) {
		employeeStatistics.employeeAverageAge = (float) employeeStatistics.sumOfAges / employee.firstName.length;
		stringBuffer.append(String.format("\nAverage age:         %12.1f\n", employeeStatistics.employeeAverageAge));
		
		employeeStatistics.averageCommissionEarnings = employeeStatistics.totalCommissionEarnings / employeeStatistics.numOfCommissionEmployees;
		stringBuffer.append(String.format("Average commission:  $%12.2f\n", employeeStatistics.averageCommissionEarnings));
		
		employeeStatistics.averageHourlyEarnings = employeeStatistics.totalHourlyEarnings / employeeStatistics.numOfHourlyEmployees;
		stringBuffer.append(String.format("Average hourly wage: $%12.2f\n", employeeStatistics.averageHourlyEarnings));
		
		employeeStatistics.averageSalaryEarnings = employeeStatistics.totalSalaryEarnings / employeeStatistics.numOfSalaryEmployees;
		stringBuffer.append(String.format("Average salary:      $%12.2f\n", employeeStatistics.averageSalaryEarnings));
	}
	
	
	private static int setBeginningIndex(int employeeIndex, int amountOfEmployees, String[ ] employeeAttributes) {
		for(;employeeIndex < employee.lastName.length; employeeIndex++) {
			if(employee.lastName[employeeIndex] == null)
				break;
			
			if(employee.lastName[employeeIndex].compareTo(employeeAttributes[1]) > 0) {
				for(int i = amountOfEmployees; i > employeeIndex; i--) {
					employee.firstName[i] = employee.firstName[i - 1];
					employee.lastName[i] = employee.lastName[i - 1];
					employee.age[i] = employee.age[i - 1];
					employee.payFrequency[i] = employee.payFrequency[i - 1];
					employee.payAmount[i] = employee.payAmount[i - 1];
				}
				break;
			}
		}
		return employeeIndex;
	}
	
	
	private static int countEmployeesInFile(Scanner employeeFile) {
		int amountOfEmployeesInFile = 0;
		while(employeeFile.hasNextLine( )) {
			String employeeLineFromFile = employeeFile.nextLine( );
			if(employeeLineFromFile.length( ) > 0)
				amountOfEmployeesInFile++;
		}
		return amountOfEmployeesInFile;
	}
	
	private static Scanner fileOpen(String listOfEmployeesFile) {
		Scanner employeeFile = null;
		try {
			employeeFile = new Scanner(new File(listOfEmployeesFile));
		} catch (FileNotFoundException exception) {
			System.err.println(exception.getMessage( ));
			return null;
		}
		return employeeFile;
	}
}
