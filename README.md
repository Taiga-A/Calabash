# Calabash
A Self-sufficient web frame for linux only, used Sqlite3 database.

*stdc++ version must higher than C++17* 


## Module

* util
  * Base64 MD5
  * data reader, Stream and string data mixed reader, used for send file
  * object pool, A header only object pool
  * logger, A log system with `SYSTEM` `INFO` `WARNING` `ERROR` four level.
* thread
  * thread pool, A header only thread pool with task queue.
* socket
  * Encapsulated linux socket api with epoll 
* web
  * server, Http server
  * request and request maker, Read socket data to request.
  * response, Http response link util/data_reader
  * http task, Http main logic.
* database
  * sqlite3, Carefully encapsulated sqlite api, compiled bind sql...
* app
  * Example for this lib

## Third party

*  [GoogleTest](https://github.com/google/googletest)

*  [Sqlite3](https://sqlite.org)

*  [JSON for Modern C++](https://github.com/nlohmann/json)


## Started

### CMAKE

``` shell
$ cd calabash
$ mkdir build && cd build
$ cmake ..
$ make .
```

### CLion With CMAKE

> Just as a project.