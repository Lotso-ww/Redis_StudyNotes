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
    std::cout << "hset && hget 的使用测试: " << std::endl;
    std::cout << "**********************************************" << std::endl;

    redis.flushall();

    redis.hset("key", "f1", "111");
    redis.hset("key", std::make_pair("f2", "222"));
    // hset 能够一次性插入多个 field-value
    redis.hset("key", {
        std::make_pair("f3", "333"),
        std::make_pair("f4", "444"),
    });

    // 先放在容器里面然后再插入
    vector<std::pair<string, string>> fields = {
        std::make_pair("f5", "555"),
        std::make_pair("f6", "666"),
    };
    redis.hset("key", fields.begin(), fields.end());

    auto result = redis.hget("key", "f1");
    if(result)
    {
        std::cout << "result: " << result.value() << std::endl;
    }
    else
    {
        std::cout << "result 无效" << std::endl;
    }
    std::cout << "**********************************************" << std::endl;
}

void test2(sw::redis::Redis& redis)
{
    cout << "hexists 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.hset("key", "f1", "111");
    redis.hset("key", "f2", "222");
    redis.hset("key", "f3", "333");

    bool result = redis.exists("key", "f4");
    cout << "result: " << result << endl;

    cout << "**********************************************" << endl;
}

void test3(sw::redis::Redis& redis)
{
    cout << "hdel && hlen 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.hset("key", "f1", "111");
    redis.hset("key", "f2", "222");
    redis.hset("key", "f3", "333");

    long long result = redis.hdel("key", "f1");
    cout << "result: " << result << endl;
    

    result = redis.hdel("key", {"f2", "f3"});
    cout << "result: " << result << endl;

    long long len = redis.hlen("key");
    cout << "len: " << len << endl; 

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "hkeys && hvals 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.hset("key", "f1", "111");
    redis.hset("key", "f2", "222");
    redis.hset("key", "f3", "333");
    
    vector<string> fields;
    auto itFields = std::back_inserter(fields);
    redis.hkeys("key", itFields);
    PrintContainer(fields);

    vector<string> values;
    auto itValues = std::back_inserter(values);
    redis.hvals("key", itValues);
    PrintContainer(values);

    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    cout << "hmget && hmset 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.hmset("key", {
        std::make_pair("f1", "111"), 
        std::make_pair("f2", "222"), 
        std::make_pair("f3", "333"), 
    });

    vector<std::pair<string, string>> pairs = {
        std::make_pair("f6", "666"),
        std::make_pair("f5", "555"),
        std::make_pair("f6", "666"),
    };
    redis.hmset("key", pairs.begin(), pairs.end());

    vector<string> values;
    auto it = std::back_inserter(values);
    redis.hmget("key", {"f1", "f2", "f3"}, it);
    PrintContainer(values);

    cout << "**********************************************" << endl;
}

int main()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    // test1(redis);
    // test2(redis);
    //test3(redis);
    test4(redis);
    // test5(redis);

    return 0;
}
