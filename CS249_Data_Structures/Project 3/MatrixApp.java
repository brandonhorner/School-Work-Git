import java.util.Scanner;

/**
 * Created by Brandon on 3/27/14.
 * This class asks the user to create a new matrix, and offers
 * methods to manipulate the matrix.
 */
public class MatrixApp {
    public static void main(String[] args) {

        System.out.print("Please enter the size of your desired matrix. \n" + "Width: ");
        Scanner scan = new Scanner(System.in);
        int i = scan.nextInt();
        System.out.print("Height: ");
        int j = scan.nextInt();
        Matrix matrix = new Matrix(i, j);

        boolean flag = true;
        while(flag){  //run until user wants to stop
            System.out.print("\nPlease enter; \n" +
                    "\"1\" to insert a specific integer at a specific location.\n" +
                    "\"2\" to fill the entire matrix with a given integer value.\n" +
                    "\"3\" to ask for the sum of rows and columns.\n" +
                    "\"4\" to display the matrix.\n" +
                    "\"5\" to exit menu.\n" +
                    "Enter a number: ");
            int n = scan.nextInt();

            if(n==1){  //if the user wants to insert a number into the matrix
                System.out.println("Please enter an integer to be inserted: ");
                int x = scan.nextInt();
                System.out.println("X-pos: ");
                i = scan.nextInt();
                System.out.println("Y-pos: ");
                j = scan.nextInt();
                matrix.insertMatrix(x, i, j);
            }
            if(n==2){  //if the user wants to fill entire matrix with integer
                System.out.println("Please enter an integer to fill matrix with.");
                int x = scan.nextInt();
                System.out.println("");
                matrix.fillMatrix(x);
            }
            if(n==3){ //if the user wants to know the sum of the rows and columns
                System.out.println("Here is the sum of rows and columns:");
                System.out.println("");
                matrix.sumMatrix();
            }
            if(n==4){ //if the user wants to display the matrix
                System.out.println("");
                matrix.displayMatrix();
            }
            if(n==5){ //if the user wants to exit the program
                flag = false;
                System.out.print("\nExiting...");
            }
        }
    }
}
