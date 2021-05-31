#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define mj 80 //mj - max number characters in string;
#define mi 16 //mi - max number strings;

void child(char *command[], char *input, char *output, int inpipe, int outpipe, int cl);

int main()
{
int stop = 0;
while(!stop)
{
char ch[mi][mj];
char *args[mi];

int cc = 1;

int c, i = 0, j = 0;
int new = 1;

printf("$ ");
while ((c = getchar()) != EOF)
{
if (c != '\n')
{
if (c == ' ' || i > (mj-1))
{
if (new) continue;

ch[j][i] = '\0';
args[j] = ch[j];
j++;
i = 0;
new = 1;
continue;
}
if (c == '<' || c == '>' || c == '|')
{
if (!new)
{
ch[j][i] = '\0';
args[j] = ch[j];
j++;
i = 0;
}

if (c == '|') cc++;

ch[j][i] = c;
ch[j][i+1] = '\0';
args[j] = ch[j];
j++;

new = 1;
continue;
}

ch[j][i++] = c;
new = 0;
}
else
{
ch[j][i] = '\0';
args[j] = ch[j];
j++;
args[j] = NULL;
break;
}
}

if (c == EOF)
{
printf("THE END");
stop = 1;
return EXIT_SUCCESS;
}

char *commands[cc][mi];
char *i_f[c];
char *o_f[cc];

for (int i = 0; i < cc; i++)
{
i_f[i] = NULL;
o_f[i] = NULL;
}

int k = 0;
int l = 0;

for (int i = 0; i < j; i++)
{
if (args[i][0] == '<')
{
i_f[l] = args[++i];
continue;
}

if (args[i][0] == '>')
{
o_f[l] = args[++i];
continue;
}

if (args[i][0] == '|')
{
commands[l][k] = NULL;
l++;
k = 0;
continue;
}

commands[l][k] = args[i];
k++;
}
commands[l][k] = NULL;

int pp[cc-1][2];

for (int i = 0; i < cc; i++)
{
if (pipe(pp[i]) == -1)
{
perror("PIPE ERROR");
exit(EXIT_FAILURE);
}
}

for (int i = 0; i < cc; i++)
{
int in = -1;
int cl = -1;
if (i != 0)
{
in = pp[i-1][0];
cl = pp[i-1][1];
}

int out = i == cc - 1 ? -1 : pp[i][1];

child(commands[i], i_f[i], o_f[i], in, out, cl);
}

for (int i = 0; i < cc; i++)
{
pid_t w = wait(NULL);
if (w == -1)
{
perror("WAIT ERROR");
return EXIT_FAILURE;
}

close(pp[i][0]);
close(pp[i][1]);
}
}

return EXIT_SUCCESS;
}
void child(char *command[], char *input, char *output, int inpipe, int outpipe, int cl)
{
pid_t pid = fork();
if (pid == -1)
{
perror("FORK ERROR");
exit(EXIT_FAILURE);
}
if (!pid)
{
if (cl != -1) close(cl);

if (inpipe != -1)
{
if(dup2(inpipe, STDIN_FILENO) == -1)
{
perror("DUP2 ERROR [INPIPE]");
exit(EXIT_FAILURE);
}
}

if (outpipe != -1)
{
if(dup2(outpipe, STDOUT_FILENO) == -1)
{
perror("DUP2 ERROR [OUTPIPE]");
exit(EXIT_FAILURE);
}
}

if (input != NULL)
{
int inf = open(input, O_RDONLY);
if (inf == -1)
{
perror("OPEN ERROR [INPUT]");
exit(EXIT_FAILURE);
}

if(dup2(inf, STDIN_FILENO) == -1)
{
perror("DUP2 ERROR [INPUT]");
exit(EXIT_FAILURE);
}
}

if (output != NULL)
{
int outf = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
if (outf == -1)
{
perror("OPEN ERROR [OUTPUT]");
exit(EXIT_FAILURE);
}

if(dup2(outf, STDOUT_FILENO) == -1)
{
perror("DUP2 ERROR [OUTPUT]");
exit(EXIT_FAILURE);
}
}

int evp = execvp(command[0], command);
if (evp == -1)
{
perror("EXECVP ERROR");
exit(EXIT_FAILURE);
}

exit(EXIT_SUCCESS);
}
}
