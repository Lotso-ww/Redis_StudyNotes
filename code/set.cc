#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <set>
#include <sw/redis++/redis++.h> // 包含一下头文件
#include "util.hpp"

using namespace std;

void test1(sw::redis::Redis& redis)
{
    cout << "sadd 和 smembers 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 一次添加一个元素
    redis.sadd("key", "111");

    // 一次添加多个元素(初始化列表)
    redis.sadd("key", {"222", "333", "444"});

    // 一次添加多个元素(使用迭代器)
    set<string> elems = {"555", "666", "777"};
    redis.sadd("key", elems.begin(), elems.end());

    // 获取
    set<string> result;
    auto it = std::inserter(result, result.end());
    redis.smembers("key", it);

    PrintContainer(result);

    cout << "**********************************************" << endl;
}

void test2(sw::redis::Redis& redis)
{
    cout << "sismember 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.sadd("key", {"111", "222", "333", "444"});

    bool result = redis.sismember("key", "555");
    cout << "result: " << result << endl;

    cout << "**********************************************" << endl;
}

void test3(sw::redis::Redis& redis)
{
    cout << "scard 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    redis.sadd("key", {"111", "222", "333"});
    long long ret = redis.scard("key");
    cout << "result: " << ret << endl;

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "spop 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.sadd("key", {"111", "222", "333", "444"});
    auto ret = redis.spop("key");
    if(ret)
    {
        cout << "result: " << ret.value() << endl;
    }
    else
    {
        cout << "result 无效!" << endl;
    }

    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    cout << "sinter 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.sadd("key1", {"111", "222", "333"});
    redis.sadd("key2", {"111", "222", "444"});

    set<string> result;
    auto it = std::inserter(result, result.end());
    redis.sinter({"key1", "key2"}, it);

    PrintContainer(result);

    cout << "**********************************************" << endl;
}

void test6(sw::redis::Redis& redis)
{
    cout << "sinterstore 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.sadd("key1", {"111", "222", "333"});
    redis.sadd("key2", {"111", "222", "444"});

    long long len = redis.sinterstore("key3", {"key1", "key2"});
    cout << "len: " << len << endl;

    set<string> result;
    auto it = std::inserter(result, result.end());
    redis.smembers("key3", it);

    PrintContainer(result);

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
    test6(redis);

    return 0;
}
