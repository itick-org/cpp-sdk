#include <iostream>
#include "client.h"
#include <thread>
#include <chrono>

using namespace io::github::itick::sdk;

int main() {
    try {
        // 初始化客户端
        std::string token = "8850*****************ee4127087";
        Client client(token);

        // 设置 WebSocket 消息处理器
        client.setMessageHandler([](const std::string& message) {
            std::cout << "Received WebSocket message: " << message << std::endl;
        });

        // 设置 WebSocket 错误处理器
        client.setErrorHandler([](const std::string& error) {
            std::cerr << "WebSocket error: " << error << std::endl;
        });

        // 设置 WebSocket 打开处理器
        client.setOpenHandler([]() {
            std::cout << "WebSocket connected" << std::endl;
        });

        // 设置 WebSocket 关闭处理器
        client.setCloseHandler([]() {
            std::cout << "WebSocket closed" << std::endl;
        });

        // 测试外汇实时成交接口
        std::cout << "Testing forex tick..." << std::endl;
        std::string tick = client.getForexTick("GB", "EURUSD");
        std::cout << tick << std::endl;

        // 测试外汇实时报价接口
        std::cout << "\nTesting forex quote..." << std::endl;
        std::string quote = client.getForexQuote("GB", "EURUSD");
        std::cout << quote << std::endl;

        // 测试外汇实时盘口接口
        std::cout << "\nTesting forex depth..." << std::endl;
        std::string depth = client.getForexDepth("GB", "EURUSD");
        std::cout << depth << std::endl;

        // 测试外汇历史K线接口
        std::cout << "\nTesting forex kline..." << std::endl;
        std::string kline = client.getForexKline("GB", "EURUSD", 2, 10);
        std::cout << kline << std::endl;

        // 测试 WebSocket 连接
        std::cout << "\nTesting WebSocket..." << std::endl;
        client.connectForexWebSocket();

        // 发送订阅消息
        client.sendWebSocketMessage("{\"action\": \"subscribe\", \"codes\": [\"EURUSD\"]}");

        // 等待一段时间
        std::cout << "Waiting for WebSocket messages..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // 检查连接状态
        std::cout << "WebSocket connected: " << (client.isWebSocketConnected() ? "true" : "false") << std::endl;

        // 关闭 WebSocket
        client.closeWebSocket();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
