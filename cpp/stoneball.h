#ifndef __STONEBALL_H__
#define __STONEBALL_H__

#include "android.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void javaDrawGameText(int x, int y, const wchar_t* text, int length, bool strikethrough, int fontSize, int r, int g, int b);
//void javaDrawGameText(int x, int y, const char* text, bool strikethrough, int fontSize);
void javaGameTextSize(const wchar_t* text, int length, int fontSize, int* width, int* height);
void javaSetFontBold(bool bold);
void javaOpenWebBrowser(const char* text);

#ifdef __cplusplus
}
#endif

#endif // __STONEBALL_H__
