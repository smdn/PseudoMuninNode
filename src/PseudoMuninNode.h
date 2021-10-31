// 
// Copyright (c) 2019 smdn (https://smdn.jp/)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PseudoMuninNode_h
#define PseudoMuninNode_h

#include <Arduino.h>
#include <WiFi.h>

#include "PseudoMuninPlugin.h"

class PseudoMuninNode {
  public:
    static constexpr const char* VERSION = "1.0.2";

    static constexpr uint32_t DEFAULT_TIMEOUT_MILLISECONDS = 32 * 1000;
    static constexpr const char* DEFAULT_HOSTNAME = "espressif32.localdomain";
    static constexpr uint16_t DEFAULT_PORT = 4949;

    PseudoMuninNode(
      PseudoMuninPlugin* const plugins,
      size_t numberOfPlugins,
      uint8_t maxClients = 1,
      uint32_t timeoutMilliseconds = DEFAULT_TIMEOUT_MILLISECONDS,
      const char* hostname = DEFAULT_HOSTNAME,
      uint16_t port = DEFAULT_PORT
    ) :
      m_server(port, maxClients),
      m_pluginList(plugins),
      m_numberOfPlugins(numberOfPlugins),
      m_timeoutMilliseconds(timeoutMilliseconds),
      m_hostname(hostname)
    {};

    void begin();
    void acceptClient();

  private:
    static bool expectCommand(const String& response, const String& command, String& arguments);

    void writeResponse(const String& response, WiFiClient& client);
    void processCommand(WiFiClient& client, const String& command);
    void processCommandNodes(WiFiClient& client, const String& command);
    void processCommandCap(WiFiClient& client, const String& command, const String& arguments);
    void processCommandVersion(WiFiClient& client, const String& command);
    void processCommandList(WiFiClient& client, const String& command, const String& arguments);
    void processCommandFetch(WiFiClient& client, const String& command, const String& arguments);
    void processCommandConfig(WiFiClient& client, const String& command, const String& arguments);

    WiFiServer m_server;
    PseudoMuninPlugin* const m_pluginList;
    const size_t m_numberOfPlugins;
    const uint32_t m_timeoutMilliseconds;
    const String m_hostname;
};

#endif // PseudoMuninNode_h
