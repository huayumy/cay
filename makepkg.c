#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
extern char**environ;
int run_makepkg(const char*dir){
int fd[2];if(pipe(fd))return 1;
pid_t p=fork();
if(p<0){close(fd[0]);close(fd[1]);return 1;}
if(p==0){
dup2(fd[1],1);dup2(fd[1],2);
close(fd[0]);close(fd[1]);
chdir(dir);
char*av[]={"makepkg","-si","--noconfirm",0};
execve("/usr/bin/makepkg",av,environ);
_exit(127);}
close(fd[1]);
char b[256];ssize_t n;
while((n=read(fd[0],b,sizeof b))>0)fwrite(b,1,n,stdout);
close(fd[0]);
int st;waitpid(p,&st,0);
return WIFEXITED(st)?WEXITSTATUS(st):1;}
