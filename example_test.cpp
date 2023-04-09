#include "mock_esp32.hpp"
#include <thread>
#include <chrono>
#include <iostream>

#include <gtest/gtest.h>

using namespace std;

#define MQTT_ADRESS "tcp://localhost:1883"
#define QOS 1
#define DEATH_TIME 100 //ms

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
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
