#include <unitree/robot/channel/channel_publisher.hpp>
#include <unitree/common/time/time_tool.hpp>
#include <unitree/idl/go2/SportModeCmd_.hpp>

#define TOPIC_SPORTMODECMD "rt/sportmodecmd"

using namespace unitree::robot;
using namespace unitree::common;

int main()
{
    ChannelFactory::Instance()->Init(0);
    ChannelPublisher<unitree_go::msg::dds_::SportModeCmd_> publisher(TOPIC_SPORTMODECMD);

    publisher.InitChannel();

    while (true)
    {
        unitree_go::msg::dds_::SportModeCmd_ cmd;
        // コマンドのフィールドに値を設定する
        cmd.mode(6);
        cmd.gait_type(0);
        cmd.speed_level(0);
        cmd.foot_raise_height(0);
        cmd.body_height(0);
        // cmd.position([0, 0]);
        // cmd.euler([0, 0, 0]);
        // cmd.velocity([0, 0]);
        cmd.yaw_speed(0);
        // cmd.bms_cmd(0);
        // cmd.path_point(0);

        publisher.Write(cmd);
        sleep(0.01);
    }

    return 0;
}
