#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
char*http_get(const char*);
char*js_str(const char*,const char*);
double js_num(const char*,const char*);
static char*mkurl(const char*t,const char*a){
CURL*c=curl_easy_init();if(!c)return 0;
char*e=curl_easy_escape(c,a,0);if(!e){curl_easy_cleanup(c);return 0;}
char*u=malloc(strlen(e)+80);if(u)
sprintf(u,"https://aur.archlinux.org/rpc/?v=5&type=%s&arg=%s",t,e);
curl_free(e);curl_easy_cleanup(c);return u;}
int aur_search(const char*k){
char*u=mkurl("search",k);if(!u)return 1;
char*r=http_get(u);free(u);if(!r)return 1;
double c=js_num(r,"resultcount");
printf("total: %.0f\n",c);
const char*p=r;
while((p=strstr(p,"\"Name\":\""))){
char*n=js_str(p,"Name");
char*v=js_str(p,"Version");
char*d=js_str(p,"Description");
double vt=js_num(p,"NumVotes");
printf("aur/%s %s (votes %.0f)\n  %s\n",
n?n:"?",v?v:"?",vt,d?d:"");
free(n);free(v);free(d);
p=strchr(p,'}');if(!p)break;p++;}
free(r);return 0;}
int aur_info(const char*pkg){
char*u=mkurl("info",pkg);if(!u)return 1;
char*r=http_get(u);free(u);if(!r)return 1;
if(js_num(r,"resultcount")<=0){
printf("not found: %s\n",pkg);free(r);return 1;}
char*n=js_str(r,"Name");
char*v=js_str(r,"Version");
char*d=js_str(r,"Description");
printf("Name    : %s\nVersion : %s\nDesc    : %s\n",
n?n:"?",v?v:"?",d?d:"?");
free(n);free(v);free(d);free(r);return 0;}
