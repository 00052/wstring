#define W_INTERNAL
#include "wstring.h"
#include "tmpbuftable.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#ifdef __linux__
#include <iconv.h>
#elif defined( __win32__)
#include <windows.h>
#endif

#include <stdio.h>

#ifdef __i386__
#define AFTER_SELF ,void*_Ign
#else
#define AFTER_SELF
#endif

#define STRING_ATTRIBUTE_NORMAL 0
#define STRING_ATTRIBUTE_TEMP   1
#define STRING_ATTRIBUTE_CONST  2
//////////////////////////////////////////////////////////////////
#define OFFSET(m,n) ((long)m-((long)n+sizeof(void*)*2+2));

#pragma pack(push,1)
typedef struct _RootFun
{
#if defined __ARMEL__
#if 0
	unsigned int sub_sp;
	unsigned int move_p4;
#endif
	unsigned int move_p3;
	unsigned int move_p2;
	unsigned int move_p1;
	unsigned int load_this;
#if 0
	unsigned int move_pc;
#endif
	unsigned int jump_to_func;
#if 0
	unsigned int add_sp;
	unsigned int move_lr;
#endif
	void *this_addr;
	void *func_addr;
#elif defined __i386__
	unsigned char push;
	long param;
	unsigned char call;
	void* funaddr;
	unsigned char add;
	unsigned short esp_4;
	unsigned char ret;
	unsigned long imm;
#else
#error "Unsupported arch"
#endif
} __attribute__((packed)) RootFun;
#pragma pack(pop)
RootFun* createRootFun(void *funAddr, long param)
{
	//fprintf(stderr, "createRootFun(%p, %p)\n", funAddr, (void *)param);
	RootFun *rootFun = (RootFun*)malloc(sizeof(RootFun));
#if defined __ARMEL__
#if 0
	rootFun->sub_sp = 0xe24dd004;			//	sub	sp, #4
	rootFun->move_p4 = 0xe58d3000;			//	str	r3, [sp]
#endif
	rootFun->move_p3 = 0xe1a03002;			//	mov	r3, r2
	rootFun->move_p2 = 0xe1a02001;			//	mov	r2, r1
	rootFun->move_p1 = 0xe1a01000;			//	mov	r1, r0
#if 0
	rootFun->load_this = 0xe59f000c;		//	ldr	r0, [pc, #12]
	rootFun->move_pc = 0xe1a0e00f;			//	mov	lr, pc
	rootFun->jump_to_func = 0xe59ff008;		//	ldr	pc, [pc, #8]
	rootFun->add_sp = 0xe28dd004;			//	add	sp, #4
	rootFun->move_lr = 0xe1a0f00e;			//	mov	pc, lr
#else
	rootFun->load_this = 0xe59f0000;		//	ldr	r0, [pc]
	rootFun->jump_to_func = 0xe59ff000;		//	ldr	pc, [pc]
#endif
	rootFun->this_addr = (void *)param;
	rootFun->func_addr = funAddr;
#elif defined __i386__
	void *addr;
	addr=(void*)OFFSET(funAddr,rootFun);
	rootFun->push=0x68;
	rootFun->param=param;
	rootFun->call=0xe8;
	rootFun->funaddr=addr;
	rootFun->add=0x83;
	rootFun->esp_4=0x04c4;
	rootFun->ret=0xc2;
	rootFun->imm=0;
#else
#error "Unsupported arch"
#endif
	return rootFun;
}
//////////////////////////////////////////////////////////////////

//"int8 int16 int32 uint8 uint16 uint32 char wstring astr wstr single double pointer |B D O X E 0.0";

typedef struct _string_stc _string;
struct _string_stc
{
	void        (*append)(wstring s);
	void        (*insert)(wstring s, int i);
	wchar_t     (*at)(int index);
	int         (*compare)(wstring s);
	int         (*find)(wstring s);
	int         (*length)(void);
	wstring     (*subString)(int begin, int end);
	wbool       (*startWith)(wstring s);
	wbool       (*endWith)(wstring s);
	const char* (*toAnsi)(void);
	const wchar_t*(*toUtf16)(void);
    
	void	(*partest)(int a, int b, int c);

	//wstring (*format)();
	//int     _length;
	char     _attrib;
	int      _size;
	wchar_t* _buffer;
};
wstring wnull;
void	append(_string* self AFTER_SELF,wstring s);
void	insert(_string* self AFTER_SELF,wstring s);
wchar_t	at(_string* self AFTER_SELF,unsigned int index);
int	compare(_string* self AFTER_SELF,wstring s);
int	find(_string* self AFTER_SELF,wstring s);
int	length(_string* self AFTER_SELF);
wstring	subString(_string* self AFTER_SELF,unsigned int begin,unsigned int end);
wbool	startWith(_string* self AFTER_SELF, wstring s);
wbool	endWith(_string* self AFTER_SELF, wstring s);

const char *toAnsi();
const wchar_t *toUtf16();

//void        freeTmpBuf();

//unsigned char * _M_temp_buffer=NULL;

_string*    _M_constructBaseStruct();
void        _M_copyStruct(_string** target,_string*src);
void        _M_moveStruct(_string** target,_string*src);
void        _M_deleteStruct(_string * pinst);
void        _M_checkUpStruct(_string* inst);
int         _M_strSizeW(const wchar_t *str);
int         _M_strSizeA(const char* str);

_string *_M_constructBaseStruct()
{
	_string*inst = (_string*)malloc(sizeof(_string));

	RootFun*rf_append	=	createRootFun(append,(long)inst);
	RootFun*rf_insert	=	createRootFun(insert,(long)inst);
	RootFun*rf_at		=	createRootFun(at,(long)inst);
	RootFun*rf_compare	=	createRootFun(compare,(long)inst);
	RootFun*rf_find		=	createRootFun(find,(long)inst);
	RootFun*rf_length	=	createRootFun(length,(long)inst);
	RootFun*rf_startWith	=	createRootFun(startWith,(long)inst);
	RootFun*rf_endWith	=	createRootFun(endWith,(long)inst);
	RootFun*rf_subString	=	createRootFun(subString,(long)inst);
	RootFun*rf_toAnsi	=	createRootFun(toAnsi,(long)inst);
	RootFun*rf_toUtf16	=	createRootFun(toUtf16,(long)inst);

	inst->append    =  (void(*)())          rf_append;
	inst->insert    =  (void(*)())         rf_insert;
	inst->at        =  (wchar_t(*)())       rf_at;
	inst->compare   =  (int(*)())           rf_compare;
	inst->find      =  (int(*)())           rf_find;
	inst->length    =  (int(*)())           rf_length;
	inst->startWith =  (wbool(*)())         rf_startWith;
	inst->endWith   =  (wbool(*)())         rf_endWith;
	inst->subString =  (wstring(*)())       rf_subString;
	inst->toAnsi    =  (const char*(*)())   rf_toAnsi;
	inst->toUtf16   =  (const wchar_t*(*)())rf_toUtf16;



	return inst;
}

void _M_copyStruct(_string** target, _string*src)
{
	*target=_M_constructBaseStruct();
	(*target)->_attrib=src->_attrib;
	(*target)->_size=src->_size;
	(*target)->_buffer=malloc(src->_size);

	memcpy((*target)->_buffer,src->_buffer,sizeof(src->_size));
}

void _M_moveStruct(_string** target, _string*src)
{
	*target=src;
}

void _M_deleteStruct(_string *inst)
{
	free(inst->append);
	free(inst->insert);
	free(inst->at);
	free(inst->compare);
	free(inst->find);
	free(inst->length);

	free(inst->_buffer);

	free(inst);
}

void _M_checkUpStruct(_string* inst)
{
	if(STRING_ATTRIBUTE_TEMP==inst->_attrib)
	{
		_M_deleteStruct(inst);
	}
}

int _M_strSizeW(const wchar_t*str)
{
	return (wcslen(str)+1) * sizeof(wchar_t);
}

int _M_strSizeA(const char *str)
{
	return strlen(str)+1;
}
/*
size_t _M_mbyteToWChar(const char* mbs, wchar_t**wcs)
{
	int minSize;
	minSize = MultiByteToWideChar (CP_ACP, 0, mbs, -1, NULL, 0);
	wchar_t *buf=malloc(minSize*sizeof(wchar_t));
	MultiByteToWideChar (CP_ACP, 0, mbs, -1, buf, minSize);
	//printf("buf=%hx %hx\n",buf[0],buf[1]);
	*wcs=buf;
	return minSize * sizeof(wchar_t);
}

size_t _M_wcharToMByte(const wchar_t* wcs, char**mbs)
{
	//fprintf(stderr, "Function: _M_wcharToMByte(%p, %p)\n", wcs, mbs);

	int minSize;
	minSize = WideCharToMultiByte(CP_ACP,0,wcs,-1,NULL,0,NULL,FALSE);
	*mbs=(char*)malloc(minSize);
	WideCharToMultiByte (CP_ACP, 0, wcs, -1, *mbs, minSize, NULL, 0);
	return minSize;
}
*/
//#include <wchar.h>
size_t _M_mbyteToWChar(const char* mbs, wchar_t**wcs)
{
    int minSize;
    //minSize = MultiByteToWideChar (CP_ACP, 0, mbs, -1, NULL, 0);
    minSize = mbstowcs(NULL,mbs,0) + 1;
	//printf("minsize: %d\n",minSize);
    wchar_t *buf=(wchar_t *)malloc(minSize*sizeof(wchar_t));
    mbstowcs(buf,mbs,minSize);
    //printf("ret:%ls\n",buf);
    //MultiByteToWideChar (CP_ACP, 0, mbs, -1, buf, minSize);
    //printf("buf=%hx %hx\n",buf[0],buf[1]);
    *wcs=buf;
    return minSize * sizeof(wchar_t);
}

size_t _M_wcharToMByte(const wchar_t* wcs, char**mbs)
{
    //fprintf(stderr, "Function: _M_wcharToMByte(%p, %p)\n", wcs, mbs);
    //for(int i=0;*(wcs+i);i++);
    int minSize = (wcstombs(NULL,wcs,0) + 1);
    //minSize = WideCharToMultiByte(CP_ACP,0,wcs,-1,NULL,0,NULL,FALSE);
    char *buf=(char*)malloc(minSize);
    //WideCharToMultiByte (CP_ACP, 0, wcs, -1, *mbs, minSize, NULL, 0);
    wcstombs(buf,wcs,minSize);
    *mbs = buf;
    return minSize;
}
void append(_string* self AFTER_SELF, wstring s)
{
	//wcscat()
	//int len_self=wcslen(self->_buffer);
	int s_size,newSize;
	wchar_t *newBuffer;
	s_size=((_string*)s)->_size;
	newSize=self->_size+s_size-sizeof(wchar_t);
	newBuffer=realloc(self->_buffer,newSize);
	memcpy(((char*)newBuffer)+self->_size-sizeof(wchar_t),((_string*)s)->_buffer,s_size);
	self->_buffer=newBuffer;
	self->_size=newSize;
	_M_checkUpStruct((_string *)s);
	return;
}

void insert(_string* self AFTER_SELF, wstring s)
{
	_M_checkUpStruct(self);
	return;
}

wchar_t at(_string* self AFTER_SELF, unsigned int index)
{
	wchar_t ret=index<(self->_size/sizeof(wchar_t)-1)
				? self->_buffer[index] : 0;
	_M_checkUpStruct(self);
	return ret;
}

int compare(_string* self AFTER_SELF, wstring s)
{
    _M_checkUpStruct(self);
    return 0;
}

int find(_string* self AFTER_SELF, wstring s)
{
	_M_checkUpStruct(self);
	return 0;
}

int length(_string* self AFTER_SELF)
{
	int len = self->_size / sizeof(wchar_t) - 1;
	_M_checkUpStruct(self);
	return len;
}

wbool startWith(_string *self AFTER_SELF, wstring s)
{
    //fprintf(stderr, "Function: startWith(%p, %p)\n", self, s);

	_string *str = (_string *)s;
	size_t len = str->_size/sizeof(wchar_t)-1;		// Cannot use length() here
	if(self->length() < len) return wfalse;
	wbool ret = wcsncmp(self->_buffer, str->_buffer, len) == 0;
	_M_checkUpStruct(self);
	return ret;
}

wbool endWith(_string *self AFTER_SELF, wstring s)
{
	_string *str = (_string *)s;
	size_t len1 = self->length();
	size_t len2 = str->_size/sizeof(wchar_t)-1;		// Cannot use length() here
	if(len1 < len2) return wfalse;
	size_t pos = len1 - len2;
	wbool ret = wcsncmp(self->_buffer + pos, str->_buffer, len2) == 0;
	_M_checkUpStruct(self);
	return ret;
}

wstring subString(_string* self AFTER_SELF, unsigned int begin, unsigned int end)
{
	//fprintf(stderr, "Function: subString(%p, %u, %u)\n", self, begin, end);

	int size;
	int _begin,_end;
	unsigned int maxIndex;
	if(begin>end)
		_begin=end,_end=begin;
	else
		_begin=begin,_end=end;

	maxIndex=(self->_size/sizeof(wchar_t)-2);
	if(_begin > maxIndex) return wnull;
	if(_end > maxIndex) _end=maxIndex;
	size = (_end-_begin+2)*sizeof(wchar_t);
	wchar_t*buffer = (wchar_t*)malloc(size);
	memcpy(buffer,self->_buffer+_begin,size-sizeof(wchar_t));
	buffer[_end-_begin+1]=0;

	//printf("size=%d begin=%d end=%d\n",size,_begin,_end);
	_string* inst=_M_constructBaseStruct();
	inst->_size=size;
	inst->_buffer=buffer;
	inst->_attrib=STRING_ATTRIBUTE_TEMP;
	_M_checkUpStruct(self);
	return (wstring)inst;
}

const char *toAnsi(_string* self)
{
	//freeTmpBuf();
	void *tmpBuffer;
	_M_wcharToMByte(self->_buffer,(char**)&tmpBuffer);

	setTmpBuffer(tmpBuffer);
	_M_checkUpStruct(self);

	return (const char*)tmpBuffer;
}

const wchar_t *toUtf16(_string* self)
{
	//fprintf(stderr, "Function: toUtf16(%p)\n", self);
	//MessageBoxW(0,self->_buffer,L"toUtf16",0);

	//freeTmpBuf();
    
	if(self->_attrib==STRING_ATTRIBUTE_TEMP)
	{
		wchar_t *tmpBuffer=self->_buffer;
		setTmpBuffer(tmpBuffer);
		self->_buffer=NULL;
		_M_deleteStruct(self);
		return tmpBuffer;
	}
	return self->_buffer;
}

/*
void partest(_string *self, int a, int b, int c, int d, int e, int f) {
	printf("partest(%p, %d, %d, %d, %d, %d, %d)\n", self, a, b, c, d, e, f);
	MessageBoxW(NULL, self->_buffer, L"partest", 0);
}*/

wstring WA(const char *s)
{
	int s_size;
	wchar_t*wchar_buff;

	s_size=_M_mbyteToWChar(s,&wchar_buff);
    //printf("WA:%ls\n",wchar_buff);
	_string* inst=_M_constructBaseStruct();
	inst->_size=s_size;
	inst->_buffer=wchar_buff;
	inst->_attrib=STRING_ATTRIBUTE_TEMP;

	return (wstring)inst;
}

wstring WW(const wchar_t *s)
{
	int s_size;
	s_size=_M_strSizeW(s);
	wchar_t*buffer=(wchar_t*)malloc(s_size);
	memcpy(buffer,s,s_size);
	_string* inst=_M_constructBaseStruct();
	inst->_size=s_size;
	inst->_buffer=buffer;
	inst->_attrib=STRING_ATTRIBUTE_TEMP;
	return (wstring)inst;
}

wstring new_wstring(wstring src)
{
	_string *ret;
	if(src)
	{
		if(STRING_ATTRIBUTE_TEMP==((_string*)src)->_attrib)
		{
			_M_moveStruct(&ret, (_string *)src);
			ret->_attrib=STRING_ATTRIBUTE_NORMAL;
		} else {
			_M_copyStruct(&ret, (_string *)src);
		}
	} else
	{
		ret=_M_constructBaseStruct();
		ret->_attrib=STRING_ATTRIBUTE_NORMAL;
		ret->_size = sizeof(wchar_t);
		ret->_buffer=malloc(sizeof(wchar_t));
		ret->_buffer[0]=0;
	}
	return (wstring)ret;
}

void delete_wstring(wstring *inst)
{
	_M_deleteStruct((_string *)inst);
}

void initwstring()
{
	initTmpBuf(9);
	wnull=new_wstring(WW(L""));
}
