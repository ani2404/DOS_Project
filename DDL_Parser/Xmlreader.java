import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * This class is used to parse a DDL file and finally writes back the content to
 * the driver file FILE_NAME
 * 
 * @author crystalonix
 *
 */
public class Xmlreader {

	private static final String WRITE = "write";
	// device pin
	private static final String DEVICE_TYPE = "gpio_";
	private static final String INPUT_MODE = "in";
	private static final String OUTPUT_MODE = "out";
	private static final String READ = "read";
	// name of the driver file name to be generated
	private static final String FILE_NAME = "./pio.c";
	// input directive
	private static final String INCLUDE_INPUT = "#include <ddl_io_in.inc>";
	// output directive
	private static final String INCLUDE_OUTPUT = "#include <ddl_io_out.inc>";
	// this file is used as backbone to generate the driver file
	private static final String BACKBONE_FILE_NAME = "./pio.txt";
	
	//files for input and output
	private static final String FILE_NAME_IN = "./ddl_io_in.inc";
	private static final String FILE_NAME_OUT = "./ddl_io_out.inc";
	

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		List<device> devices = new ArrayList<>();
		try {
			File inputFile = new File("DDL_sample.xml");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(inputFile);
			doc.getDocumentElement().normalize();
			 System.out.println("Root element :" +
			 doc.getDocumentElement().getNodeName());
			NodeList nList = doc.getElementsByTagName("Device");
			System.out.println("Debug point 1");
			for (int i = 0; i < nList.getLength(); i++) {
				System.out.println("Debug point 2");
				Node n = nList.item(i);
				device dev = new device();
				dev.setId((((Element) (n)).getElementsByTagName("Id").item(0).getTextContent()));
				
				//dev.setName((((Element) (n)).getElementsByTagName("name").item(0).getTextContent()));
				if (n.getNodeType() == Node.ELEMENT_NODE) {
					
					Element devElem = (Element) n;
					NodeList pns = devElem.getElementsByTagName("Topic");
					System.out.println(pns.item(0).getNodeName());
					for (int j = 0; j < pns.getLength(); j++) {
						
						topic p = new topic();
						p.setMode(((Element) (pns.item(j))).getElementsByTagName("Mode").item(0).getTextContent());
						dev.addTopic(p);
					}
				}
				devices.add(dev);
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("error occurred" + e);
		}
		StringBuilder outWrite = new StringBuilder();
		StringBuilder InWrite = new StringBuilder();
		
		StringBuilder inRead = new StringBuilder();
		for (int i = 0; i < devices.size(); i++) {
			List<topic> t = devices.get(i).topics;
			for (int j = 0; j < t.size(); j++) {
				if (t.get(j).getMode().equals(OUTPUT_MODE)) {
					System.out.println("Debug i");
					outWrite.append("case " + devices.get(i).id + ":\n");
					outWrite.append(DEVICE_TYPE + WRITE + "(" + devices.get(i).id + ",data" + ")\n");
				} else if (t.get(j).getMode().equals(INPUT_MODE)) {
					System.out.println("Debug j");
					InWrite.append("case " + devices.get(i).id + ":\n");
					InWrite.append(DEVICE_TYPE + READ + "(" + devices.get(i).id + ",data" + ")\n");
				}
			}
		}
		/*InWrite.deleteCharAt(InWrite.length()-1);
		outWrite.deleteCharAt(outWrite.length()-1);
		*/

		BufferedWriter outputWriterin = null;
		BufferedWriter outputWriterout = null;

		// creating the output file if not already exists
		File filein = new File(FILE_NAME_IN);
		File fileout=new File(FILE_NAME_OUT);

		if (!filein.exists()) {
			try {
				filein.createNewFile();
			} catch (IOException e) {
				System.out.println("Output file could not be created file in");
				return;
			}
		}
		
		if (!fileout.exists()) {
			try {
				fileout.createNewFile();
			} catch (IOException e) {
				System.out.println("Output file could not be created file out");
				return;
			}
		}
		
		
		/**
		 * Preparing the output stream
		 */
		FileWriter fwin,fwout;
		try {
			fwin = new FileWriter(filein);
			fwout=new FileWriter(fileout);
		} catch (IOException e1) {
			System.out.println("FileWriter could not be opened:" + e1);
			return;
		}
		outputWriterin = new BufferedWriter(fwin);
		outputWriterout = new BufferedWriter(fwout);
		try {
			outputWriterin.write(InWrite.toString());
			outputWriterout.write(outWrite.toString());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		finally {
			try {
				outputWriterin.close();
				outputWriterout.close();
			} catch (IOException e) {
				System.out.println("Error trying to close the file writer:");
			}
		}
	}
}

class device {
	String name;
	String id;
	List<topic> topics = new ArrayList<>();

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
		System.out.println(":Name:"+name);
	}

	public void addTopic(topic t) {
		topics.add(t);
	}
	public void setId(String id) {
		this.id=id;
		System.out.println(":ID:"+this.id);
		
	}
	public String getId() {
		return id;
	}
}

class topic {
	String mode;
	
	

	

	public String getMode() {
		return mode;
	}

	public void setMode(String mode) {
		this.mode = mode;
	}

	
}
