
# MQTT IoT Device Simulator 

This small piece of code is a mock/simulation of a IoT device with MQTT. It is useful for simulating real IoT devices in an MQTT enabled network for testing purposes.

Instead of loading the firmware to a real device for testing, you can use this project for simulation the communication over MQTT. You can publish to a topic, listen to a topic asyncronally, and later pull out the received data.

The implementation allows that one simulated device can be both a listener and a subscriber although this has no real usage.








## FAQ

#### Why would I use this?

If you have a MQTT device like the ESP32 and you wish to simulate real publish/suscribe to it because the software on your PC is also connected to the MQTT broker is easier to create a virtual ESP32 and simulate the communication that way. 

#### Why have you created this?

This is software was created as a byproduct of a closed source project of mine, but I think that his code could be useful to anyone working with IoT devices.

#### Does it have MQTT security?

Currently, there are no security features coded into the simulated device.

#### Why is it there Death time?

I found out that the MQTT broker needs some time to process the publish/suscription other ways kicks out the new device


## Usage/Examples

```cpp
TEST(mqtt_avalibility, check_mosquitto){
    esp32 iot_device_1(0, MQTT_ADRESS, "id1");
    iot_device_1.wait_connect();
    esp32 iot_device_2(0, MQTT_ADRESS, "id2");
    string msg = "world";
    pair<string, string> data_in;
    iot_device_2.wait_connect();
    std::this_thread::sleep_for(std::chrono::milliseconds(DEATH_TIME));
    iot_device_2.suscribe("hello", QOS);
    std::this_thread::sleep_for(std::chrono::milliseconds(DEATH_TIME));
    iot_device_1.publish("hello", msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(DEATH_TIME));
    ASSERT_NE(0, iot_device_2.get_message_q_size());
    data_in = iot_device_2.get_last_msg();
    iot_device_2.remove_last_msg();
    ASSERT_EQ(0, iot_device_2.get_message_q_size());
    ASSERT_EQ("hello", data_in.first);
    ASSERT_EQ(msg, data_in.second);
}
```


## Installation and Usage

This sofware is dependent on the paho-cpp library, you must install it first.
Follow the install instruction from link [link](https://github.com/eclipse/paho.mqtt.cpp)

Is recommended to use a testing framework like GTest, but the code will run without one.

To add the code to your project add the following to your CMake

```cmake

find_package(OpenSSL REQUIRED)
find_library(paho-mqttpp3 NAMES libpaho-mqttpp3.a REQUIRED)
find_library(paho-mqtt3a NAMES libpaho-mqtt3a.so REQUIRED)

target_link_libraries(${PROJECT_NAME} pthread paho-mqttpp3 paho-mqtt3as ${CMAKE_DL_LIBS}) 

add_executable(${PROJECT_NAME} mock_esp32.cpp)
```


## Authors

- [@stan-one](https://www.github.com/stan-one)


## License

[MIT](https://choosealicense.com/licenses/mit/)

