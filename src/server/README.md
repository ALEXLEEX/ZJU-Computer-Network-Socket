# SERVER

Author: [lqy](https://github.com/lEEExp3rt)

---

## TODOs

- [x] 客户端ID：队列的下标
- [x] `Server_TCP::process`中，连接上后和断开连接后的消息提示
- [x] 关闭server时关闭所有连接
- [x] `Server_UDP::saveConnectInfo`修复
- [ ] 客户端ID分配
- [ ] 客户端队列满的情况
- [ ] 接收到客户端的指令分析与对应响应
- [ ] 协议包设计：信息编码与解码，设计一个协议包的类
- [ ] 更多样式的server控制台输出
- [x] `server.h`与`server.cpp`实现，`main.cpp`完成最终程序

---

## PROCESS

1. 初始化
   1. 创建套接字`init`
   2. 修改套接字选项`setOption`
2. 绑定服务端IP和端口`bindAddress`
3. 若为TCP，则开启监听线程并等待`startListen`；否则不开启，转4
4. 开启服务端主线程`startSocketThread`，创造服务端主线程`Thread_Socket`，调用工作函数`worker`

### TCPworker

1. 等待客户端连接`waitAndAccept`
2. 连接后开启对应客户端的线程`startClientThread`，创建子线程`Thread_Client`，调用TCP过程函数`process`，开始聊天
3. 连接断开后，关闭socket，记录客户端连接信息

### UDPworker

1. 不断接收消息，并打印信息
2. 直至没有消息后,关闭socket，保存连接客户端的信息（TODO）

## IDEA

### 信息输出

所有的输出全部使用`printMessage`，且输出显示为不同颜色

要在 C++ 中更改输出文字的颜色，可以使用 ANSI 转义序列。这种方法在终端支持 ANSI 的情况下能够实现彩色输出。在你的代码中，可以在 `serverError` 函数中添加对输出颜色的支持。例如，加上一些代码以改变错误信息的颜色。以下是一个简单的实现示例：

```cpp
void serverError(std::string message)
{
    // ANSI 转义序列：红色文本
    std::cerr << "\033[31m[Server] " << message << "\033[0m" << std::endl;
    exit(-1);
}
```

在这个示例中，`"\033[31m"` 用于设置文本颜色为红色，`"\033[0m"` 用于重置文本颜色为默认。你可以调整这些转义序列以实现不同的颜色效果。例如：

- 红色：`\033[31m`
- 绿色：`\033[32m`
- 黄色：`\033[33m`
- 蓝色：`\033[34m`
- 紫色：`\033[35m`
- 青色：`\033[36m`
- 白色：`\033[37m`

你可以在其他输出中也使用类似的方式，只需在需要的地方插入对应的 ANSI 转义序列即可。

### 类封装

父类`server`，子类根据服务类型分为`TCPserver`和`UDPserver`

两种类型的server都包含一个`serverSocket`和客户端连接信息`clientQueue`