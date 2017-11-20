#ifndef WSTRING_H
#define WSTRING_H
#include <wchar.h>
//#define wnull ((wstring)0)
//typedef struct {} *wstring;

typedef enum{wfalse,wtrue}wbool;
typedef struct wstring_s *wstring;
struct wstring_s
{
	void    (*append)(wstring s);
	void*   (*insert)(wstring s,int i);
	wchar_t (*at)(unsigned int index);
	int     (*compare)(wstring s);
	int     (*find)(wstring s);
	int     (*length)(void);
	wstring (*subString)(unsigned int begin,unsigned int end);
	wbool   (*startWith)(wstring s);
	wbool   (*endWith)(wstring s);

	char*   (*toAnsi)();
	wchar_t*(*toUtf16)();
};

#ifndef W_INTERNAL
extern const struct wstring_s *const wnull;

#endif
//extern const _string*const wnull;
extern wstring  WA(const char*s);
extern wstring  WW(const wchar_t*s);
extern void     delete_wstring(wstring *inst);
extern wstring  new_wstring(wstring src);
extern void     freeTmpBuf(void);
extern void     initwstring();
#endif // WSTRING_H
