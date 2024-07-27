#pragma once

#include <map>
#include <string>

inline std::string httpEmptyHeader =  "HTTP/ \r\n"
                                    "Content-Type: text/html\r\n"
                                    "\r\n";

/*
    HTTP status codes
*/
enum HTTP_STATUS : int {
//1xx: Info

//2xx: Success
    HTTP_OK = 200,
//3xx: Redirection

//4xx: Client Error
    HTTP_BAD_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
//5xx: Server Error
    HTTP_VERSION_NOT_SUPPORTED = 505,
};

inline std::map<HTTP_STATUS, std::string> statusStringMap {
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