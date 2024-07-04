#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <random>

#define SERVICE_ID 0x1111
#define INSTANCE_ID 0x2222
#define EVENTGROUP_ID 0x4444
#define EVENT_ID 0x5555

std::shared_ptr<vsomeip::application> app;
std::atomic<bool> is_eco_mode{false};
std::atomic<bool> is_cruise_mode{false};
std::atomic<bool> is_sports_mode{false};

static int client_Id;

void run_service()
{
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 160);

    while (true)
    {
        int motor_speed = dis(gen); // Generate motor speed between 1 and 160

        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        payload->set_data(std::vector<vsomeip::byte_t>({(vsomeip::byte_t)motor_speed}));

        app->notify(SERVICE_ID, INSTANCE_ID, EVENT_ID, payload);

        if (motor_speed > 1 && motor_speed < 80 )  {
            std::cout << motor_speed << std::endl;
            std::cout << "In  cruise Mode..." << std::endl;
            std::cout << "Client ID:" << client_Id << std::endl;

            is_cruise_mode = false;
            is_eco_mode = true;
            is_sports_mode = false;

        } else if (motor_speed >= 80 && motor_speed <=120 ) {
            std::cout << motor_speed << std::endl;
            std::cout << "In Eco Mode..." << std::endl;
            std::cout << "Client ID:" << client_Id << std::endl;
            is_cruise_mode = true;
            is_eco_mode = false;
            is_sports_mode = false;
        }
        else if (motor_speed > 120 && motor_speed <160 ) {
            std::cout << motor_speed << std::endl;
            std::cout << "In sports Mode..." << std::endl;
            std::cout << "Client ID:" << client_Id << std::endl;
            is_cruise_mode = false;
            is_eco_mode = false;
            is_sports_mode = true;
        }
            client_Id++;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main()
{
    app = vsomeip::runtime::get()->create_application("Service");
    app->init();

    app->offer_service(SERVICE_ID, INSTANCE_ID);

    std::thread service_thread(run_service);
    service_thread.join();

    return 0;
}
