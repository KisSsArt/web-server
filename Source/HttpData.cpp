#include "HttpData.h"

#include <fstream>

using namespace http;

HTTPData* HTTPData::ParsePacket(std::string packet)
{
    size_t currentPos;

    size_t firstLinePos = packet.find('\r');
    if (firstLinePos == std::string::npos)
    {
        this->setValid(false);
        return this;
    }

    std::string firstLine = packet.substr(0, firstLinePos);

    // method
    currentPos = firstLine.find("GET");
    if (currentPos != std::string::npos)
    {
        this->setHTTPMethod(GET);
    }
    else
    {
        currentPos = firstLine.find("POST");
        if (currentPos != std::string::npos)
            this->setHTTPMethod(POST);
        else
            this->setValid(false);
    }

    // filename
    std::string fileName(DEFAULT_SITE_DIRECTORY);
    size_t directoryNameSize = fileName.size();
    currentPos = firstLine.find("/");
    if (currentPos == std::string::npos)
    {
        fileName += DEFAULT_FILE_NAME;
    }
    else
    {
        size_t endNamePos = firstLine.find(' ', currentPos);
        if (endNamePos == std::string::npos)
        {
            this->setValid(false);
            return this;
        }
        else
        {
            if (endNamePos - currentPos > 1)
            {
                fileName += firstLine.substr(currentPos + 1, (endNamePos - currentPos - 1));
                size_t _pos = fileName.find('?');
                if (_pos != std::string::npos)
                    fileName = fileName.substr(0, _pos);
            }
            else
                fileName += DEFAULT_FILE_NAME;
        }
        currentPos = endNamePos;
    }

    this->setFile(fileName);

    // mime
    size_t dot = fileName.find('.', directoryNameSize);
    if (dot == std::string::npos)
        this->setMime(mimeTypeMap["html"]);
    else
        this->setMime(mimeTypeMap[fileName.substr(dot + 1)]);

    // http version
    currentPos = firstLine.find("HTTP/1.1");
    if (currentPos != std::string::npos)
    {
        this->setHTTPVersion(HTTP_11);
    }
    else
    {
        currentPos = firstLine.find("HTTP/1.0");
        if (currentPos != std::string::npos)
            this->setHTTPVersion(HTTP_10);
        else
            // 505 - HTTP Version Not Supported / HTTP_VERSION_NOT_SUPPORTED
            this->setValid(false);
    }

    return this;
}

char* HTTPData::CreateResponsePacket()
{
    char* response = new char[1024];
    std::memset(response, '\0', 1024);

    if (!this->getValid())
        return CreateResponseHeader(response, HTTP_BAD_REQUEST);

    std::ifstream file(this->getFile());
    if (!file.is_open())
        return CreateResponseHeader(response, HTTP_NOT_FOUND);

    CreateResponseHeader(response, HTTP_OK);

    file.read(&(response[std::strlen(response)]), (1024 - std::strlen(response)));
    file.close();
    return response;
}

char* HTTPData::CreateResponseHeader(char* response, HTTP_STATUS status)
{
    std::string httpHeader("HTTP/");
    httpHeader += (this->getHTTPVersion() == HTTP_10) ? "1.0 " : "1.1 ";
    httpHeader += statusStringMap[status] + "\r\n";
    httpHeader += "Content-Type: " + this->getMime() + "\r\n";
    httpHeader += "\r\n";

    std::strcpy(response, httpHeader.c_str());
    return response;
}