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

#include "ReceiveDatagramThread.hpp"
#include "Receiver.hpp"
#include "SharedData.hpp"

#include <iostream>

void ReceiveDatagramThread::start() 
{

    shutdown = false;

    receiveThread = new boost::thread(
        boost::bind(&ReceiveDatagramThread::runReceiveThread, this));
}

void ReceiveDatagramThread::runReceiveThread() 
{

    Receiver receiver(io_service_r,
        boost::asio::ip::address::from_string("0.0.0.0"),
        boost::asio::ip::address::from_string(
            SharedData::getInstance()->appConfig.getMulticastAddress()),
            SharedData::getInstance()->appConfig.getMulticastPort());
    receiver.setCallback(
        boost::bind(&ReceiveDatagramThread::processRcvCallback, this, _1, _2));
    io_service_r.run();

    while (!shutdown) {

        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
}

void ReceiveDatagramThread::processRcvCallback(char* data, int numBytes) 
{

    JSONPacket packet;
    memcpy(packet.data, data, numBytes);
    packet.data[numBytes] = 0;
    packet.length = numBytes+1;

    queue.push(packet);
}

void ReceiveDatagramThread::drainTo(vector<JSONPacket>& packetList) 
{

    int queueSize = queue.size();
    JSONPacket packet;

    for (int i=0; i<queueSize; i++)
    {
        queue.front(packet);
        packetList.push_back(packet);
    }
}

void ReceiveDatagramThread::ReceiveDatagramThread::shutdownReq() 
{

    shutdown = true;
}
