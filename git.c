#define _XOPEN_SOURCE 700
#include <git2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>
static int rmrf_cb(const char*p,const struct stat*sb,int t,struct FTW*f){
(void)sb;(void)f;return remove(p);}
/* 递归删除 libgit2 为不存在的包建出的空目录 */
static void rmrf(const char*p){
if(nftw(p,rmrf_cb,16,FTW_DEPTH|FTW_PHYS))remove(p);}
char*mirror(const char*u){
if(!u)return 0;
/* 只在 github.com 走代理镜像，其他域名（如 aur.archlinux.org）原样返回 */
const char*g="github.com";
if(!strstr(u,g))return strdup(u);
const char*m="ghproxy.net/https://github.com";
const char*p=strstr(u,g);
size_t n=strlen(u)+strlen(m)-strlen(g)+1;
char*o=malloc(n);
if(!o)return 0;
size_t h=(size_t)(p-u);
memcpy(o,u,h);
strcpy(o+h,m);
strcat(o,p+strlen(g));
return o;}
int cay_git_clone(const char*pkg){
char url[256];
snprintf(url,sizeof url,"https://aur.archlinux.org/%s.git",pkg);
char*mu=mirror(url);
if(!mu)return 1;
    git_libgit2_init();
    git_repository*r=0;
    int e=git_clone(&r,mu,pkg,0);
    if(r)git_repository_free(r);
    free(mu);
    if(e){
        const git_error*ge=git_error_last();
        fprintf(stderr,"cay: clone %s failed: %s\n",pkg,ge?ge->message:"unknown");
        git_libgit2_shutdown();
        return 1;}
    git_libgit2_shutdown();
    /* 远端不存在时 libgit2 可能建出空仓库且不报错，检查 PKGBUILD 兜底 */
    char pb[300];
    snprintf(pb,sizeof pb,"%s/PKGBUILD",pkg);
    FILE*f=fopen(pb,"r");
    if(!f){fprintf(stderr,"cay: %s not found in AUR\n",pkg);rmrf(pkg);return 1;}
    fclose(f);
    return 0;}
