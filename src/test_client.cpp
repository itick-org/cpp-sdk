#include "client.h"
#include <iostream>
#include <vector>

using namespace io::github::itick::sdk;

int main() {
    // 使用真实API密钥
    std::string token = "8850*****************ee4127087";
    Client client(token);

    // 测试基础模块
    std::cout << "=== 测试基础模块 ===" << std::endl;
    try {
        std::string symbolList = client.getSymbolList();
        std::cout << "getSymbolList 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getSymbolList 错误: " << e.what() << std::endl;
    }

    try {
        std::string symbolHolidays = client.getSymbolHolidays();
        std::cout << "getSymbolHolidays 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getSymbolHolidays 错误: " << e.what() << std::endl;
    }

    // 测试股票模块
    std::cout << "\n=== 测试股票模块 ===" << std::endl;
    try {
        std::string stockInfo = client.getStockInfo("us", "AAPL");
        std::cout << "getStockInfo 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getStockInfo 错误: " << e.what() << std::endl;
    }

    try {
        std::string stockTick = client.getStockTick("us", "AAPL");
        std::cout << "getStockTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getStockTick 错误: " << e.what() << std::endl;
    }

    // 测试指数模块
    std::cout << "\n=== 测试指数模块 ===" << std::endl;
    try {
        std::string indicesTick = client.getIndicesTick("us", "SPX");
        std::cout << "getIndicesTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getIndicesTick 错误: " << e.what() << std::endl;
    }

    // 测试期货模块
    std::cout << "\n=== 测试期货模块 ===" << std::endl;
    try {
        std::string futureTick = client.getFutureTick("us", "ES");
        std::cout << "getFutureTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getFutureTick 错误: " << e.what() << std::endl;
    }

    // 测试基金模块
    std::cout << "\n=== 测试基金模块 ===" << std::endl;
    try {
        std::string fundTick = client.getFundTick("us", "SPY");
        std::cout << "getFundTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getFundTick 错误: " << e.what() << std::endl;
    }

    // 测试外汇模块
    std::cout << "\n=== 测试外汇模块 ===" << std::endl;
    try {
        std::string forexTick = client.getForexTick("forex", "EURUSD");
        std::cout << "getForexTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getForexTick 错误: " << e.what() << std::endl;
    }

    // 测试加密货币模块
    std::cout << "\n=== 测试加密货币模块 ===" << std::endl;
    try {
        std::string cryptoTick = client.getCryptoTick("crypto", "BTCUSD");
        std::cout << "getCryptoTick 成功" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "getCryptoTick 错误: " << e.what() << std::endl;
    }

    // 测试WebSocket
    std::cout << "\n=== 测试WebSocket ===" << std::endl;
    try {
        client.connectStockWebSocket();
        std::cout << "connectStockWebSocket 成功" << std::endl;
        client.closeWebSocket();
    } catch (const std::exception& e) {
        std::cout << "connectStockWebSocket 错误: " << e.what() << std::endl;
    }

    std::cout << "\n所有测试完成" << std::endl;
    return 0;
}
