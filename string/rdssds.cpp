#include "rdssds.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

// sds API

// 
//  创建一个指定长度的 sds
// 
sds sdsnewlen(const void *init, size_t init_len){
	SDShdr *sh;
    size_t sdshdr_len = sizeof(SDShdr);
	if (init == NULL) {
		sh = (SDShdr *) malloc(sdshdr_len + init_len + 1);
	}
	else {
		sh = (SDShdr *) calloc(sdshdr_len + init_len + 1, sizeof (SDShdr));
	}
	if (sh == NULL) return NULL;
	sh->len = init_len;
	sh->free = 0;
	if(init_len && init != NULL) {
		memcpy(sh->buf, init, init_len);
	}
	sh->buf[init_len] = '\0';
	return (char *)sh->buf;
}

//
// 创建一个只包含空白字符串""的sds
//
sds sdsempty(){
	return sdsnewlen("", 0);
}

//
// 根据给定C字符串，创建一个相应的sds
//
sds sdsnew(const char * init){
    size_t initlen = (init == NULL) ? 0 : strlen(init); 
	return sdsnewlen(init, initlen);
}

//
// 复制给定sds
//
sds sdsdup(const sds s){
	return sdsnewlen(s, sdslen(s));
}

//
// 释放给定sds
//
void sdsfree(sds s){
	if(s == NULL) return ;
    free(s - sizeof(SDShdr));	
    return ;
}

//
// 更新给定 sds 所对应 sdrhdr 结构的free 和 len
//
void sdsupdatelen(sds s) {
    SDShdr *sh = (SDShdr *) (s - (sizeof(SDShdr)));
    int reallen = strlen(s);
    sh->free += (sh->len-reallen);
    sh->len = reallen;
}

//
// 将给定 sds 的 buf 扩展至指定长度，无内容的部分用 \0 来填充 
//
sds sdsgrowByZero(sds s, size_t len) {
    SDShdr *sh = (SDShdr *)(s-(sizeof(SDShdr)));
    size_t totlen, curlen = sh->len;

    if (len <= curlen) return s;
    s = sdsMakeRoomFor(s,len-curlen);  /* 按策略申请长度 */
    if (s == NULL) return NULL;

    /* Make sure added region doesn't contain garbage */
    sh = (SDShdr *)(s-(sizeof(SDShdr)));
    memset(s+curlen,0,(len-curlen+1)); /* also set trailing \0 byte */
    totlen = sh->len+sh->free;
    sh->len = len;
    sh->free = totlen-sh->len;
    return s;
}
//
// 追加一个C类型的字符串 带长度len
//

sds sdscatlen(sds s, const void *t, size_t len) {
    SDShdr *sh;
    size_t curlen = sdslen(s);

    s = sdsMakeRoomFor(s,len);
    if (s == NULL) return NULL;
    sh = (SDShdr *) (s-(sizeof(SDShdr)));
    memcpy(s+curlen, t, len);
    sh->len = curlen+len;
    sh->free = sh->free - len;
    s[curlen+len] = '\0';
    return s;
}

//
// 追加一个C类型的字符串
//
sds sdscat(sds s, const char *t) {
    return sdscatlen(s, t, strlen(t));
}

//
// 追加一个sds 字符串 
//
sds sdscatsds(sds s, const sds t) {
    return sdscatlen(s, t, sdslen(t));
}

//
// 拷贝一个C类型的字符串 带长度len
//
sds sdscpylen(sds s, const char *t, size_t len) {
    SDShdr *sh = (SDShdr *) (s-(sizeof(SDShdr)));
    size_t totlen = sh->free+sh->len;

    if (totlen < len) {
        s = sdsMakeRoomFor(s,len-sh->len);
        if (s == NULL) return NULL;
        sh = (SDShdr *) (s-(sizeof(SDShdr)));
        totlen = sh->free+sh->len;
    }
    memcpy(s, t, len);
    s[len] = '\0';
    sh->len = len;
    sh->free = totlen-len;
    return s;
}

//
// 拷贝一个C类型的字符串
//
sds sdscpy(sds s, const char *t) {
    return sdscpylen(s, t, strlen(t));
}

//
// 比较两个字符串 
// 1  if s1 > s2.
// -1 if s1 < s2.
// 0  if s1 and s2 are exactly the same binary string.
//
int sdscmp(const sds s1, const sds s2) {
    size_t l1, l2, minlen;
    int cmp;

    l1 = sdslen(s1);
    l2 = sdslen(s2);
    minlen = (l1 < l2) ? l1 : l2;
    cmp = memcmp(s1, s2, minlen);
    if (cmp == 0) return l1 - l2;
    return cmp;
}


//
//清除给定sds的内，将它初始化为""
// 
void sdsclear( sds s){
	SDShdr *sh = (SDShdr *) (s - (sizeof(SDShdr)));
	sh->free += sh->len;
	sh->len =0;
	sh->buf[0] = '\0';
}

//
// 打印出 sds 字符串
//
void  sdsprint(sds s){
	SDShdr *sh = (SDShdr *) (s - (sizeof(SDShdr)));
	printf("[sds string print] %s\n", sh->buf);
	return ;
}

//
// 取出子串 end为负时从后面往前算起
//
void sdsrange(sds s, int start, int end) {
    SDShdr  *sh = (SDShdr *) (s - (sizeof(SDShdr)));
    size_t newlen, len = sdslen(s);

    if (len == 0) return;
    if (start < 0) {
        start = len+start;
        if (start < 0) start = 0;
    }
    if (end < 0) {
        end = len+end;
        if (end < 0) end = 0;
    }
    newlen = (start > end) ? 0 : (end-start)+1;
    if (newlen != 0) {
        if (start >= (signed)len) {
            newlen = 0;
        } else if (end >= (signed)len) {
            end = len-1;
            newlen = (start > end) ? 0 : (end-start)+1;
        }
    } else {
        start = 0;
    }
    if (start && newlen) memmove(sh->buf, sh->buf+start, newlen);
    sh->buf[newlen] = 0;
    sh->free = sh->free+(sh->len-newlen);
    sh->len = newlen;
}

//
// 把s按sep分割, len是s的长度，seplen是sep的长度
// 
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count) {
    int elements = 0, slots = 5, start = 0, j;
    sds *tokens;

    if (seplen < 1 || len < 0) return NULL;

    tokens = (sds *) malloc(sizeof(sds) * slots);
    if (tokens == NULL) return NULL;

    if (len == 0) {
        *count = 0;
        return tokens;
    }
    for (j = 0; j < (len-(seplen-1)); j++) {
        /* make sure there is room for the next element and the final one */
        if (slots < elements + 2) {
            sds *newtokens;

            slots *= 2;
            newtokens = (sds *)realloc(tokens, sizeof(sds) * slots);
            if (newtokens == NULL) goto cleanup;
            tokens = newtokens;
        }
        /* search the separator */
        if ((seplen == 1 && *(s + j) == sep[0]) || (memcmp(s + j,sep, seplen) == 0)) {
            tokens[elements] = sdsnewlen(s + start,j - start);
            if (tokens[elements] == NULL) goto cleanup;
            elements++;
            start = j + seplen;
            j = j + seplen - 1; /* skip the separator */
        }
    }
    /* Add the final element. We are sure there is room in the tokens array. */
    tokens[elements] = sdsnewlen(s + start,len - start);
    if (tokens[elements] == NULL) goto cleanup;
    elements++;
    *count = elements;
    return tokens;

cleanup:
    {
        int i;
        for (i = 0; i < elements; i++) sdsfree(tokens[i]);
        free(tokens);
        *count = 0;
        return NULL;
    }
}

//
// 释放 sdssplitlen 的返回值，sdssplitlen专用啊,其实就是释放一个数组 
//
void sdsfreesplitres(sds *tokens, int count) {
    if (!tokens) return;
    while(count--)
        sdsfree(tokens[count]);
    free(tokens);
}

//
// 转为小写 
//
void sdstolower(sds s) {
    int len = sdslen(s), j;
    for (j = 0; j < len; j++) s[j] = tolower(s[j]);
}

//
// 转为大写
//
void sdstoupper(sds s) {
    int len = sdslen(s), j;
    for (j = 0; j < len; j++) s[j] = toupper(s[j]);
}

//
//  long long to sds
//
#define SDS_LLSTR_SIZE 21
sds sdsfromlonglong(long long value) {
    char buf[SDS_LLSTR_SIZE];
    int len = sdsll2str(buf,value);

    return sdsnewlen(buf,len);
}

// ---------------------------------------------------------------
//
//
sds sdsgrowzero(sds s, size_t len) {
	SDShdr *sh = (SDShdr *)(s-(sizeof(SDShdr)));
    size_t totlen, curlen = sh->len;

    if (len <= curlen) return s;
    s = sdsMakeRoomFor(s,len-curlen); /* 按策略申请长度 */
    if (s == NULL) return NULL;

    /* Make sure added region doesn't contain garbage */
    sh = (SDShdr *)(s-(sizeof(SDShdr)));
    memset(s+curlen,0,(len-curlen+1)); /* also set trailing \0 byte */
    totlen = sh->len+sh->free;
    sh->len = len;
    sh->free = totlen-sh->len;
    return s;
}

#define SDS_MAX_PREALLOC (1024*1024)
sds sdsMakeRoomFor(sds s, size_t addlen) {
    SDShdr *sh, *newsh;
    size_t free = sdsavail(s);
    size_t len, newlen;

    if (free >= addlen) return s;
    len = sdslen(s);
    sh = (SDShdr *) (s - (sizeof(SDShdr)));
    newlen = (len + addlen);
    if (newlen < SDS_MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += SDS_MAX_PREALLOC;
    newsh = (SDShdr *)realloc(sh, sizeof(SDShdr) + newlen + 1);
    if (newsh == NULL) return NULL;

    newsh->free = newlen - len;
    return newsh->buf;
}

int sdsll2str(char *s, long long value) {
    char *p, aux;
    unsigned long long v;
    size_t l;

    /* Generate the string representation, this method produces
     * an reversed string. */
    v = (value < 0) ? -value : value;
    p = s;
    do {
        *p++ = '0'+(v%10);
        v /= 10;
    } while(v);
    if (value < 0) *p++ = '-';

    /* Compute length and add null term. */
    l = p-s;
    *p = '\0';

    /* Reverse the string. */
    p--;
    while(s < p) {
        aux = *s;
        *s = *p;
        *p = aux;
        s++;
        p--;
    }
    return l;
}
