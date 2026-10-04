#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
struct buf{char*d;size_t n;};
static size_t wcb(void*p,size_t s,size_t m,void*u){
struct buf*b=u;size_t t=s*m;
char*x=realloc(b->d,b->n+t+1);
if(!x)return 0;
b->d=x;memcpy(b->d+b->n,p,t);
b->n+=t;b->d[b->n]=0;return t;}
char*http_get(const char*u){
CURL*c=curl_easy_init();if(!c)return 0;
struct buf b={0};
b.d=malloc(1);if(!b.d){curl_easy_cleanup(c);return 0;}
b.d[0]=0;
curl_easy_setopt(c,CURLOPT_URL,u);
curl_easy_setopt(c,CURLOPT_WRITEFUNCTION,wcb);
curl_easy_setopt(c,CURLOPT_WRITEDATA,&b);
curl_easy_setopt(c,CURLOPT_FOLLOWLOCATION,1L);
curl_easy_setopt(c,CURLOPT_TIMEOUT,30L);
CURLcode r=curl_easy_perform(c);
curl_easy_cleanup(c);
if(r){free(b.d);return 0;}
return b.d;}
