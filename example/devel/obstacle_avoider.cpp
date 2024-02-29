#include <unitree/robot/go2/obstacles_avoid/obstacles_avoid_client.hpp>
#include <iostream>
#include <cstdlib> // 追加: atoi関数のため

int main(int argc, char* argv[]) // 引数を受け取るように変更
{
    /*
     * Initialize ChannelFactory
     */
    unitree::robot::ChannelFactory::Instance()->Init(0);
    unitree::robot::go2::ObstaclesAvoidClient oac;

    std::cout << std::boolalpha;

    oac.SetTimeout(5.0f);
    oac.Init();

    bool is_avoid;
    bool set_avoid = true; // デフォルト値は true に設定

    // コマンドライン引数から障害物回避の設定を読み取る
    if (argc > 1) { // 引数が与えられた場合
        set_avoid = atoi(argv[1]) != 0; // 0 以外の値なら true、0 なら false
    }

    int32_t ret = oac.Get(is_avoid);
    std::cout << "障害物回避機能の状態取得結果: 戻り値 = " << ret << ", 現在の設定 = " << (is_avoid ? "有効" : "無効") << std::endl;

    ret = oac.Set(set_avoid);
    std::cout << "障害物回避機能を" << (set_avoid ? "有効" : "無効") << "に設定: 戻り値 = " << ret << std::endl;

    ret = oac.Get(is_avoid);
    std::cout << "設定後の障害物回避機能の状態取得結果: 戻り値 = " << ret << ", 現在の設定 = " << (is_avoid ? "有効" : "無効") << std::endl;

    usleep(1000);

    return 0;
}
