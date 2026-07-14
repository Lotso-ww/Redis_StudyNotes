#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sw/redis++/redis++.h> // 包含一下头文件

using namespace std;

int main()
{
    // 创建 redis 对象的时候，需要在构造函数中，指定 redis 服务器的地址和端口号
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    // 调用 ping 方法，让客户端给服务器发了一个 PING，然后服务器会返回一个 PONG，可以通过返回值获取到
    string result = redis.ping();
    cout << result << endl;
    return 0;
}