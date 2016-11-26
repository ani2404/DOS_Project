import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import java.io.PrintWriter;
import java.lang.String;

class StackImpl
{
  private int top;
  private String[] SArray;
  public StackImpl()
  {
    top=-1;
    SArray=new String[100];
    
  }
  public void push(String s)
  {
    SArray[++top]=s;
    
  }
  public String pop()
  {
    return SArray[top--];
    
  }
  public String[] ReturnParent()
  {
    String[] temp=new String[top];
    for (int i=0;i<=top;i++)
    {
      temp[i]=Sarray[i];
    }
    return temp;
  }
  
  
}
public class Xmlreader {
  

  public static void main(String[] args) {
public StackImpl SI=new StackImpl();
    try {
      

	File file = new File("./DDL_sample.xml");
  
	DocumentBuilder dBuilder = DocumentBuilderFactory.newInstance()
                             .newDocumentBuilder();

	Document doc = dBuilder.parse(file);

	System.out.println("Root element :" + doc.getDocumentElement().getNodeName());
  SI.push(doc.getDocumentElement().getNodeName());

	if (doc.hasChildNodes()) {

		printNote(doc.getChildNodes());

	}

    } catch (Exception e) {
	System.out.println(e.getMessage());
    }

  }

  private static void printNote(NodeList nodeList) {
    
    
   
System.out.println("\nNode Name =" + tempNode.getNodeName() + " [OPEN]");
    //push in parent
    SI.push(tempNode.getNodeName());
    for (int count = 0; count < nodeList.getLength();count++) {

	Node tempNode = nodeList.item(count);

	// make sure it's element node.
	if (tempNode.getNodeType() == Node.ELEMENT_NODE) {

    
		if (tempNode.hasChildNodes()) {

			// loop again if has child nodes
			printNote(tempNode.getChildNodes());

		}
  if(tempNode.getNodeName().equals("Id")){
    System.out.println("\nNode Name =" + tempNode.getNodeName() + " [OPEN]");   
		System.out.println("Node Value =" + tempNode.getTextContent());
    
	}

		System.out.println("Node Name =" + tempNode.getNodeName() + " [CLOSE]");
   //pull out parent
    SI.pop(tempNode.getNodeName());

	}
  

    }

  }

}


