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

#include "ServerMain.hpp"

#include <iostream>
#include <boost/thread.hpp>

#include "util/Base64.hpp"
#include "JSONObject.hpp"
#include "SharedData.hpp"
#include "ReceiveDatagramThread.hpp"

static bool shutdownRequested = false;

#ifdef WIN32
#include <windows.h> 

BOOL WINAPI consoleHandlerServer(DWORD signal) 
{

    if (signal == CTRL_C_EVENT)
        shutdownRequested = true;

    return TRUE;
}
#elif UNIX

void sigint_handler(int sig) {

    shutdownRequested = true;
}
#endif

ServerMain::ServerMain() 
{

    SharedData::getInstance()->appConfig.loadXml("config.xml");
}

int main1(int argc, char* argv[]) 
{

#ifdef WIN32
    SetConsoleCtrlHandler(consoleHandlerServer, TRUE);
#elif UNIX
    signal(SIGINT, sigint_handler);
#endif

    ServerMain serverMain;
    ReceiveDatagramThread receivedDatagramThread;

    receivedDatagramThread.start();

    while (!shutdownRequested) {

        vector<JSONPacket> packetList;

        receivedDatagramThread.drainTo(packetList);

        cout << "received: " << packetList.size() << endl;

        for (JSONPacket packet : packetList) {

            cout << "contents: " << packet.data << endl;

            json::JSONObject jsonObject(packet.data);

            cout << "eventName = " << jsonObject.getString("eventName") << endl;
            cout << "chatham = " << jsonObject.getByte("chatham") << endl;
            cout << "waitaha = " << jsonObject.getShort("waitaha") << endl;
            cout << "king = " << jsonObject.getInt("king") << endl;
            cout << "emperor = " << jsonObject.getLong("emperor") << endl;
            cout << "chinstrap = " << jsonObject.getFloat("chinstrap") << endl;
            cout << "gentoo = " << jsonObject.getDouble("gentoo") << endl;
            cout << "magellanic = " << jsonObject.getBoolean("magellanic") << endl;
            cout << "humboldt = " << jsonObject.getString("humboldt") << endl;

            string decodedBytes = base64_decode(jsonObject.getString("macaroni"));
            cout << "macaroni = " << decodedBytes << endl << endl;
        }

        boost::this_thread::sleep(boost::posix_time::seconds(5));
    }

    cout << "Shutting down ..." << std::endl;
    receivedDatagramThread.shutdownReq();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    return 0;
}
