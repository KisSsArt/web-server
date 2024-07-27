#pragma once

#include <string>
#include "http.h"
#include "config.h"

class HTTPData {
public:
    HTTPData() : valid{true},method{GET},file{DEFAULT_FILE_NAME},version{HTTP_10} {} 

    HTTPData(std::string packet) { this->setValid(true); this->ParsePacket(packet); }

    HTTPData* ParsePacket(std::string packet);

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