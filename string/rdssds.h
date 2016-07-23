#ifndef RDSSDS_H_
#define RDSSDS_H_

#include <sys/types.h>
#include <stdarg.h>

// redis string

typedef char * sds;

typedef struct tagsdshdr{
	
	int len;     // buf 已占用长度
	int free;    // buf 剩余可用长度
	char buf []; // 实际保存字符串数据的地方
} SDShdr;

// sds的长度
static inline size_t sdslen(const sds s){
	SDShdr *sh = (SDShdr*)(s - (sizeof(SDShdr)));
	return sh->len;
}

// sds 空闲	长度
static inline size_t sdsavail(const sds s) {
    SDShdr *sh = (SDShdr*)(s-(sizeof(SDShdr)));
    return sh->free;
}

sds sdsnewlen(const void *init, size_t initlen);  /* 创建新字符串，内部申请了内存 */
sds sdsnew(const char *init);    /* 对sdsnewlen的封装而已 */
sds sdsempty(void);              /*创建一个空的字符串 调用sdsnewlen */
size_t sdslen(const sds s); 
sds sdsdup(const sds s);   /* 拷贝 */ 
void sdsfree(sds s);       /* 释放 */
size_t sdsavail(const sds s);
sds sdsgrowzero(sds s, size_t len); /* 将给定 sds 的 buf 扩展至指定长度，无内容的部分用 \0 来填充 */
sds sdscatlen(sds s, const void *t, size_t len);  /* 追加一个C类型的字符串 带长度len */
sds sdscat(sds s, const char *t);                 /* 调用sdscatlen， 在内部算长度 */
sds sdscatsds(sds s, const sds t);                /* 追加一个sds 字符串 */
sds sdscpylen(sds s, const char *t, size_t len);  /* 拷贝一个C类型的字符串 带长度len */
sds sdscpy(sds s, const char *t);                 /* 调用sdscpylen， 在内部算长度 */
void sdsprint(sds s);

//sds sdstrim(sds s, const char *cset);
void sdsrange(sds s, int start, int end);  /* 取出子串 end为负时从后面往前算起 */
void sdsupdatelen(sds s);                  /* 当手动强制把字符串砍掉时， 要用sdsupd telen更新len和free */
void sdsclear(sds s);                      /* 清除掉当前的字符串  */
int sdscmp(const sds s1, const sds s2);    /* 比较两个字符串 */

/* 把s按sep分割, len是s的长度，seplen是sep的长度  */
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void sdsfreesplitres(sds *tokens, int count); /* 释放 sdssplitlen 的返回值，sdssplitlen专用啊,其实就是释放一个数组  */
void sdstolower(sds s);  /* 转为小写 */
void sdstoupper(sds s);  /* 转为大写 */
sds sdsfromlonglong(long long value);   /*long long 转为字符串 */

/* Low level functions exposed to the user API */
sds sdsMakeRoomFor(sds s, size_t addlen);   /* 按策略申请长度 */
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSize(sds s);
int sdsll2str(char *s, long long value);  

#endif
