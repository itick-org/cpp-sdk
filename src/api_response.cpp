#include "api_response.h"

using namespace io::github::itick::sdk;

ApiResponse::ApiResponse(const std::string& j) {
    // Simple JSON parsing (placeholder)
    // In a real implementation, you would use a proper JSON parser
    code = 0;
    msg = "";
    data = j;
}

int ApiResponse::getCode() const {
    return code;
}

std::string ApiResponse::getMsg() const {
    return msg;
}

std::string ApiResponse::getData() const {
    return data;
}
