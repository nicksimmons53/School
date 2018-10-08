package misc;

/**
 * 
 * @author nick simmons (srz888)
 * This class initializes a statistics module and allows RecordProcessor to modify the 
 * data being read from a file.
 */
public class Statistics {
	int sumOfAges;
	float employeeAverageAge;
	int numOfCommissionEmployees;
	double totalCommissionEarnings;
	double averageCommissionEarnings;
	int numOfHourlyEmployees;
	double totalHourlyEarnings;
	double averageHourlyEarnings;
	int numOfSalaryEmployees;
	double totalSalaryEarnings;
	double averageSalaryEarnings;
	
	Statistics( ) {
		this.sumOfAges = 0;
		this.employeeAverageAge = 0f;
		this.numOfCommissionEmployees = 0;
		this.totalCommissionEarnings = 0;
		this.averageCommissionEarnings = 0;
		this.numOfHourlyEmployees = 0;
		this.totalHourlyEarnings = 0;
		this.averageHourlyEarnings = 0;
		this.numOfSalaryEmployees = 0;
		this.totalSalaryEarnings = 0;
		this.averageSalaryEarnings = 0;
	}
}
