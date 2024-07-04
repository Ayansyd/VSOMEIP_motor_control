#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <atomic>
#include <set>
#include <thread>

#define SERVICE_ID 0x1111
#define INSTANCE_ID 0x2222
#define EVENTGROUP_ID 0x4444
#define EVENT_ID 0x5555

std::shared_ptr<vsomeip::application> app;
std::atomic<bool> is_eco_mode{false};
std::atomic<bool> is_cruise_mode{false};
std::atomic<bool> is_sports_mode{false};


void on_event(const std::shared_ptr<vsomeip::message> &msg) {
    auto payload = msg->get_payload();
    auto data = payload->get_data();

    int motor_speed = data[0];
    std::cout << "Received motor speed: " << motor_speed << std::endl;

    if (motor_speed > 1 && motor_speed < 80 && !is_cruise_mode) { 
            std::cout << "Switching to cruise Mode..." << std::endl;
            is_cruise_mode = true;
        } else if (motor_speed >= 80 && motor_speed <=120 && !is_eco_mode) { 
            std::cout << "Switching Eco Mode..." << std::endl;
            is_eco_mode = true;
        }
        else if (motor_speed > 120 && motor_speed <160 && !is_sports_mode) { 
            std::cout << "Switching sports Mode..." << std::endl;
            is_sports_mode = true;
        }
}

void run_client() {
    std::set<vsomeip::eventgroup_t> eventgroups = {EVENTGROUP_ID};

    app->request_event(SERVICE_ID, INSTANCE_ID, EVENT_ID, eventgroups);
    app->subscribe(SERVICE_ID, INSTANCE_ID, EVENTGROUP_ID);

    app->register_message_handler(SERVICE_ID, INSTANCE_ID, EVENT_ID, on_event);

    app->start();
}

int main() {
    app = vsomeip::runtime::get()->create_application("Client");
    app->init();

    std::thread client_thread(run_client);
    client_thread.join();

    return 0;
}
