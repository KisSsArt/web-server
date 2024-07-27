#pragma once

#include <string>
#include <map>

#include "config.h"

namespace http
{
    inline std::string httpEmptyHeader =  "HTTP/ \r\n"
                                    "Content-Type: text/html\r\n"
                                    "\r\n";

    /*
        HTTP status codes
    */
    enum HTTP_STATUS : int {
        // 1xx: Info

        // 2xx: Success
        HTTP_OK = 200,
        // 3xx: Redirection

        // 4xx: Client Error
        HTTP_BAD_REQUEST = 400,
        HTTP_NOT_FOUND = 404,
        // 5xx: Server Error
        HTTP_VERSION_NOT_SUPPORTED = 505,
    };

    inline std::map<HTTP_STATUS, std::string> statusStringMap{
        {HTTP_OK, "200 OK"},
        {HTTP_BAD_REQUEST, "400 Bad Request"},
        {HTTP_NOT_FOUND, "404 Not Found"},
    };

    /*
        HTTP versions
    */
    enum HTTP_ver : int {
        HTTP_10 = 0,
        HTTP_11 = 1
    };

    /*
        HTTP method
    */
    enum HTTP_met : int {
        GET = 0,
        POST = 1
    };

    class HTTPData {
    public:
        HTTPData() : valid{true}, method{GET}, file{DEFAULT_FILE_NAME}, version{HTTP_10} {}

        HTTPData(std::string packet)
        {
            this->setValid(true);
            this->ParsePacket(packet);
        }

        HTTPData* ParsePacket(std::string packet);
        char* CreateResponsePacket();
        char* CreateResponseHeader(char* response, HTTP_ver version, HTTP_STATUS status);

        bool getValid() { return this->valid; }
        HTTP_met getHTTPMethod() { return this->method; }
        std::string getFile() { return this->file; }
        HTTP_ver getHTTPVersion() { return this->version; }

        void setValid(bool valid) { this->valid = valid; }
        void setHTTPMethod(HTTP_met method) { this->method = method; }
        void setFile(std::string file) { this->file = file; }
        void setHTTPVersion(HTTP_ver version) { this->version = version; }

    private:
        bool valid;
        HTTP_met method;
        std::string file;
        HTTP_ver version;
    };
}