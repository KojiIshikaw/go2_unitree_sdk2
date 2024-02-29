#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unitree/robot/channel/channel_publisher.hpp>
#include <unitree/idl/go2/LowCmd_.hpp>
#include <unitree/common/time/time_tool.hpp>
#include <unitree/common/thread/thread.hpp>

using namespace unitree::common;
using namespace unitree::robot;

#define TOPIC_LOWCMD "rt/lowcmd"

constexpr double PosStopF = 0;
constexpr double VelStopF = 0;

class Custom
{
public:
    explicit Custom() {}
    ~Custom() {}

    void Init();

private:
    void InitLowCmd();
    void LowCmdWrite();

private:
    unitree_go::msg::dds_::LowCmd_ low_cmd{}; // default init

    /* publisher */
    ChannelPublisherPtr<unitree_go::msg::dds_::LowCmd_> lowcmd_publisher;

    /* LowCmd write thread */
    ThreadPtr lowCmdWriteThreadPtr;
};

void Custom::Init()
{
    InitLowCmd();

    /* create publisher */
    lowcmd_publisher.reset(new ChannelPublisher<unitree_go::msg::dds_::LowCmd_>(TOPIC_LOWCMD));
    lowcmd_publisher->InitChannel();

    /* loop publishing thread */
    lowCmdWriteThreadPtr = CreateRecurrentThreadEx("writebasiccmd", UT_CPU_ID_NONE, 2000, &Custom::LowCmdWrite, this);
}

void Custom::InitLowCmd()
{
    low_cmd.head()[0] = 0xFE;
    low_cmd.head()[1] = 0xEF;
    low_cmd.level_flag() = 0xFF;
    low_cmd.gpio() = 0;

    for(int i=0; i<20; i++)
    {
        low_cmd.motor_cmd()[i].mode() = (0x01);   // motor switch to servo (PMSM) mode
        low_cmd.motor_cmd()[i].q() = (PosStopF);
        low_cmd.motor_cmd()[i].kp() = (0);
        low_cmd.motor_cmd()[i].dq() = (VelStopF);
        low_cmd.motor_cmd()[i].kd() = (0);
        low_cmd.motor_cmd()[i].tau() = (0);
    }

    unitree_go::msg::dds_::BmsCmd_ bms_cmd{};
    bms_cmd.off(0xA5);
    low_cmd.bms_cmd(bms_cmd);
}

void Custom::LowCmdWrite()
{
    // std::cout << "pub lowcmd! : " <<  << std::endl;
    lowcmd_publisher->Write(low_cmd);
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
        exit(-1); 
    }

    ChannelFactory::Instance()->Init(0, argv[1]);

    Custom custom;
    custom.Init();

    while (1)
    {
        sleep(10);
    }

    return 0;
}
