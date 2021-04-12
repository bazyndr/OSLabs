#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int main()
{
char arg_inp[16][80];
char *a[16];
int i=0;
int j=0,k,error_ex,ch;

printf("/");

while((ch=getchar()) != EOF)
{
if (ch=='\n')
{
arg_inp[i][j]='\0';
k=i;
for(i=0;i<=k;i++)
{
a[i]=arg_inp[i];
}
a[i]=NULL;

pid_t pid = fork();  // пустой процесс
if (pid==-1)
{
perror("fork");  
}
else
if (pid == 0) // if !pid 
{
error_ex=execvp(a[0],a); // замещает свободный процесс в fork на тот котрый мы ввели с консоли
if (error_ex==-1)
{
perror("execvp");
}
}
pid=wait(NULL);  // posix вызов ожидания
if (pid==-1)
{
perror("wait");
}
printf("/");
i=0; j=0;
continue;
}
if (ch==' ') {
arg_inp[i][j]='\0';
++i;
j=0;
continue;
}
arg_inp[i][j]=ch;
++j;
}


return 0;
}
