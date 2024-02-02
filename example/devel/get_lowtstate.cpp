#include <iostream>
#include <chrono>
#include <unitree/robot/channel/channel_subscriber.hpp>
#include <unitree/idl/go2/LowState_.hpp>
#include <unitree/common/time/time_tool.hpp>
#include <unitree/common/thread/thread.hpp>
#include <unitree/robot/channel/channel_factory.hpp>

using namespace unitree::common;
using namespace unitree::robot;
using namespace std::chrono;

#define TOPIC_LOWSTATE "rt/lowstate"

class LowStateHandler
{
public:
    LowStateHandler() : last_print_time_(steady_clock::now()), message_count_(0) {}
    ~LowStateHandler() = default;

    void Init();
    void LowStateMessageHandler(const void *message);

private:
    unitree_go::msg::dds_::LowState_ low_state{};
    ChannelSubscriberPtr<unitree_go::msg::dds_::LowState_> lowstate_subscriber;
    steady_clock::time_point last_print_time_;
    int message_count_;
};

void LowStateHandler::Init()
{
    lowstate_subscriber.reset(new ChannelSubscriber<unitree_go::msg::dds_::LowState_>(TOPIC_LOWSTATE));
    lowstate_subscriber->InitChannel(std::bind(&LowStateHandler::LowStateMessageHandler, this, std::placeholders::_1), 1);
}

// DDS topic "rt/lowstate" の受信ごとに呼び出される関数
void LowStateHandler::LowStateMessageHandler(const void *message)
{
    low_state = *(unitree_go::msg::dds_::LowState_ *)message;
    message_count_++;

    auto now = steady_clock::now();

    // 1秒に1回Bmsの情報を出力する
    if(duration_cast<seconds>(now - last_print_time_).count() >= 1)
    {
        std::cout << "----------------------------------------" << std::endl;
        const auto& bms_state = low_state.bms_state();

        std::cout << "BmsState received:" << std::endl;
        std::cout << "Version High: " << static_cast<int>(bms_state.version_high()) << std::endl;
        std::cout << "Version Low: " << static_cast<int>(bms_state.version_low()) << std::endl;
        std::cout << "Status: " << static_cast<int>(bms_state.status()) << std::endl;
        std::cout << "SoC: " << static_cast<int>(bms_state.soc()) << "%" << std::endl;
        std::cout << "Current: " << bms_state.current() << " mA" << std::endl;
        std::cout << "Cycle Count: " << bms_state.cycle() << std::endl;

        std::cout << "LowState message frequency: " << message_count_ << " messages/sec" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        last_print_time_ = now;
        message_count_ = 0;
    }
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
        exit(-1); 
    }

    ChannelFactory::Instance()->Init(0, argv[1]);

    LowStateHandler handler;
    handler.Init();

    while (true)
    {
        sleep(1);
    }

    return 0;
}
