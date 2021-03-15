#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <setjmp.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdbool.h>
#include <signal.h>


static int *glob_var;
static char *str;
static int *partDeger=0;
static int *deger=0;
static jmp_buf s_jumpBuffer;
static int *errorControl=0;
static int *garbageCollector=0;
static char get[512];
static char file[512];
static FILE *fp;
static int *parentPid;
static int quitFlag=0;
static int fileFlag=0;
char* dizi;
char* pick;
char* str2;


char* findTheCommand(char* string,int final)
{ 
 
    char character;
    if(final != *deger)  
    {
    character = ';';
    }
    	
    else
    {
     character = '\0';
    }	
    	
    int index;
    char* ptr;
    ptr=strchr(string,character);
    index=(int)(ptr-string);

     pick=malloc(sizeof(char)+(index+1));
    strncpy(pick,string,index);
    *deger=*deger+1;
    return pick;	
}

char* findParameters(char* string,int final)
{
    char character = ';';
    int index;
    char* ptr;
    str2=malloc(strlen(string)+1);

    ptr=strchr(string,character);
    index=(int)(ptr-string);

    strcpy(str2,string+(index+1));
    *partDeger=*partDeger+1;
    return str2;

}

void removeChar(char *line,char** argv) {

	int index;
	int indexArgv=0;
              
	while(*line != '\0')
	{
		while(*line == ' ')
		{
		   *line++;
		}
		
		   if(*line != '\0')
		      {
		
		        char* dizi = malloc(sizeof(char)*512); 
		        index =0;
			
		            while (*line != ' '&& *line !='\0')
         		   {
         		     dizi[index]=*line;
         		     index++;
              		     *line++;     
          		   } 
          		
          		    while (*line != '\0' && *line != ' ') 
          	      	   {
          	     	      line++;
          	           }     
          		
      			 dizi[index]='\0';
      			 argv[indexArgv] = dizi;
      			 indexArgv++;
          	     }    
	}
	argv[indexArgv] = NULL;
}

void forkIslemi(int cocukSayisi)
{

	int pid;
	
	for(*glob_var=0;*glob_var<cocukSayisi;*glob_var=*glob_var+1)
	{  
		pid = fork();
		
		if(pid < 0)
		{       
		    exit(1);
		}
		else if(pid == 0)
		{	
			
		}
		else
		{
		    *parentPid = getpid();
		    wait(NULL);	
		}
	}
}

int calculateCharacter(char* line)
{
  int sayi;
  for (sayi=0; line[sayi]; line[sayi]==';' ? sayi++ : *line++);
  return sayi;
}

void controlBlank(char** argv)
{
  if(argv[0] == NULL)
   {
   
   char* line = "";
   char* dizi = malloc(sizeof(char)*1);
   dizi[0] = *line;
   argv[0] = dizi;  
   
   free(dizi);
   }
}

void setup()
{
    str = mmap(NULL,sizeof* str,PROT_READ | PROT_WRITE,  
                       MAP_SHARED | MAP_ANONYMOUS,-1,0);
    
    deger = mmap(NULL,sizeof* deger,PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1,0);
			
    partDeger = mmap(NULL,sizeof* partDeger,PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1,0);
			
    errorControl = mmap(NULL,sizeof* errorControl,PROT_READ | PROT_WRITE,
		        MAP_SHARED | MAP_ANONYMOUS, -1,0);
		  
		        
    parentPid = mmap(NULL,sizeof* parentPid,PROT_READ | PROT_WRITE,
		        MAP_SHARED | MAP_ANONYMOUS, -1,0);
		       
    glob_var = mmap(NULL,sizeof* glob_var,PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1,0);
			
    garbageCollector= mmap(NULL,sizeof* glob_var,PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, -1,0);
}
void execute(char line[512])
{
    
    char* argv[512];
    char* token;
    int sayi; 
    int control;
    
    sayi = calculateCharacter(line);
    
    line[strcspn(line, "\n")] = 0;
    strcpy(str, line);
    
    forkIslemi(sayi+1);
  
   
   if(*garbageCollector != sayi+1)
     {
     
      *garbageCollector = *garbageCollector+1;
      token =  findTheCommand(str,sayi); 
    
          if(sayi >= *deger){ strcpy(str,findParameters(str,sayi));}
   
          removeChar(token,argv);
     
             if(strcmp(argv[0],"quit"))
               {
                 control = setjmp(s_jumpBuffer);
                 
                if(control)	
                   {
                     execvp(argv[0],argv);
                         if(*errorControl == 0) 
                            {  
                         printf("syntax error near unexpected token %s \n",argv[0]);                 
                         *errorControl = *errorControl+1; 
                            }
                              exit(control);
                   }
                         else
                          {
                           longjmp(s_jumpBuffer, 42);
                          }
              }	
    
                  else { quitFlag=1;}
       }
     else 
     { 
        if(fileFlag==1) 
          {
            quitFlag=1;
          }
     }
     
}

void freeAreas()
{ 
   	
  munmap(str,sizeof* str); 
  munmap(deger,sizeof* deger);
  munmap(partDeger,sizeof* partDeger);
  munmap(errorControl,sizeof* errorControl);
  munmap(glob_var,sizeof* glob_var);
  free(dizi);
  free(pick);
  free(str2);

}
bool fileControl(char line[512])
{
	if(strstr(line,".txt")==NULL)
	{
	  return false;
	}
	else
	{
	  return true;
	}
}
	
void checkFileOrNot(int argcm, char** argvm)
{
	if(argcm > 1)  
	{ 
		
	   fileFlag=1;
	   char buf[512];
	   fp = fopen(argvm[1],"r");  
	
	    if(fp == NULL)
  	      {
    	         perror("Error opening file"); 
    	         strcpy(get,"\n");
    	      }
    	 
    	       else
    	       { 
    	            int sayac=0; 
    	            while(fgets(buf, sizeof(buf), fp))    
                      {
                   int i=0;
       
                      while(buf[i] != '\n') 
                        {
                          i++;
                          sayac++;
                        }
              
                     if(sayac == 511)
    	                 {
    	                    printf("Value bigger than 512 character.\n");
    	                 }
    	         
                        buf[i]=';';
    	                strcat(get,buf);
    	     
	              }
  	       }
	 }
	
	else 
	  {
	
	 char* say = fgets(get,sizeof(get),stdin);
	 get[strcspn(get, "\n")] = 0;
	 if(strlen(say) == 511) 
	 {
	    printf("Value bigger than 512 character.\n");
	 }
	
	 }
   
	
}
int main(int argcm, char** argvm)
{
	 
  do
  {
    printf("o--> ");
    checkFileOrNot(argcm, argvm);
      
    if(get != NULL)
     {
       if(strcmp(get,"quit"))
          {
      	    setup();
      	     if(strcmp(get,""))
      	      {
      		execute(get);
	      }
	        freeAreas();
          }
     }
  }
  while(strcmp(get,"quit")&&!feof(stdin)&& quitFlag!=1&&get!=NULL);
  

    if(fp!=NULL)
    {fclose(fp);} 
    kill(0, SIGINT);
       
    return 0;
}

