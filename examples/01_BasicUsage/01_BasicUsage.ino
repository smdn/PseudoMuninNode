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

#include <WiFi.h>
#include <PseudoMuninNode.h>

// field definitions of munin plugin
PseudoMuninPluginField fields_uptime[] = {
  {
    .name = "uptime",
  },
};

// definitions of munin plugin
PseudoMuninPlugin plugins[] = {
  {
    .name = "uptime",
    // graph args (see http://guide.munin-monitoring.org/en/latest/example/graph/graph_args.html)
    .config = R"(graph_title Uptime
graph_category system
graph_args --base 1000 --lower-limit 0
graph_scale no
graph_vlabel uptime in milliseconds
uptime.label uptime [ms]
uptime.draw AREA)",
    .fields = fields_uptime,
    .numberOfFields = 1,
  },
};

// pseudo munin-node instance
PseudoMuninNode node = {
  .plugins = plugins,
  .numberOfPlugins = 1
};

constexpr char WIFI_SSID[] = "your_wifi_ssid";
constexpr char WIFI_PASSPHRASE[] = "your_wifi_passphrase";

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  // setup WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);

  while (WiFi.status() != WL_CONNECTED)
    delay(100);

  // begin pseudo munin-node
  node.begin();
}

void loop()
{
  // update the value of 'uptime' field
  fields_uptime[0].value = millis();

  // wait a request from munin-update; respond commands and return values of field
  node.acceptClient();

  // write your own code here
  delay(100);
}