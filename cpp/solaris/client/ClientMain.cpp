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

#include "ClientMain.hpp"

#include <iostream>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

#include "util/Base64.hpp"
#include "JSONObject.hpp"
#include "JSONPacket.hpp"
#include "PublishDatagramThread.hpp"
#include "SharedData.hpp"

using namespace std;

static bool shutdownRequested = false;

#ifdef WIN32
#include <windows.h> 

BOOL WINAPI consoleHandlerClient(DWORD signal) 
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

ClientMain::ClientMain() 
{

    SharedData::getInstance()->appConfig.loadXml("config.xml");
}

int main(int argc, char* argv[]) 
{

#ifdef WIN32
    SetConsoleCtrlHandler(consoleHandlerClient, TRUE);
#elif UNIX
    signal(SIGINT, sigint_handler);
#endif

    ClientMain ClientMain;
    PublishDatagramThread publishDatagramThread;

    publishDatagramThread.start();

    int count = 0;

    while (count < 1) 
    {
        json::JSONObject jsonObject;
        unsigned char bytes[5] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
        string base64Bytes = base64_encode(bytes, 5);

        jsonObject.put("eventName", "penguins");
        jsonObject.put("chatham", (int8_t)123);
        jsonObject.put("waitaha", (short)12345);
        jsonObject.put("king", (int) 123456789);
        jsonObject.put("emperor", (int64_t) 123456789123456789);
        jsonObject.put("chinstrap", (float) 123.456);
        jsonObject.put("gentoo", (double) 123456789.123456789);
        jsonObject.put("magellanic", (bool) true);
        jsonObject.put("humboldt", "This is a string");
        jsonObject.put("macaroni", base64Bytes);
        
        publishDatagramThread.sendJSONString(jsonObject.toString());
        count++;
    }

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    publishDatagramThread.shutdownReq();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    return 0;
}
