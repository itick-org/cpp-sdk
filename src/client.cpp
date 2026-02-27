#include "client.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace io::github::itick::sdk;

Client::Client(const std::string& token) : token(token), isRunning(false), isConnected(false), reconnectAttempts(0) {
    curl = curl_easy_init();
}

Client::~Client() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    closeWebSocket();
}

// WebSocket callback setters
void Client::setMessageHandler(std::function<void(const std::string&)> handler) {
    messageHandler = handler;
}

void Client::setErrorHandler(std::function<void(const std::string&)> handler) {
    errorHandler = handler;
}

void Client::setOpenHandler(std::function<void()> handler) {
    openHandler = handler;
}

void Client::setCloseHandler(std::function<void()> handler) {
    closeHandler = handler;
}

bool Client::isWebSocketConnected() const {
    return isConnected;
}

size_t Client::writeCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}

std::string Client::buildURL(const std::string& path, const std::map<std::string, std::string>& params) {
    std::string url = baseURL + path;
    if (!params.empty()) {
        url += "?";
        for (const auto& param : params) {
            url += param.first + "=" + param.second + "&";
        }
        url.pop_back(); // Remove the trailing '&'
    }
    return url;
}

ApiResponse Client::get(const std::string& path, const std::map<std::string, std::string>& params) {
    std::string url = buildURL(path, params);
    std::string responseString;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, ("accept: application/json")));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, ("token: " + token).c_str()));

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return ApiResponse(responseString);
}

void Client::connectWebSocket(const std::string& path) {
    this->wsPath = path;
    this->isRunning = true;
    this->isConnected = false;
    this->reconnectAttempts = 0;
    
    std::string url = wssURL + path;
    std::cout << "Connecting to WebSocket: " << path << std::endl;
    
    // Simulate connection (actual WebSocket implementation would go here)
    // For now, we'll just call the open handler
    if (openHandler) {
        openHandler();
    }
    isConnected = true;
}

void Client::sendWebSocketMessage(const std::string& message) {
    if (!isConnected) {
        if (errorHandler) {
            errorHandler("WebSocket not connected");
        }
        return;
    }
    
    std::cout << "Sending WebSocket message: " << message << std::endl;
    
    // Simulate message sending (actual WebSocket implementation would go here)
}

void Client::closeWebSocket() {
    if (isRunning) {
        isRunning = false;
        isConnected = false;
        
        if (closeHandler) {
            closeHandler();
        }
        std::cout << "Closing WebSocket" << std::endl;
    }
}

// Forex API
std::string Client::getForexTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/forex/tick", params);
    return response.getData();
}

std::string Client::getForexQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/forex/quote", params);
    return response.getData();
}

std::string Client::getForexDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/forex/depth", params);
    return response.getData();
}

std::string Client::getForexKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/forex/kline", params);
    return response.getData();
}

std::string Client::getForexTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/forex/ticks", params);
    return response.getData();
}

std::string Client::getForexQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/forex/quotes", params);
    return response.getData();
}

std::string Client::getForexDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/forex/depths", params);
    return response.getData();
}

std::string Client::getForexKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/forex/klines", params);
    return response.getData();
}

void Client::connectForexWebSocket() {
    connectWebSocket("/websocket/forex");
}

// Stock API
std::string Client::getStockTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/tick", params);
    return response.getData();
}

std::string Client::getStockQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/quote", params);
    return response.getData();
}

std::string Client::getStockDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/depth", params);
    return response.getData();
}

std::string Client::getStockKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/stock/kline", params);
    return response.getData();
}

std::string Client::getStockTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/stock/ticks", params);
    return response.getData();
}

std::string Client::getStockQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/stock/quotes", params);
    return response.getData();
}

std::string Client::getStockDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/stock/depths", params);
    return response.getData();
}

std::string Client::getStockKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/stock/klines", params);
    return response.getData();
}

void Client::connectStockWebSocket() {
    connectWebSocket("/websocket/stock");
}

// Crypto API
std::string Client::getCryptoTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/crypto/tick", params);
    return response.getData();
}

std::string Client::getCryptoQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/crypto/quote", params);
    return response.getData();
}

std::string Client::getCryptoDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/crypto/depth", params);
    return response.getData();
}

std::string Client::getCryptoKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/crypto/kline", params);
    return response.getData();
}

std::string Client::getCryptoTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/crypto/ticks", params);
    return response.getData();
}

std::string Client::getCryptoQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/crypto/quotes", params);
    return response.getData();
}

std::string Client::getCryptoDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/crypto/depths", params);
    return response.getData();
}

std::string Client::getCryptoKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/crypto/klines", params);
    return response.getData();
}

// 基础模块
std::string Client::getSymbolList() {
    std::map<std::string, std::string> params;
    ApiResponse response = get("/symbol/list", params);
    return response.getData();
}

std::string Client::getSymbolHolidays() {
    std::map<std::string, std::string> params;
    ApiResponse response = get("/symbol/holidays", params);
    return response.getData();
}

// 股票模块
std::string Client::getStockInfo(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/info", params);
    return response.getData();
}

std::string Client::getStockIPO(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/ipo", params);
    return response.getData();
}

std::string Client::getStockSplit(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/stock/split", params);
    return response.getData();
}

// 指数模块
std::string Client::getIndicesTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/indices/tick", params);
    return response.getData();
}

std::string Client::getIndicesQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/indices/quote", params);
    return response.getData();
}

std::string Client::getIndicesDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/indices/depth", params);
    return response.getData();
}

std::string Client::getIndicesKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/indices/kline", params);
    return response.getData();
}

std::string Client::getIndicesTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/indices/ticks", params);
    return response.getData();
}

std::string Client::getIndicesQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/indices/quotes", params);
    return response.getData();
}

std::string Client::getIndicesDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/indices/depths", params);
    return response.getData();
}

std::string Client::getIndicesKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/indices/klines", params);
    return response.getData();
}

void Client::connectIndicesWebSocket() {
    connectWebSocket("/websocket/indices");
}

// 期货模块
std::string Client::getFutureTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/future/tick", params);
    return response.getData();
}

std::string Client::getFutureQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/future/quote", params);
    return response.getData();
}

std::string Client::getFutureDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/future/depth", params);
    return response.getData();
}

std::string Client::getFutureKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/future/kline", params);
    return response.getData();
}

std::string Client::getFutureTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/future/ticks", params);
    return response.getData();
}

std::string Client::getFutureQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/future/quotes", params);
    return response.getData();
}

std::string Client::getFutureDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/future/depths", params);
    return response.getData();
}

std::string Client::getFutureKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/future/klines", params);
    return response.getData();
}

void Client::connectFutureWebSocket() {
    connectWebSocket("/websocket/future");
}

// 基金模块
std::string Client::getFundTick(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/fund/tick", params);
    return response.getData();
}

std::string Client::getFundQuote(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/fund/quote", params);
    return response.getData();
}

std::string Client::getFundDepth(const std::string& region, const std::string& code) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    ApiResponse response = get("/fund/depth", params);
    return response.getData();
}

std::string Client::getFundKline(const std::string& region, const std::string& code, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    params["code"] = code;
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/fund/kline", params);
    return response.getData();
}

std::string Client::getFundTicks(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/fund/ticks", params);
    return response.getData();
}

std::string Client::getFundQuotes(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/fund/quotes", params);
    return response.getData();
}

std::string Client::getFundDepths(const std::string& region, const std::vector<std::string>& codes) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    ApiResponse response = get("/fund/depths", params);
    return response.getData();
}

std::string Client::getFundKlines(const std::string& region, const std::vector<std::string>& codes, int kType, int limit, long long et) {
    std::map<std::string, std::string> params;
    params["region"] = region;
    std::stringstream ss;
    for (size_t i = 0; i < codes.size(); ++i) {
        ss << codes[i];
        if (i < codes.size() - 1) {
            ss << ",";
        }
    }
    params["codes"] = ss.str();
    params["kType"] = std::to_string(kType);
    params["limit"] = std::to_string(limit);
    if (et > 0) {
        params["et"] = std::to_string(et);
    }
    ApiResponse response = get("/fund/klines", params);
    return response.getData();
}

void Client::connectFundWebSocket() {
    connectWebSocket("/websocket/fund");
}

void Client::connectCryptoWebSocket() {
    connectWebSocket("/websocket/crypto");
}


