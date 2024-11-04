#include <iostream>

void printColoredMessage(const std::string& message, const std::string& color) {
    std::cout << color << message << "\033[0m" << std::endl; // 重置颜色
}

int main() {
    // 普通文本输出
    printColoredMessage("Hello, World!", "\033[0m"); // 默认颜色
    std::cout << std::endl;

    // 各种颜色输出
    printColoredMessage("Hello, World!", "\033[31m"); // 红色
    printColoredMessage("Hello, World!", "\033[32m"); // 绿色
    printColoredMessage("Hello, World!", "\033[33m"); // 黄色
    printColoredMessage("Hello, World!", "\033[34m"); // 蓝色
    printColoredMessage("Hello, World!", "\033[35m"); // 紫色
    printColoredMessage("Hello, World!", "\033[36m"); // 青色
    printColoredMessage("Hello, World!", "\033[37m"); // 白色
    std::cout << std::endl;

    // 亮色输出
    printColoredMessage("Hello, World!", "\033[1;31m"); // 亮红色
    printColoredMessage("Hello, World!", "\033[1;32m"); // 亮绿色
    printColoredMessage("Hello, World!", "\033[1;33m"); // 亮黄色
    printColoredMessage("Hello, World!", "\033[1;34m"); // 亮蓝色
    printColoredMessage("Hello, World!", "\033[1;35m"); // 亮紫色
    printColoredMessage("Hello, World!", "\033[1;36m"); // 亮青色
    printColoredMessage("Hello, World!", "\033[1;37m"); // 亮白色
    std::cout << std::endl;

    // 背景色输出
    printColoredMessage("Hello, World!", "\033[40m"); // 背景黑色
    printColoredMessage("Hello, World!", "\033[41m"); // 背景红色
    printColoredMessage("Hello, World!", "\033[42m"); // 背景绿色
    printColoredMessage("Hello, World!", "\033[43m"); // 背景黄色
    printColoredMessage("Hello, World!", "\033[44m"); // 背景蓝色
    printColoredMessage("Hello, World!", "\033[45m"); // 背景紫色
    printColoredMessage("Hello, World!", "\033[46m"); // 背景青色
    printColoredMessage("Hello, World!", "\033[47m"); // 背景白色
    std::cout << std::endl;

    // 亮色背景输出
    printColoredMessage("Hello, World!", "\033[1;40m"); // 亮背景黑色
    printColoredMessage("Hello, World!", "\033[1;41m"); // 亮背景红色
    printColoredMessage("Hello, World!", "\033[1;42m"); // 亮背景绿色
    printColoredMessage("Hello, World!", "\033[1;43m"); // 亮背景黄色
    printColoredMessage("Hello, World!", "\033[1;44m"); // 亮背景蓝色
    printColoredMessage("Hello, World!", "\033[1;45m"); // 亮背景紫色
    printColoredMessage("Hello, World!", "\033[1;46m"); // 亮背景青色
    printColoredMessage("Hello, World!", "\033[1;47m"); // 亮背景白色
    std::cout << std::endl;

    // 粗体输出
    printColoredMessage("Hello, World!", "\033[1m"); // 粗体
    std::cout << std::endl;
    // 下划线输出
    printColoredMessage("Hello, World!", "\033[4m"); // 下划线
    std::cout << std::endl;

    return 0;
}
