# Calabash
[中文README](https://github.com/Taiga-A/Calabash/blob/master/README_ZH.md)

A Self-sufficient web frame for linux only, used Sqlite3 database.

*stdc++ version must higher than C++17* 


## Module

* util
  * Base64 MD5
  * data reader, Stream and string data mixed reader, used for send file
  * object pool, A header only object pool
  * logger, A log system with `SYSTEM` `INFO` `WARNING` `ERROR` four level.
  * token, A simple password token class with time and user for web.
* thread
  
  * thread pool, A header only thread pool with task queue.
* socket
  
  * Encapsulated linux socket api with epoll 
* web
  * server, Http server
  * request and request maker, Read socket data to request.
  * response, Http response link util/data_reader
  * http task, Http main logic.
  * exception, http event to send responce.
* database
  
  * sqlite3, Carefully encapsulated sqlite api, compiled bind sql...
  
  

## Third party

*  [GoogleTest](https://github.com/google/googletest)

*  [Sqlite3](https://sqlite.org)

*  [JSON for Modern C++](https://github.com/nlohmann/json)

## Install

### CMAKE

``` shell
$ cd calabash
$ mkdir build && cd build
$ cmake ..
$ make .
```

### CLion With CMAKE

> Just as a project.



## Start





