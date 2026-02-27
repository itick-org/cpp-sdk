#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <map>
#include <vector>
#include <curl/curl.h>
#include <functional>
#include "api_response.h"

namespace io {
namespace github {
namespace itick {
namespace sdk {

class Client {
private:
    std::string token;
    const std::string baseURL = "https://api.itick.org";
    const std::string wssURL = "wss://api.itick.org";
    CURL* curl;
    
    // WebSocket members
    std::string wsPath;
    bool isRunning;
    bool isConnected;
    int reconnectAttempts;
    
    // WebSocket constants
    static const int PING_INTERVAL = 30000; // 30 seconds
    static const int RECONNECT_INTERVAL = 5000; // 5 seconds
    static const int MAX_RECONNECT_ATTEMPTS = 10;
    
    // Callbacks
    std::function<void(const std::string&)> messageHandler;
    std::function<void(const std::string&)> errorHandler;
    std::function<void()> openHandler;
    std::function<void()> closeHandler;

public:
    Client(const std::string& token);
    ~Client();

    ApiResponse get(const std::string& path, const std::map<std::string, std::string>& params);
    
    // WebSocket methods with enhanced functionality
    void setMessageHandler(std::function<void(const std::string&)> handler);
    void setErrorHandler(std::function<void(const std::string&)> handler);
    void setOpenHandler(std::function<void()> handler);
    void setCloseHandler(std::function<void()> handler);
    
    void connectWebSocket(const std::string& path);
    void sendWebSocketMessage(const std::string& message);
    void closeWebSocket();
    bool isWebSocketConnected() const;



    // 基础模块
    std::string getSymbolList();
    std::string getSymbolHolidays();

    // 股票模块
    std::string getStockInfo(const std::string& market, const std::string& code);
    std::string getStockIPO(const std::string& market, const std::string& code);
    std::string getStockSplit(const std::string& market, const std::string& code);
    std::string getStockTick(const std::string& market, const std::string& code);
    std::string getStockQuote(const std::string& market, const std::string& code);
    std::string getStockDepth(const std::string& market, const std::string& code);
    std::string getStockKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getStockTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getStockQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getStockDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getStockKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectStockWebSocket();

    // 指数模块
    std::string getIndicesTick(const std::string& market, const std::string& code);
    std::string getIndicesQuote(const std::string& market, const std::string& code);
    std::string getIndicesDepth(const std::string& market, const std::string& code);
    std::string getIndicesKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getIndicesTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getIndicesQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getIndicesDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getIndicesKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectIndicesWebSocket();

    // 期货模块
    std::string getFutureTick(const std::string& market, const std::string& code);
    std::string getFutureQuote(const std::string& market, const std::string& code);
    std::string getFutureDepth(const std::string& market, const std::string& code);
    std::string getFutureKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getFutureTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getFutureQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getFutureDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getFutureKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectFutureWebSocket();

    // 基金模块
    std::string getFundTick(const std::string& market, const std::string& code);
    std::string getFundQuote(const std::string& market, const std::string& code);
    std::string getFundDepth(const std::string& market, const std::string& code);
    std::string getFundKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getFundTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getFundQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getFundDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getFundKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectFundWebSocket();

    // 外汇模块
    std::string getForexTick(const std::string& market, const std::string& code);
    std::string getForexQuote(const std::string& market, const std::string& code);
    std::string getForexDepth(const std::string& market, const std::string& code);
    std::string getForexKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getForexTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getForexQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getForexDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getForexKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectForexWebSocket();

    // 加密货币模块
    std::string getCryptoTick(const std::string& market, const std::string& code);
    std::string getCryptoQuote(const std::string& market, const std::string& code);
    std::string getCryptoDepth(const std::string& market, const std::string& code);
    std::string getCryptoKline(const std::string& market, const std::string& code, int period, int limit, long long end = 0);
    std::string getCryptoTicks(const std::string& market, const std::vector<std::string>& codes);
    std::string getCryptoQuotes(const std::string& market, const std::vector<std::string>& codes);
    std::string getCryptoDepths(const std::string& market, const std::vector<std::string>& codes);
    std::string getCryptoKlines(const std::string& market, const std::vector<std::string>& codes, int period, int limit, long long end = 0);
    void connectCryptoWebSocket();

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* s);
    std::string buildURL(const std::string& path, const std::map<std::string, std::string>& params);
};

} // namespace sdk
} // namespace itick
} // namespace github
} // namespace io

#endif // CLIENT_H
