#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
  
  
int main()
{   
    char argv[16][80];
     char *program[16];
     int i=0;
     int j=0,k,err_exec,ch,inp,out,inp_flag=0,out_flag=0;;
    
    
    while((ch=getchar()) != EOF)
    {
        if (ch=='\n')
                    {
                        argv[i][j]='\0';
            k=i;
            for (i=0;i<=k;++i) 
            {  
                program[i]=argv[i];
            }
            program[i]=NULL;
            pid_t pid=fork();         
            if (pid==-1)
            {
              perror("fork error");
            }
            else 
            {
                if (!pid) 
                {
                    if (out_flag==1) 
                    {     
                        int fdout=open(program[out],O_WRONLY|O_CREAT|O_TRUNC,0664);  
                        if (fdout==-1)
                         {
                            perror ("open out error");
                            exit(EXIT_FAILURE);
                        }
                        if (-1==dup2(fdout, STDOUT_FILENO))
                         {     
                            perror("dup out error");
                            exit(EXIT_FAILURE);
                        }
                        program[out]=NULL;
                    }
                    if (inp_flag==1)
                    {      
                        int fdin=open(program[inp],O_RDONLY);  
                        if (fdin==-1) 
                        {
                            perror ("open inp error");
                            exit(EXIT_FAILURE);
                        }                        
                        if (-1==dup2(fdin, STDIN_FILENO)) 
                        { 
                            perror ("dup inp error");
                            exit(EXIT_FAILURE);
                        }
                        program[inp]=NULL;
                    }                     
                    err_exec=execvp(program[0],program);  
                    if (err_exec==-1) 
                    {
                        perror("execvp error");
                        exit(EXIT_FAILURE);
                    }
                    exit(EXIT_SUCCESS);
                }
            }
            pid=wait(NULL);           
            if (pid==-1) 
            {
                perror("wait error");
            }
            printf("///");
            i=0; 
            j=0;
            inp_flag=0;out_flag=0;
            continue;              
                    }
                    
                    
                    
                    
        if (ch==' ') 
         {
          argv[i][j]='\0';
          ++i;
          j=0;
          continue;
         }
         
         if (ch=='<') 
        {                  
            inp=i;
            if ((ch=getchar()) != ' ') 
            { 
                argv[i][j]=' ';           
                ++j;
            }
            ++inp_flag;                  
            continue;
        }

        if (ch=='>') 
        {                
            if (i==inp) 
            {               
                argv[i][j]='\0';
                ++i; j=0;
            }
            out=i;
            if ((ch=getchar()) !=' ') 
            {
               argv[i][j]=' ';
                ++j;
            }
            ++out_flag;                  
            continue;
        }
        

         
        argv[i][j]=ch;
        ++j;
        
        
        
        }
        
    
    return 0;
}
