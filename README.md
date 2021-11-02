[![GitHub license](https://img.shields.io/github/license/smdn/PseudoMuninNode)](https://github.com/smdn/PseudoMuninNode/blob/main/LICENSE.txt)

PseudoMuninNode
===========
PseudoMuninNode is a simple library for running [munin-node](http://guide.munin-monitoring.org/en/latest/architecture/index.html) compatible server on ESP32.

By behaviouring ESP32 as a munin-node, various telemetry such as sensor values can be aggregated directly by [Munin](http://munin-monitoring.org/). 
PseudoMuninNode makes it unnecessary to prepare a separate server to run munin-node, such as a PC or Raspberry Pi.

![architecture](https://github.com/smdn/PseudoMuninNode/blob/images/readme/architecture.png)

# Example code
The code below is a snippet that makes the PseudoMuninNode work.

```cpp
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
```

[Full code](/examples/01_BasicUsage/01_BasicUsage.ino) and more usage sketches are available in [examples](/examples) directory.


# Example of output
The following graph shows the results of three different temperature and humidity sensors (DHT11, AM2320, and BME280) connected to ESP32 and aggregating their measurement data with PseudoMuninNode.

![temperature graph](https://github.com/smdn/PseudoMuninNode/blob/images/readme/graph-humidity-day.png)
![humidity graph](https://github.com/smdn/PseudoMuninNode/blob/images/readme/graph-temperature-day.png)


# Installation
## Arduino IDE
[Download the ZIP archive](https://github.com/smdn/PseudoMuninNode/releases), and [include library](https://www.arduino.cc/en/guide/libraries) from the Arduino IDE menu.

## PlatformIO
Run `platformio lib install https://github.com/smdn/PseudoMuninNode.git` from terminal. See [this document](https://docs.platformio.org/en/latest/core/userguide/lib/cmd_install.html) for detail.

```sh
platformio lib install https://github.com/smdn/PseudoMuninNode.git
```

Or edit [lib_deps](https://docs.platformio.org/en/latest/projectconf/section_env_library.html#lib-deps) in `platformio.ini`.

```ini
[env:esp32dev]
lib_deps = https://github.com/smdn/PseudoMuninNode.git
```

# .NET implementation
[Smdn.Net.MuninNode](https://github.com/smdn/Smdn.Net.MuninNode), the .NET implementation of Munin-Node and Munin-Plugin is also available.
