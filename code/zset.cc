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
    cout << "zadd && zrange 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.zadd("key", "吕布", 99);

    redis.zadd("key", {
        std::make_pair("赵云", 98),
        std::make_pair("典韦", 97),
    });

    vector<std::pair<string, double>> members = {
        std::make_pair("关羽", 95),
        std::make_pair("张飞", 93),
    };
    redis.zadd("key", members.begin(), members.end());

    // zrange 支持两种主要的风格：
    // 1. 只查询 member，不带 score
    // 2. 查询 member，同时带 score
    // 关键就是看插入迭代器指向的容器的类型
    // 指向的容器只是包含一个 string，就是只查询 member
    // 指向的容器包含的是一个 pair，里面有 string 和 double，就是查询 member 同时带有 score 
    vector<string> memberResult;
    auto it = std::back_inserter(memberResult);
    redis.zrange("key", 0, -1, it);
    PrintContainer(memberResult);

    vector<std::pair<string, double>> memberWithScore;
    auto it2 = std::back_inserter(memberWithScore);
    redis.zrange("key", 0, -1, it2);
    PrintContainerPair(memberWithScore);

    cout << "**********************************************" << endl;
}

void test2(sw::redis::Redis& redis)
{
    cout << "zcard 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.zadd("key", "zhangsan", 90);
    redis.zadd("key", "lisi", 91);
    redis.zadd("key", "wangwu", 92);
    redis.zadd("key", "zhaoliu", 93);

    long long result = redis.zcard("key");
    cout << "result: " << result << endl;

    cout << "**********************************************" << endl;
}

void test3(sw::redis::Redis& redis)
{
    cout << "zrem 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.zadd("key", "zhangsan", 90);
    redis.zadd("key", "lisi", 91);
    redis.zadd("key", "wangwu", 92);
    redis.zadd("key", "zhaoliu", 93);

    redis.zrem("key", "zhangsan");

    long long result = redis.zcard("key");
    cout << "result: " << result << endl;

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "zscore 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.zadd("key", "zhangsan", 90);
    redis.zadd("key", "lisi", 91);
    redis.zadd("key", "wangwu", 92);
    redis.zadd("key", "zhaoliu", 93);

    auto score = redis.zscore("key", "zhangsan");
    if(score)
    {
        cout << "score: " << score.value() << endl;
    }
    else
    {
        cout << "score 无效" << endl;
    }
    
    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    cout << "zrank 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    redis.flushall();

    redis.zadd("key", "zhangsan", 90);
    redis.zadd("key", "lisi", 91);
    redis.zadd("key", "wangwu", 92);
    redis.zadd("key", "zhaoliu", 93); 

    auto rank = redis.zrank("key", "zhaoliu");
    if(rank)
    {
        cout << "rank: " << rank.value() << endl;
    }
    else
    {
        cout << "rank 无效" << endl;
    }
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