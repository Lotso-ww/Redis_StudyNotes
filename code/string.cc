#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <sw/redis++/redis++.h> // 包含一下头文件
#include "util.hpp"

using namespace std;

void test1(sw::redis::Redis& redis)
{
    cout << "set 和 get 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置数据
    redis.set("key1", "111");
    auto value1 = redis.get("key1");
    cout << "value1=" << value1.value() << endl;

    redis.set("key2", "222");
    auto value2 = redis.get("key2");
    cout << "value2=" << value2.value() << endl;

    cout << "**********************************************" << endl;
}

void test2(sw::redis::Redis& redis)
{
    using namespace std::chrono_literals;
    
    cout << "set 带超时时间: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置随便带上时间
    redis.set("key", "111", 10s);

    // 睡眠几秒
    std::this_thread::sleep_for(3s);

    // 获取剩余时间
    long long time = redis.ttl("key");
    cout << "time=" << time << endl;

    cout << "**********************************************" << endl;
}


void test3(sw::redis::Redis& redis)
{
    cout << "set 的 NX 和 XX 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // NX -- 不存在才会成功 -- 也就是新建
    redis.set("key1", "111", 0s, sw::redis::UpdateType::NOT_EXIST);
    auto value1 = redis.get("key1");
    if(value1)
        cout << "value1=" << value1.value() << endl; // 这里会打印出111, 因为key1不存在, 可以新建

    // XX -- 存在才会成功 -- 也就是更新
    redis.set("key1", "222", 0s, sw::redis::UpdateType::EXIST);
    redis.set("key2", "333", 0s, sw::redis::UpdateType::EXIST);
    value1 = redis.get("key1");
    auto value2 = redis.get("key2");
    if(value1)
        cout << "value1=" << value1.value() << endl; // 这里会打印出222, 因为key1存在, 可以更新
    if(value2)
        cout << "value2=" << value2.value() << endl; // 这里不会打印出333, 因为key2不存在

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "mset 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 一次性设置多个 key
    // 1. 第一种写法，使用初始化列表描述多个键值对
    // redis.mset({std::make_pair("key1", "111"), std::make_pair("key2", "222"), std::make_pair("key3", "333")});

    // 2. 第二种写法，可以把多个键值对提前组织到容器中，以迭代器的形式告诉 mset
    vector<std::pair<string, string>> keys = {
        {"key1", "111"},
        {"key2", "222"},
        {"key3", "333"}
    };

    redis.mset(keys.begin(), keys.end());

    // 获取
    auto value1 = redis.get("key1");
    if(value1)
        cout << "value1=" << value1.value() << endl;

    auto value2 = redis.get("key2");
    if(value2)
        cout << "value2=" << value2.value() << endl;

    auto value3 = redis.get("key3");
    if(value3)
        cout << "value3=" << value3.value() << endl;

    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    cout << "mget 的使用测试: " << endl;
    cout << "**********************************************" << endl;


    // 清除数据
    redis.flushall();

    // 设置数据
        vector<std::pair<string, string>> keys = {
        {"key1", "111"},
        {"key2", "222"},
        {"key3", "333"}
    };

    redis.mset(keys.begin(), keys.end());

    // 一次性获取多个元素
    vector<sw::redis::OptionalString> result;
    auto it = std::back_inserter(result);
    redis.mget({"key1", "key2", "key3", "key4"}, it);

    PrintOptioanlContainer(result);

    cout << "**********************************************" << endl;
}


void test6(sw::redis::Redis& redis)
{
    cout << "getrange 和 setrange 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置数据
    redis.set("key", "abcdefghizk");

    // 获取 2-5 
    string result = redis.getrange("key", 2, 5);
    cout << "getrange: " << result << endl;

    // 设置
    redis.setrange("key", 2, "ppp");

    // 获取
    auto value = redis.get("key");
    if(value)
        cout << "value=" << value.value() << endl;

    cout << "**********************************************" << endl;
}

void test7(sw::redis::Redis& redis)
{
    cout << "incr 和 decr 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据库
    redis.flushall();

    // 设置
    redis.set("key1", "100");

    // incr
    long long ret = redis.incr("key1");
    cout << "result: " << ret << endl;
    
    auto value = redis.get("key1");
    cout << "value1: " << value.value() << endl;

    ret = redis.decr("key1");
    cout << "result: " << ret << endl;

    value = redis.get("key1");
    cout << "value: " << value.value() << endl;
    

    cout << "**********************************************" << endl;
}


int main()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    // test1(redis);
    // test2(redis);
    // test3(redis);
    // test4(redis);
    // test5(redis);
    // test6(redis);
    test7(redis);
    return 0;
}
