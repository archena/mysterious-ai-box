# The Mysterious Box of AI

The mysterious box is a tangible way to explain the principles of machine learning by placing a simple neural network on some hardware.

*n.b.* this is very much a work-in-progress.

## Hardware

The neural network runs on an Espressive [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview) microcontroller. This is a reasonably powerful device that includes support for wifi and bluetooth along with other useful bits and pieces built in.

## Microcontroller software

To build and deploy the software you'll need to use [PlatformIO](https://platformio.org). While these instructions assume you're doing everything at the command line, PlatformIO also has its own IDE and integration into a wide range of existing development environments.

ESP32 is Arduino-compatible and the Mysterious Box has been written using the Arduino framework. As a result it is possible to use the Arduino IDE instead of PlatformIO; having tried both, my advice is that PlatformIO is at least an order of magnitude easier in the long run. Finally note that ESP32 can also be programmed using the offical [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) toolchain but I chose Arduino to make the software more portable.

### Using a virtual environment (optional)

PlatformIO is a Python-based tool and I like to run it in `virtualenv`. Feel free to skip if you're installing / have installed PlatformIO using a different method.

You'll need to have installed Python 2, pip, and virtualenv. When running for the first time you need to initialise the environment and use `pip` to install dependencies:

```
virtualenv env
source env/bin/activate
pip install -r requirements.txt
```

From here on you can `source env/bin/activate` to start the virtual environment after which you can run `platformio` (provided that you've pulled dependencies using `pip` already).

### Building and deploying

*Just build*

```
platformio run
```

*Build and upload*

```
platformio upload
```

*Run the tests* - this builds a new image and uploads it so that tests are run on the device

```
platformio test
```
