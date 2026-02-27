#ifndef API_RESPONSE_H
#define API_RESPONSE_H

#include <string>

namespace io {
namespace github {
namespace itick {
namespace sdk {

class ApiResponse {
private:
    int code;
    std::string msg;
    std::string data;

public:
    ApiResponse() : code(0) {}
    ApiResponse(const std::string& j);

    int getCode() const;
    std::string getMsg() const;
    std::string getData() const;
};

} // namespace sdk
} // namespace itick
} // namespace github
} // namespace io

#endif // API_RESPONSE_H
