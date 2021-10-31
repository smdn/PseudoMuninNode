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

#include <Arduino.h>
#include "PseudoMuninNode.h"

//#define DEBUG

void PseudoMuninNode::begin()
{
  m_server.begin();
}

void PseudoMuninNode::writeResponse(const String& response, WiFiClient& client)
{
#ifdef DEBUG
  Serial.print("[");
  Serial.print(m_hostname);
  Serial.print("]: ");
  Serial.println(response);
#endif

  client.print(response);
  client.print("\n"); // Munin expects <LF>, not <CR><LF>
}

void PseudoMuninNode::acceptClient()
{
  auto client = m_server.available();

  if (!client)
    return;

  auto now = millis();
  auto connectionTimeOutAt = (UINT32_MAX - m_timeoutMilliseconds <= now) ? UINT32_MAX : now + m_timeoutMilliseconds;

#ifdef DEBUG
  auto logPrefixClient = "[" + client.remoteIP(client.fd()).toString() + ":" + (String)client.remotePort(client.fd()) + "]: ";

  Serial.print(logPrefixClient); Serial.println("<accepted>");
#endif

  writeResponse((String)"# munin node at " + m_hostname, client);

  while (client.connected()) {
    if (connectionTimeOutAt <= millis()) {
      // timed out
      client.stop();
      break;
    }

    if (!client.available())
      continue;

    now = millis();
    connectionTimeOutAt = (UINT32_MAX - m_timeoutMilliseconds <= now) ? UINT32_MAX : now + m_timeoutMilliseconds;

    auto command = client.readStringUntil('\n');

    command.trim();

#ifdef DEBUG
    Serial.print(logPrefixClient); Serial.println(command);
#endif

    processCommand(client, command);
  }

#ifdef DEBUG
  Serial.print(logPrefixClient); Serial.println("<disconnected>");
#endif
}

bool PseudoMuninNode::expectCommand(const String& response, const String& command, String& arguments)
{
  if (response.length() == command.length()) {
    // <command> <LF>
    arguments = "";
    return response.equals(command);
  }
  else if (' ' == response.charAt(command.length())) {
    // <command> <SP> <arguments> <LF>
    arguments = response.substring(command.length() + 1);
    return response.startsWith(command);
  }
  else {
    return false;
  }
}

void PseudoMuninNode::processCommand(WiFiClient& client, const String& command)
{
  // 'Data exchange between master and node'
  //   http://guide.munin-monitoring.org/en/latest/master/network-protocol.html
  String arguments;

  if (expectCommand(command, "fetch", arguments))
    processCommandFetch(client, command, arguments);
  else if (expectCommand(command, "nodes", arguments))
    processCommandNodes(client, command);
  else if (expectCommand(command, "list", arguments))
    processCommandList(client, command, arguments);
  else if (expectCommand(command, "config", arguments))
    processCommandConfig(client, command, arguments);
  else if (expectCommand(command, "quit", arguments) ||
           command.equals("."))
    client.stop();
  else if (expectCommand(command, "cap", arguments))
    processCommandCap(client, command, arguments);
  else if (expectCommand(command, "version", arguments))
    processCommandVersion(client, command);
  else
    writeResponse("# Unknown command. Try cap, list, nodes, config, fetch, version or quit", client);
}

void PseudoMuninNode::processCommandNodes(WiFiClient& client, const String& command)
{
  writeResponse(m_hostname, client);
  writeResponse(".", client);
}

void PseudoMuninNode::processCommandVersion(WiFiClient& client, const String& command)
{
  writeResponse((String)"munins node on " + m_hostname + " version: " + VERSION, client);
}

void PseudoMuninNode::processCommandCap(WiFiClient& client, const String& command, const String& arguments)
{
  // TODO: multigraph (http://guide.munin-monitoring.org/en/latest/plugin/protocol-multigraph.html)
  // TODO: dirtyconfig (http://guide.munin-monitoring.org/en/latest/plugin/protocol-dirtyconfig.html)
  // XXX: ignore capability arguments
  writeResponse("cap", client);
}

void PseudoMuninNode::processCommandList(WiFiClient& client, const String& command, const String& arguments)
{
  // XXX: ignore [node] arguments
  String resp = "";

  for (auto i = 0u; i < m_numberOfPlugins; i++) {
    if (0 < i)
      resp += " ";

    resp += (m_pluginList + i)->name;
  }

  writeResponse(resp, client);
}

void PseudoMuninNode::processCommandFetch(WiFiClient& client, const String& command, const String& arguments)
{
  for (auto p = 0u; p < m_numberOfPlugins; p++) {
    auto plugin = (m_pluginList + p);

    if (arguments.equals(plugin->name)) {
      for (auto f = 0u; f < plugin->numberOfFields; f++) {
        auto field = (plugin->fieldList + f);

        if (field->fetch)
          field->fetch(field);

        if (field->isValueUnknown)
          // reports "U"(UNKNOWN) as field value
          writeResponse(field->name + ".value U", client);
        else
          writeResponse(field->name + ".value " + field->value, client);
      }

      writeResponse(".", client);
      return;
    }
  }

  // if not found
  writeResponse("# Unknown service", client);
  writeResponse(".", client);
}

void PseudoMuninNode::processCommandConfig(WiFiClient& client, const String& command, const String& arguments)
{
  for (auto p = 0u; p < m_numberOfPlugins; p++) {
    auto plugin = (m_pluginList + p);

    if (arguments.equals(plugin->name)) {
      writeResponse(plugin->config, client);
      writeResponse(".", client);

      return;
    }
  }

  // if not found
  writeResponse("# Unknown service", client);
  writeResponse(".", client);
}
