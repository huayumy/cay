#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
int aur_search(const char*);
int aur_info(const char*);
int cay_git_clone(const char*);
int run_makepkg(const char*);
/* 目录已存在且是 git 仓库 */
static int is_repo(const char*p){
char pb[300];snprintf(pb,sizeof pb,"%s/.git",p);
struct stat st;return stat(pb,&st)==0&&S_ISDIR(st.st_mode);}
/* 目录已存在 */
static int is_dir(const char*p){
struct stat st;return stat(p,&st)==0&&S_ISDIR(st.st_mode);}
static void usage(void){
fprintf(stderr,"usage: cay -S <pkg>...   install\n"
               "       cay -Ss <kw>      search\n"
               "       cay -Si <pkg>     info\n");}
int main(int c,char**v){
if(getuid()==0){fprintf(stderr,"cay: no root\n");return 1;}
if(c<2){usage();return 1;}

/* 单词别名 */
if(!strcmp(v[1],"search")){if(c<3){fprintf(stderr,"usage: cay -Ss <kw>\n");return 1;}return aur_search(v[2]);}
if(!strcmp(v[1],"info")){if(c<3){fprintf(stderr,"usage: cay -Si <pkg>\n");return 1;}return aur_info(v[2]);}

/* 解析短选项：op = 首字母，mods = 其余字母（yay 风格，如 -Syu） */
char op=0;char mods[32]={0};int start=1;
if(!strcmp(v[1],"install")){op='S';}
else if(!strcmp(v[1],"--")){op='S';start=2;}/* -- 之后全是包名 */
else if(v[1][0]=='-'&&v[1][1]=='-'){/* 长选项 */
if(!strcmp(v[1],"--help")){usage();return 0;}
else if(!strcmp(v[1],"--noconfirm")){op='S';start=2;}/* 透传 makepkg，本轮仅接受 */
else{fprintf(stderr,"cay: unknown option %s\n",v[1]);usage();return 1;}}
else if(v[1][0]=='-'){
if(!v[1][1]){fprintf(stderr,"cay: unknown option -\n");usage();return 1;}/* 裸 - */
op=v[1][1];
strncpy(mods,v[1]+2,sizeof mods-1);start=2;}
else op='S';/* 裸包名，默认安装 */

/* 修饰符白名单：只认 s i y u */
if(op=='S')
for(char*p=mods;*p;p++)
if(*p!='s'&&*p!='i'&&*p!='y'&&*p!='u'){
fprintf(stderr,"cay: unknown modifier -%c\n",*p);usage();return 1;}

if(op=='S'&&strchr(mods,'s')){/* -Ss 搜索 */
if(c<=start){fprintf(stderr,"usage: cay -Ss <kw>\n");return 1;}
return aur_search(v[start]);}
if(op=='S'&&strchr(mods,'i')){/* -Si 信息 */
if(c<=start){fprintf(stderr,"usage: cay -Si <pkg>\n");return 1;}
return aur_info(v[start]);}
if(op=='R'){fprintf(stderr,"cay: uninstall (-R) not implemented yet\n");return 1;}
if(op!='S'){fprintf(stderr,"cay: unknown operation -%c\n",op);usage();return 1;}

/* -S 安装（y/u 修饰本轮为 no-op，仅解析接受） */
if(c<=start){fprintf(stderr,"usage: cay -S <pkg>...\n");return 1;}
int fail=0;
for(int i=start;i<c;i++){
if(is_repo(v[i]))fprintf(stderr,"cay: %s exists, skipping clone\n",v[i]);
else if(is_dir(v[i])){fprintf(stderr,"cay: %s exists and is not a git repo, refusing\n",v[i]);fail=1;continue;}
else if(cay_git_clone(v[i])){fail=1;continue;}
if(run_makepkg(v[i]))fail=1;}
return fail?1:0;}
