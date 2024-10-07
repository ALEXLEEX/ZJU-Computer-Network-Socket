# SERVER

编写人：李秋宇

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

### 类封装

父类`server`，子类根据服务类型分为`TCPserver`和`UDPserver`
