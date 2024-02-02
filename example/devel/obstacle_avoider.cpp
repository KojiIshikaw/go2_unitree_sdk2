#include <unitree/robot/go2/obstacles_avoid/obstacles_avoid_client.hpp>

int main()
{
    /*
     * Initilaize ChannelFactory
     */
    unitree::robot::ChannelFactory::Instance()->Init(0);
    unitree::robot::go2::ObstaclesAvoidClient oac;

    std::cout << "declared oac" << std::endl;

    oac.SetTimeout(5.0f);
    oac.Init();

    //Test Api
    // while (true)
    // {
        bool is_avoid;
        int32_t ret = oac.Get(is_avoid);
        std::cout << "Call Get ret:" << ret << "enable: " << is_avoid <<std::endl;

        ret = oac.Set(false);
        std::cout << "Call Set ret:" << ret << std::endl;

        ret = oac.Get(is_avoid);
        std::cout << "Call Get ret:" << ret << "enable: " << is_avoid <<std::endl;

        usleep(1000);
    // }

    return 0;
}
