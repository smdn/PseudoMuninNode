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

PseudoMuninPluginField fields_uptime[] = {
  {
    .name = "uptime",
    .fetch = [](PseudoMuninPluginField* f) { f->value = millis(); }
  },
};

PseudoMuninPluginField fields_sensors[] = {
  // draw the sensor values of A3-A5 pins on one graph
  {
    .name = "pin-A3",
    .fetch = [](PseudoMuninPluginField* f) { f->value = analogRead(A3); }
  },
  {
    .name = "pin-A4",
    .fetch = [](PseudoMuninPluginField* f) { f->value = analogRead(A4); }
  },
  {
    .name = "pin-A5",
    .fetch = [](PseudoMuninPluginField* f) { f->value = analogRead(A5); }
  },
};

// draw the 2 graphs 'uptime' and 'sensors'
PseudoMuninPlugin plugins[] = {
  {
    .name = "uptime",
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
  {
    .name = "sensors",
    .config = R"(graph_title Analog Input Sensor
graph_category sensors
graph_args --base 1000 --lower-limit 0
graph_scale no
graph_vlabel sensor value
pin-A3.label A3
pin-A4.label A4
pin-A5.label A5)",
    .fields = fields_sensors,
    .numberOfFields = 3,
  },
};

PseudoMuninNode node = {
  .plugins = plugins,
  .numberOfPlugins = 2
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
  node.acceptClient();

  // write your own code here
  delay(100);
}