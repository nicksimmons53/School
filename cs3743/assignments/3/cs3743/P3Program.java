package cs3743;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Date;

public class P3Program 
{
    private Connection connect = null;
    
    private Statement statement = null;
    private PreparedStatement preparedStatement = null;
    private ResultSet resultSet = null;
    public static final int ER_DUP_ENTRY = 1062;
    public static final int ER_DUP_ENTRY_WITH_KEY_NAME = 1586;
    public static final String[] strMovieIdM =
    {   "LASTSF001"
       ,"GALAXY001"
       ,"STWARS001"
       ,"END"
    };
    
    public P3Program (String user, String password) throws Exception
    {
        try
        {
            // This will load the MySQL driver, each DBMS has its own driver
            Class.forName("com.mysql.jdbc.Driver");
            this.connect = DriverManager.getConnection
                    ("jdbc:mysql://10.100.1.81:3306/abc123db"
                    , user
                    , password);
        }
        catch (Exception e) 
        {
            throw e;
        } 
        
    }
       
    public void runProgram() throws Exception 
    {
        try 
        {
            // your code
        } 
        catch (Exception e) 
        {
            throw e;
        } 
        finally 
        {
            close();
        }

    }                                                                                                                        
    
    private void printCustomers(String title, ResultSet resultSet) throws SQLException 
    {
        // custNr, name, stateCd, birthDt, gender
        System.out.printf("%s\n", title);
        // your code
    }
    

    // Close the resultSet, statement, preparedStatement, and connect
    private void close() 
    {
        try 
        {
            if (resultSet != null) 
                resultSet.close();

            if (statement != null) 
                statement.close();
            
            if (preparedStatement != null) 
                preparedStatement.close();

            if (connect != null) 
                connect.close();
        } 
        catch (Exception e) 
        {

        }
    }

}
