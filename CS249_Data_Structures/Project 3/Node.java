/**
 * Created with IntelliJ IDEA.
 * User: Brandon
 * Date: 11/4/13
 * Time: 4:56 PM
 * To change this template use File | Settings | File Templates.
 */

public class Node {
    public String iData; //data field
    public Node right, down;  //right and down pointer

    public Node(String id){
        iData = id;
        right = null;
        down = null;
    }//end constructor

    public void displayNode(){
        System.out.print(iData + " ");
    }//end displayNode
}//end Node
