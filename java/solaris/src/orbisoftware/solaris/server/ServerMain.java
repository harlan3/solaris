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

package orbisoftware.solaris.server;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.util.Base64;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.json.JSONObject;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ServerMain {

	private String fileName = "settings.xml";
	private boolean shutdown = false;

	public ServerMain() {

		try {
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(fileName);
			Element rootElem = doc.getDocumentElement();

			if (rootElem != null) {
				parseElements(rootElem);
			}
		} catch (Exception e) {

			System.out.println("Exception in loadXML(): " + e.toString());
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

		ServerMain serverMain = new ServerMain();

		ProcessDatagramThread processDatagramThread = new ProcessDatagramThread();
		processDatagramThread.start();

		Runtime.getRuntime().addShutdownHook(new Thread() {
			public void run() {
				try {
					Thread.sleep(200);
					System.out.println("Shutting down ...");
					serverMain.shutdown = true;
					processDatagramThread.shutdownReq();
				} catch (InterruptedException e) {
					Thread.currentThread().interrupt();
					e.printStackTrace();
				}
			}
		});

		while (!serverMain.shutdown) {

			List<DatagramPacket> datagramPacketList = processDatagramThread.getQueuedData();

			System.out.println("received: " + datagramPacketList.size());

			for (DatagramPacket packet : datagramPacketList) {

				String json = new String(packet.getData(), packet.getOffset(), packet.getLength());
				System.out.println("contents: " + json);

				JSONObject jsonObject = new JSONObject(json);

				System.out.println("eventName = " + jsonObject.getString("eventName"));
				System.out.println("chatham = " + jsonObject.getByte("chatham"));
				System.out.println("waitaha = " + jsonObject.getShort("waitaha"));
				System.out.println("king = " + jsonObject.getInt("king"));
				System.out.println("emperor = " + jsonObject.getLong("emperor"));
				System.out.println("chinstrap = " + jsonObject.getFloat("chinstrap"));
				System.out.println("gentoo = " + jsonObject.getDouble("gentoo"));
				System.out.println("magellanic = " + jsonObject.getBoolean("magellanic"));
				System.out.println("humboldt = " + jsonObject.getString("humboldt"));

				try {
					byte[] decodedBytes = Base64.getDecoder().decode(jsonObject.getString("macaroni"));
					System.out.println("macaroni = " + new String(decodedBytes, "UTF-8"));
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
