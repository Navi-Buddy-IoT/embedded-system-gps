#pragma once
#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

const char* login(const char* email = "iot@mail.com", const char* password = "123456");
int sendUpdateRequest(const char* requestBody, const char* authToken, int resourceId = 1);
void checkResponseCode(int httpResponseCode);
#endif