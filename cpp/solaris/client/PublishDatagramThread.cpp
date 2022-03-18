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

#include "PublishDatagramThread.hpp"
#include "Sender.hpp"
#include "SharedData.hpp"

#include <iostream>

void PublishDatagramThread::start() 
{

    shutdown = false;

    publishThread = new boost::thread(
        boost::bind(&PublishDatagramThread::runPublishThread, this));
}

void PublishDatagramThread::sendJSONString(string jsonString) 
{

    JSONPacket packet;

    strcpy(packet.data, jsonString.c_str());
    packet.length = strlen(jsonString.c_str());

    queue.push(packet);
}

void PublishDatagramThread::runPublishThread() 
{

    Sender sender(io_service_p,
        boost::asio::ip::address::from_string(
            SharedData::getInstance()->appConfig.getMulticastAddress()),
            SharedData::getInstance()->appConfig.getMulticastPort());

    io_service_p.run();

    while (!shutdown) {

        int queueSize = queue.size();

        for (int i = 0; i < queueSize; i++)
        {
            JSONPacket packet;

            queue.front(packet);
            sender.sendTo(packet.data);
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void PublishDatagramThread::shutdownReq() 
{

    shutdown = true;
}
