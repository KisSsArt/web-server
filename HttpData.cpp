#include "HttpData.h"

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

    std::string fileName;
    currentPos = firstLine.find("/");
    if (currentPos == std::string::npos)
    {
        this->setFile(DEFAULT_FILE_NAME);
        this->setHTTPVersion(HTTP_11);
        return this;
    }
    else
    {
        size_t endNamePos = firstLine.find(' ', currentPos);
        if (endNamePos < 0)
        {
            this->setValid(false);
            return this;
        }
        else
        {
            if (endNamePos - currentPos > 1)
            {
                fileName = firstLine.substr(currentPos + 1, (endNamePos - currentPos - 1));
                size_t _pos = fileName.find('?');
                if (_pos >= 0)
                    fileName = fileName.substr(0, _pos);
            }
            else
                fileName = DEFAULT_FILE_NAME;
        }
        currentPos = endNamePos;
    }

    this->setFile(fileName);

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