#pragma once 

#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include "mqtt/async_client.h"
#include "mqtt/client.h"
#include <memory>
#include <queue>

#define N_RETRY_ATTEMPTS 5

using namespace std;

class esp32;

class callback : public virtual mqtt::callback, public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
    
	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override;

    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
    void connected(const std::string& cause) override;
    void connection_lost(const std::string& cause) override;
    esp32 *host;

public:
    
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts, esp32 *_host);
};


class esp32{
    public:
    esp32(int id, string server_ad, string client_id);
    void publish(string topic, string payload);
    void suscribe(string topic, int QOS);
    void load_msg(pair<string, string> msg);
    pair<string, string> get_last_msg();
    void remove_last_msg();
    int get_message_q_size();

    void wait_connect();

    void set_connected();

    private:
    int rack_id;
    const string SERVER_ADDRESS;
    const string CLIENT_ID;
	unique_ptr<mqtt::client> cli_pub;
    unique_ptr<mqtt::async_client> cli_sub;
    
    mqtt::connect_options connOpts_pub;
    mqtt::connect_options connOpts_sub;
    unique_ptr<callback> cb;
    bool conn_state = false;
    queue<pair<string, string>> received_msg;

};