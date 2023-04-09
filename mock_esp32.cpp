#include "mock_esp32.hpp"
// Callback for when a message arrives.


callback::callback(mqtt::async_client& cli, mqtt::connect_options& connOpts, esp32* _host)
	: nretry_(0), cli_(cli), connOpts_(connOpts), host(_host) {}


void callback::message_arrived(mqtt::const_message_ptr msg){
    host->load_msg(pair<string, string>(msg->get_topic(), msg->to_string()));
};

void callback::on_failure(const mqtt::token& tok) {};
void callback::on_success(const mqtt::token& tok) {};

void callback::connected(const std::string& cause){
    host->set_connected();
}

void callback::connection_lost(const std::string& cause){
	std::cout << "\nConnection lost" << std::endl;
	if (!cause.empty())
		std::cout << "\tcause: " << cause << std::endl;

	std::cout << "Reconnecting..." << std::endl;
}


esp32::esp32(int id, string server_ad, string client_id):rack_id(id), SERVER_ADDRESS(server_ad), CLIENT_ID(client_id){
        cli_pub.reset(new mqtt::client(SERVER_ADDRESS, CLIENT_ID+"_pub"));
        cli_pub->connect(connOpts_pub);
        
        cli_sub.reset(new mqtt::async_client(SERVER_ADDRESS, CLIENT_ID+"_sub"));
        cb.reset(new callback(*cli_sub, connOpts_sub, this));
        cli_sub->set_callback(*cb);
        cli_sub->connect(connOpts_sub, nullptr,*cb);
        
    }

void esp32::publish(string topic, string payload){
    mqtt::message_ptr msg;
    const int QOS = 1;
    msg = mqtt::make_message(topic, payload);
    msg->set_qos(QOS);
    cli_pub->publish(msg);
}

void esp32::suscribe(string topic, int QOS){
    cli_sub->subscribe(topic, QOS);
}

void esp32::load_msg(pair<string, string> msg){
    received_msg.push(msg);
}


pair<string, string> esp32::get_last_msg(){
    return received_msg.front();
}

void esp32::remove_last_msg(){
    received_msg.pop();
}

int esp32::get_message_q_size(){
    return received_msg.size();
}

void esp32::set_connected(){
    this->conn_state = true;
}

void esp32::wait_connect(){
    while(!this->conn_state){
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
}