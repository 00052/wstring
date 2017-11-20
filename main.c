//#include <windows.h>
#include <stdio.h>
#include "wstring.h"
#include <stdlib.h>
#include <locale.h>
#include "tmpbuftable.h"

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

void func1() {
//string a;
//std::string.find()
    initwstring();
    //WA("woaaaÎÒÎÒÎÒ")->toAnsi();
    //setlocale(LC_ALL,"");
    //WA("ÎÒÎÒ");
    //char *a,*b,*c,*d,*e,*f;

    //char buf[1000];
    setlocale(LC_ALL,"");

    wstring s=new_wstring(WW(L"012345678"));
    //s->append(WA("ÎÒ"));
    printf("init\n");
    printf("startWith: %d\n", s->startWith(WA("123")));
    printf("startWith: %d\n", s->startWith(WA("01")));
    printf("startWith: %d\n", s->startWith(WA("013")));
    //s->partest(1, 2, 3);
    printf("toAnsi: %s\n", s->toAnsi());
    //
    printf("subString: %s\n", s->subString(1, 5)->toAnsi());
    printf("toUtf16: %ls\n", s->toUtf16());

    //puts(buf);
    printf("%ls\n",WA("你好")->toUtf16());

    //puts(WA("ÎÒÎÒ")->toUtf16());
    printf("%ls\n",WA("我是中国人，我爱你中国")->subString(3, 6)->toUtf16(), s->subString(2, 4)->toUtf16(), 0);
    /*
    str[0]=0xFEFF;
    str[1]=0x6211;
    str[2]=0;
    */
    //wprintf(str);
    //char *text1 = "11ÎÒÎÒ22";
    //wchar_t wtext1[12];

    //printf("%d\n",wcslen(L"ÄãËû"));
    //printf("%d\n",strlen("ÎÒÄãabcËû"));
    /*
    ix_string s1=new_ix_string("I'm ");
    s1->append("Ixil.");
    puts(s1->c_str());

    ix_string s2=new_ix_string("I speak for myself.");
    puts(s2->c_str());

    delete_ix_string(&s1);
    delete_ix_string(&s2);
    */
    //return 0;
}

int main() {
    //wcstest();
    func1();
    puts("OK");
    return 0;
}
