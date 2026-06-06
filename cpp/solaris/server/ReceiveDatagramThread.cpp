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
    try
    {
        // Setup the I/O context required by all Asio programs
        boost::asio::io_context io_ctx;

        // Define receive port
        const unsigned short port =
            SharedData::getInstance()->appConfig.getMulticastPort();

        // Create the addresses
        boost::asio::ip::address_v4 interface_address =
            boost::asio::ip::address_v4::from_string(
                SharedData::getInstance()->appConfig.getMulticastDeviceAddress()
            );

        boost::asio::ip::address_v4 multicast_address =
            boost::asio::ip::address_v4::from_string(
                SharedData::getInstance()->appConfig.getMulticastAddress()
            );

        // Create local listen endpoint.
        // For multicast receivers, bind to 0.0.0.0:port or the interface address.
        boost::asio::ip::udp::endpoint listen_endpoint(
            boost::asio::ip::udp::v4(),
            port
        );

        // Open the IPv4 UDP socket
        boost::asio::ip::udp::socket socket(io_ctx);
        socketPtr = &socket;

        socket.open(boost::asio::ip::udp::v4());

        // Allow multiple apps/sockets to bind to the same multicast port
        socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));

        // Bind socket to the multicast receive port
        socket.bind(listen_endpoint);

        // Join the multicast group on the specific network interface
        socket.set_option(
            boost::asio::ip::multicast::join_group(
                multicast_address,
                interface_address
            )
        );

        std::array<unsigned char, 1500> receiveBuffer;

        while (!shutdown) {

                boost::asio::ip::udp::endpoint senderEndpoint;
                boost::system::error_code ec;

                std::size_t bytesReceived =
                    socket.receive_from(
                        boost::asio::buffer(receiveBuffer),
                        senderEndpoint,
                        0,
                        ec
                    );

                if (shutdown)
                    break;

                if (ec)
                {
                    if (ec == boost::asio::error::operation_aborted)
                        break;

                    std::cerr << "Multicast receive error: "
                        << ec.message()
                        << std::endl;

                    continue;
                }

                std::vector<unsigned char> data(
                    receiveBuffer.begin(),
                    receiveBuffer.begin() + bytesReceived
                );

                handleReceivedData(data, data.size());
            }

            boost::system::error_code ec;
            socket.close(ec);
            socketPtr = nullptr;
        }
    catch (const std::exception& ex)
    {
        std::cerr << "ServerReceiveSocketThread exception: "
            << ex.what()
            << std::endl;
    }
}


void ReceiveDatagramThread::handleReceivedData(
    const std::vector<unsigned char>&data,
    const int numBytes)
{

    JSONPacket packet;

    memcpy(packet.data, data.data(), numBytes);
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
