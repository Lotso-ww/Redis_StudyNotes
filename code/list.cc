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
    cout << "lpush 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据库
    redis.flushall();

    // 插入单个元素
    redis.lpush("key1", "111");

    // 初始化列表插入
    redis.lpush("key1", {"222", "333", "444"});

    // 容器插入
    vector<string> values = {"555", "666", "777"};
    redis.lpush("key1" ,values.begin(), values.end());

    // lrange 获取列表中的元素
    vector<string> results;
    auto it = std::back_inserter(results);
    redis.lrange("key1", 0, -1, it);
    
    PrintContainer(results);

    cout << "**********************************************" << endl;
}

void test2(sw::redis::Redis& redis)
{
    cout << "rpush 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据库
    redis.flushall();

    // 插入单个元素
    redis.rpush("key1", "111");

    // 初始化列表插入
    redis.rpush("key1", {"222", "333", "444"});

    // 容器插入
    vector<string> values = {"555", "666", "777"};
    redis.rpush("key1" ,values.begin(), values.end());

    // lrange 获取列表中的元素
    vector<string> results;
    auto it = std::back_inserter(results);
    redis.lrange("key1", 0, -1, it);
    
    PrintContainer(results);
    
    cout << "**********************************************" << endl;
}

void test3(sw::redis::Redis& redis)
{
    cout << "lpop 和 rpop 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    redis.rpush("key", {"1", "2", "3", "4"});
    

    auto result = redis.lpop("key");
    if(result)
    {
        cout << "lpop: " << result.value() << endl;
    }

    result = redis.rpop("key");
    if(result)
    {
        cout << "lpop: " << result.value() << endl;
    }

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "blpop 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    using namespace std::chrono_literals;
    cout << "blpop" << endl;
    redis.flushall();

    auto result = redis.blpop({"key", "key2", "key3"}, 10s);
    if(result)
    {
        cout << "key: " << result.value().first << endl;
        cout << "elem: " << result.value().second << endl;
    }
    else
    {
        cout << "result 无效" << endl;
    }
    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    cout << "llen 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据库
    redis.flushall();

    // 插入元素
    redis.lpush("key", {"111", "222", "333", "444"});
    long long len = redis.llen("key");
    cout << "len: " << len << endl;

    cout << "**********************************************" << endl;
}

int main()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    // test1(redis);
    // test2(redis);
    // test3(redis);
    // test4(redis);
    test5(redis);
    return 0;
}