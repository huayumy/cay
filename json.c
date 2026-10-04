#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char*js_str(const char*p,const char*k){
char pat[64];sprintf(pat,"\"%s\":\"",k);
const char*s=strstr(p,pat);if(!s)return 0;
s+=strlen(pat);const char*e=s;
while(*e&&*e!='"')e++;
size_t n=e-s;char*o=malloc(n+1);if(!o)return 0;
memcpy(o,s,n);o[n]=0;return o;}
double js_num(const char*p,const char*k){
char pat[64];sprintf(pat,"\"%s\":",k);
const char*s=strstr(p,pat);if(!s)return 0;
return strtod(s+strlen(pat),0);}
