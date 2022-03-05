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

import java.beans.PropertyChangeSupport;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class ReceiveDatagramThread extends Thread {

	private PropertyChangeSupport propertyChangeSupport;
	private boolean shutdown = false;
	private String multicastIP = "";
	private int multicastPort = 0;

	public ReceiveDatagramThread(String ip, int port) {
		
		propertyChangeSupport = new PropertyChangeSupport(this);
		multicastIP = ip;
		multicastPort = port;
	}

	public PropertyChangeSupport getPropertyChangeSupport() {
		
		return propertyChangeSupport;
	}

	private void receiveUDPMessage() throws IOException {

		byte[] buffer = new byte[1024];
		MulticastSocket socket = new MulticastSocket(multicastPort);
		InetAddress group = InetAddress.getByName(multicastIP);
		socket.joinGroup(group);

		while (!shutdown) {

			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			socket.receive(packet);

			propertyChangeSupport.firePropertyChange("datagramReceived", 0, packet);
		}

		socket.leaveGroup(group);
		socket.close();
	}

	public void shutdownReq() {
		
		shutdown = true;
	}

	@Override
	public void run() {
		
		try {
			receiveUDPMessage();
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}