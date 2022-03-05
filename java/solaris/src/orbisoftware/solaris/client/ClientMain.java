/*
	MIT License
	
	Copyright (c) Harlan Murphy
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

package orbisoftware.solaris.client;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Base64;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.json.JSONObject;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import orbisoftware.solaris.server.SharedData;

public class ClientMain {

	private DatagramSocket socket;
	private String multicastIP;
	private int multicastPort;
	private InetAddress group;
	private String fileName = "settings.xml";

	public ClientMain() {

		try {
			// Process XML
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(fileName);
			Element rootElem = doc.getDocumentElement();

			if (rootElem != null) {
				parseElements(rootElem);
			}

			// Initiate socket
			socket = new DatagramSocket();
			multicastIP = SharedData.getInstance().xmlMap.get("MulticastIP");
			multicastPort = Integer.parseInt(SharedData.getInstance().xmlMap.get("MulticastPort"));
			group = InetAddress.getByName(multicastIP);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void sendUDPMessage(String message) {

		try {
			byte[] msg = message.getBytes();
			DatagramPacket packet = new DatagramPacket(msg, msg.length, group, multicastPort);

			socket.send(packet);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void parseElements(Element root) {

		String name = "";

		if (root != null) {

			NodeList nl = root.getChildNodes();

			if (nl != null) {

				for (int i = 0; i < nl.getLength(); i++) {
					Node node = nl.item(i);

					if (node.getNodeName().equalsIgnoreCase("setting")) {

						NodeList childNodes = node.getChildNodes();

						for (int j = 0; j < childNodes.getLength(); j++) {

							Node child = childNodes.item(j);

							if (child.getNodeName().equalsIgnoreCase("name"))
								name = child.getTextContent();
							else if (child.getNodeName().equalsIgnoreCase("value"))
								SharedData.getInstance().xmlMap.put(name, child.getTextContent());
						}
					}
				}
			}
		}
	}

	public static void main(String[] args) {

		ClientMain clientMain = new ClientMain();
		int count = 0;

		while (count < 1) {

			JSONObject jsonObject = new JSONObject();

			byte[] bytes = new byte[] { (byte) 0x48, (byte) 0x65, (byte) 0x6c, (byte) 0x6c, (byte) 0x6f };
			String base64Bytes = Base64.getEncoder().encodeToString(bytes);

			jsonObject.put("eventName", "penguins");
			jsonObject.put("chatham", new Byte((byte) 123));
			jsonObject.put("waitaha", new Short((short) 12345));
			jsonObject.put("king", new Integer(123456789));
			jsonObject.put("emperor", new Long(123456789123456789L));
			jsonObject.put("chinstrap", new Float(123.456));
			jsonObject.put("gentoo", new Double(123456789.123456789));
			jsonObject.put("magellanic", new Boolean(true));
			jsonObject.put("humboldt", "This is a string");
			jsonObject.put("macaroni", base64Bytes);

			clientMain.sendUDPMessage(jsonObject.toString());

			count++;
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		clientMain.socket.close();
	}
}