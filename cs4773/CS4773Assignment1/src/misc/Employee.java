package misc;

/**
 * 
 * @author nick simmons (srz888)
 * This class initializes an employee class to correct values.
 *
 */
public class Employee {
	String [] firstName;
	String [] lastName;
	int [] age;
	String [] payFrequency;
	double [] payAmount;
	
	Employee(int totalNumberOfEmployees) {
		this.firstName = new String[totalNumberOfEmployees];
		this.lastName = new String[totalNumberOfEmployees];
		this.age = new int[totalNumberOfEmployees];
		this.payFrequency = new String[totalNumberOfEmployees];
		this.payAmount = new double[totalNumberOfEmployees];
	}
}
