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

    // set
    redis.set("key1", "111");
    redis.set("key2", "222");
    redis.set("key3", "333");
    

    // get -- 返回值是 OptionalString
    // value4 其实是 optional 的非法状态，对于非法状态的 optional，进行取值操作，就会抛出异常
    auto value1 = redis.get("key1");
    // 可以使用 try catch，但是更常见的是先进行判定，再访问
    // optional 可以隐式转换成 bool 类型，可以直接在 if 中判断，如果是无效元素，就是返回 false
    if(value1)
        cout << "value1=" << value1.value() << endl;

    auto value2 = redis.get("key2");
    if(value2)
        cout << "value2=" << value2.value() << endl;

    auto value3 = redis.get("key3");
    if(value3)
        cout << "value3=" << value3.value() << endl;

    auto value4 = redis.get("key4");
    if(value4)
        cout << "value4=" << value4.value() << endl;
    cout << "**********************************************" << endl;
}


void test2(sw::redis::Redis& redis)
{
    cout << "exists 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置数据
    redis.set("key1", "111");
    redis.set("key3", "333");

    // 判断是否存在
    auto ret = redis.exists("key1");
    cout << ret << endl;

    ret = redis.exists("key2");
    cout << ret << endl;

    // 同时判断多个
    ret = redis.exists({"key1", "key2", "key3"});
    cout << ret << endl;

    cout << "**********************************************" << endl;
}

void test3(sw::redis::Redis& redis)
{
    cout << "del 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置数据
    redis.set("key1", "111");
    redis.set("key2", "222");

    // 跟 exists 一样，可以同时删除多个
    auto ret = redis.del({"key1", "key2", "key3"});
    cout << ret << endl;

    ret = redis.exists({"key1", "key2"});
    cout << ret << endl; // 虽然设置了，但是删除了之后就都没了

    cout << "**********************************************" << endl;
}

void test4(sw::redis::Redis& redis)
{
    cout << "keys 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置 key
    redis.set("key1", "111");
    redis.set("key2", "222");
    redis.set("key3", "333");
    redis.set("key4", "444");
    redis.set("key5", "555");
    redis.set("key6", "666");

    // keys 的第二个参数，是一个 “插入迭代器”，咱们需要先准备好一个保存结果的容器
    // 接下来再创建一个插入迭代器指向容器的位置，就可以把 keys 获取到的结果依次通过刚才的插入迭代器插入到容器的指定位置中
    vector<string> result;
    auto it = std::back_inserter(result);
    redis.keys("*", it);

    PrintContainer(result);
    cout << "**********************************************" << endl;
}

void test5(sw::redis::Redis& redis)
{
    using namespace std::chrono_literals; // 后续可以直接使用字面量常量 eg:3s
    cout << "expire 和 ttl 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();

    // 设置 key
    redis.set("key1", "111");

    // 设置过期时间
    // 其实直接写10也是可以的，就是 10s，但是这样不直观，所以我们使用这个方法
    redis.expire("key1", std::chrono::seconds(10));
    // 其实还可以这样写
    // redis.expire("key", 10s);

    // 休眠一段时间 -- 我们使用 sleep_for 方法
    std::this_thread::sleep_for(3s);

    // 查看剩余时间
    auto ret = redis.ttl("key1");
    cout << ret << endl;
    cout << "**********************************************" << endl;
}

void test6(sw::redis::Redis& redis)
{
    cout << "type 的使用测试: " << endl;
    cout << "**********************************************" << endl;

    // 清除数据
    redis.flushall();
    string result;

    // string
    redis.set("key1", "111");
    result = redis.type("key1");
    cout << "key1: " << result << endl;

    // list
    redis.lpush("key2", "222");
    result = redis.type("key2");
    cout << "key2: " << result << endl;

    // hash
    redis.hset("key3", "field1", "333");
    result = redis.type("key3");
    cout << "key3: " << result << endl;

    // set
    redis.sadd("key4", "444");
    result = redis.type("key4");
    cout << "key4: " << result << endl;

    // zset
    redis.zadd("key5", "赵云", 100);
    result = redis.type("key5");
    cout << "key5: " << result << endl;

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