#ifndef _BASE64_H
#define _BASE64_H

char* base64_encode(const char* src, int len);
char* base64_decode(const char* src, int* len);

#endif
