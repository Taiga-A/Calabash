# Calabash_zh

> 中文

一个适用于 `linux` 系统的自给自足的 `http web` 框架。

`c++17` 及以上适用

## 模块

* util

  * Base64 MD5 编码
  * data reader, 储存字符串和流，主要用来发送文件
  * object pool, 单头文件并发对象池
  * logger, 日志系统，包含 `SYSTEM` `INFO` `WARNING` `ERROR` 四个等级。
  * token, 简单的加密身份验证，包含过期时间及用户数据

* thread

  * thread pool, 单头文件任务流线程池

* socket

  * 封装的`linxu` Socket 使用 `epoll` 事件驱动

* web

  * server, Http 服务
  * request and request maker, 生成请求
  * response, 生成响应
  * http task, Http 主要逻辑
  * exception, http 异常，用于发送事件

* database

  * sqlite3, 封装的Sqlite3 数据库接口, 提供SQL接口、预编译接口、参数绑定接口、结果格式化接口 ...

  

## 第三方

*  [谷歌测试 GoogleTest](https://github.com/google/googletest)
*  [Sqlite3数据库](https://sqlite.org)
*  [现代C++JSON - JSON for Modern C++](https://github.com/nlohmann/json)



## 安装

### CMAKE

``` shell
$ cd calabash
$ mkdir build && cd build
$ cmake ..
$ make .
```

### CLion 和CMAKE

> 直接作为项目打开



## 快速开始

### 创建一个简单的服务

``` c++
#include <calabash/server.h>
#include <calabash/web/request.h>
#include <calabash/web/responce.h>

using namespace calabash;

void app_hello(Request &req, Responce &res, ServerNextFunc &next) {
    res.BodyData("<h1> Hello Calabash! </h1>");
    res.SetHtmlType();
}

int main() {
    Server server;
    server.Bind("/hello", app_hello);
    server.Listen(8080);
	server.Start();
}
```



### 附加静态文件服务

> 服务器会优先寻找 `bind` 的地址，其次才是静待文件

``` c++
#include <calabash/server.h>
#include <calabash/web/request.h>
#include <calabash/web/responce.h>

using namespace calabash;

void app_hello(Request &req, Responce &res, ServerNextFunc &next) {
    res.BodyData("<h1> Hello Calabash! </h1>");
    res.SetHtmlType();
}

int main() {
    Server server;
    server.Bind("/hello", app_hello);
    server.StaticPath("./static");
    server.Listen(8080);
	server.Start();
}

// |-server.bin
// |-static
// |---|-index.html
// |---|-style.css
```



### 构建复杂的应用程序

> 中间件形式，全局处理

``` c++
#include <calabash/server.h>
#include <calabash/web/request.h>
#include <calabash/web/responce.h>

using namespace calabash;
using namespace nlohmann; // json - third party

void app_first(Request &req, Responce &res, ServerNextFunc &next) {
	string str = req.body();
    req.json() = json::parse(str);
    next();	// run next function
}

void app_next1(Request &req, Responce &res, ServerNextFunc &next) {
	// do something
    next();	// run app_next2
    // go on do something
}

void app_next2(Request &req, Responce &res, ServerNextFunc &next) {
	// do something other
}

int main() {
    Server server;
    server.ProcessBind(app_first);
    server.Bind("/hello", {app_next, app_next2});
    server.Listen(8080);
	server.Start();
}
```

所有被 `bind` 的url 均会执行 `ProcessBind` 的函数， `Bind` 的函数会根据 `ServerNextFunc` 的调用，依次递归调用。

执行函数栈： app_first -> app_next1 -> app_next2

### 推荐的 JSON 构建方式

``` c++
#include <calabash/server.h>
#include <calabash/web/request.h>
#include <calabash/web/responce.h>
#include <calabash/web/exception.h>

using namespace calabash;
using namespace nlohmann; // json - third party

void app_exception(Request &req, Responce &res, ServerNextFunc &next) {
    try {
        next();
    } catch(const HttpException &e) {
        res.set_code(e.base_code());
        res.BodyData(json{
            {"code", e.code()},
            {"msg", e.what()}
        }.dump(2));
    }
}
void app_first(Request &req, Responce &res, ServerNextFunc &next) {}
void app_hello(Request &req, Responce &res, ServerNextFunc &next) {
    // do something
    throw HttpException(200, "hello calabash!");
}

int main() {
    Server server;
    server.ProcessBind(app_exception);
    server.ProcessBind(app_first);
    server.Bind("/hello", app_hello);
    server.Listen(8080);
	server.Start();
}
```

``` json
// status 200 ok
{
    "code": 200,
    "msg": "hello calabash!"
}
```



