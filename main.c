//#include <windows.h>
#include <stdio.h>
#include "wstring.h"
#include <stdlib.h>
#include <locale.h>
/*
void wcstest() {
    extern size_t _M_mbyteToWChar(const char *, wchar_t **);
    extern size_t _M_wcharToMByte(const wchar_t *, char **);
    //char text[10];
    //wchar_t wtext[10];
    char *text;
    wchar_t *wtext;
    _M_wcharToMByte(L"1234", &text);
    _M_mbyteToWChar("侧丧气", &wtext);
    printf("%s, %ls\n", text, wtext);
    //puts(text);
}
*/
int main(int argc,char**argv) {
    initwstring();
    setlocale(LC_ALL,"");
    wstring s=new_wstring(WW(L"大明湖"));
    s->append(WA("湖明大"));
    printf("toAnsi: %s\n", s->toAnsi());
    printf("toUtf16: %ls\n", s->toUtf16());
    printf("%ls\n",WA("大明湖里有荷花")->subString(3, 6)->toUtf16());
    return 0;
}
