/**                            COMMENT BETTER
 * Created with IntelliJ IDEA.
 * User: Brandon
 * Date: 3/25/14
 * Time: 5:01 PM
 * To change this template use File | Settings | File Templates.
 */
public class Matrix {
    private int height, width;
    private Node start, leftEnd, onLeft, above, temp;

    public Matrix(int h, int w){
        height = h;
        width = w;
        if(height<=0 || width<=0){
            System.out.println("Invalid height or width (must be greater than 0)");
            return;
        }
        else if(height>10 || width>10){
            System.out.println("Max size allowed: h = 10, w = 10");
            return;
        }
        else{
            for(int j=0; j<height; j++){
                for(int i=0; i<width; i++){
                    String x = Integer.toString(j); //formatting
                    String y = Integer.toString(i);
                    String xy = String.format("%s%s", x, y);  //use coordinate system as Node contents
                    Node newNode = new Node(xy);
                    if(j==0 && i==0){ //for first node in first row
                        start = onLeft = leftEnd = newNode; //place all pointers towards node
                        onLeft.right = null; //pointers to right == null
                    }
                    else if(j==0 && i>0){ //for each node in the first row
                        onLeft.right = newNode;
                        onLeft = newNode;
                        onLeft.right = null;
                    }
                    else if(j>0 && i==0){ //for first node in following rows
                        above = leftEnd;
                        leftEnd.down = newNode;
                        leftEnd = newNode;
                        onLeft = leftEnd;
                        onLeft.right = null;
                    }
                    else{ //for any middle nodes
                        above = above.right;     //advance right
                        onLeft.right = newNode;  //apply right pointer to new node
                        above.down = newNode;   //apply down pointer to new node
                        onLeft = newNode;   //advance onLeft
                        onLeft.right = null;
                    }
                }//end inner for
            }//end outer for
        }//end else
    }//end constructor

    public void insertMatrix(int x, int column, int row){
        System.out.println(x +" is being inserted at ("+ column +", "+ row +")");   //3, 4
        String n = Integer.toString(x);
        leftEnd = onLeft = start; //reset useful variables to start for use in method
        for(int j=0;j<height;j++){
            for(int i=0;i<width;i++){
                if(j==column && i==row){
                    onLeft.iData = n;
                    break;
                }
                onLeft = onLeft.right;
            }//end inner for
            leftEnd = leftEnd.down;
            onLeft = leftEnd;
        }//end outer for
    }//end insertMatrix

    public void displayMatrix(){
        System.out.println("Matrix("+ height +", "+ width +"): ");
        leftEnd = onLeft = start; //reset useful variables to start for use in method
        for(int j=0; j<height; j++){
            for(int i=0; i<width; i++){
                onLeft.displayNode();
                onLeft = onLeft.right;
            }//end inner for
            System.out.println("");
            leftEnd = leftEnd.down;
            onLeft = leftEnd;
        }//end outer for
    }//end displayMatrix()

    public void sumMatrix(){
        temp = leftEnd = onLeft = start;
        int rowSum = 0;
        for(int j=0; j<height; j++){
            for(int i=0; i<width; i++){
                rowSum += Integer.parseInt(onLeft.iData);
                onLeft.displayNode();
                onLeft = onLeft.right;
                if(i==width-1){
                    System.out.print(" "+ rowSum);
                    rowSum = 0;
                }
            }//end inner for
            System.out.println("");
            leftEnd = leftEnd.down;
            onLeft = leftEnd;
        }//end outer for
        onLeft = start;
        int colSum = 0;
        for(int i=0; i<width; i++){
            for(int j=0; j<height; j++){
                colSum += Integer.parseInt(temp.iData);
                temp = temp.down;
                if(j==height-1){
                    System.out.print(colSum + " ");
                    colSum = 0;
                }
            }//end inner for
            onLeft = onLeft.right;
            temp = onLeft;
        }
    }

    public void fillMatrix(int x){
        System.out.println("Matrix is being filled with: "+ x);
        String n = Integer.toString(x);
        leftEnd = onLeft = start; //reset useful variables to start for use in method
        for(int j=0; j<height; j++){
            for(int i=0; i<width; i++){
                onLeft.iData = n;
                onLeft = onLeft.right;
            }
            leftEnd = leftEnd.down;
            onLeft = leftEnd;
        }
    }
}