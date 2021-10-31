// 
// Copyright (c) 2021 smdn (https://smdn.jp/)
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



// This example shows how to report UNKNOWN/"U" plugin field value to munin.
// If you set `true` to PseudoMuninPluginField.isValueUnknown, PseudoMuninNode will report UNKNOWN as a field value.

#include <WiFi.h>
#include <PseudoMuninNode.h>

#define NUMBER_OF_ROOMS 2

// field definitions of munin plugin
PseudoMuninPluginField fields_room_temperature[] = {
  {
    .name = "room0",
  },
  {
    .name = "room1",
  },
};

// definitions of munin plugin
PseudoMuninPlugin plugins[] = {
  {
    .name = "temperature",
    // graph args (see http://guide.munin-monitoring.org/en/latest/example/graph/graph_args.html)
    .config = R"(graph_title Room temperatures
graph_category sensors
graph_args --base 1000 --lower-limit -10 --upper-limit 30
graph_scale no
graph_vlabel Temperature [deg C]
room0.label Room #0
room0.draw LINE
room1.label Room #1
room1.draw LINE)",
    .fields = fields_room_temperature,
    .numberOfFields = NUMBER_OF_ROOMS,
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

// A function to read the temperature from the sensor
bool getRoomTemperature(const int& roomNumber, double& temprature)
{
  // assumes that the sensor value is retrieved here
  temprature = (double)(roomNumber * 10 + random(10));

  // assumes that the retrieving sensor value fails randomly
  if (0 == random(3)) {
    return false;
  }
  else {
    return true;
  }
}

void loop()
{
  for (auto i = 0; i < NUMBER_OF_ROOMS; i++) {
    // reset the UNKNOWN state of the field value
    fields_room_temperature[i].isValueUnknown = false;

    double currentTemperature;

    // retrieve the sensor value and update the field value
    if (getRoomTemperature(i, currentTemperature)) {
      // if a new value can be retrieved from the sensor, set that value
      fields_room_temperature[i].value = currentTemperature;
    }
    else {
      // if a value cannot be retrieved from the sensor, set to UNKNOWN state
      fields_room_temperature[i].isValueUnknown = true;
    }
  }

  // wait a request from munin-update; respond commands and return values of field
  node.acceptClient();

  // write your own code here
  delay(100);
}
