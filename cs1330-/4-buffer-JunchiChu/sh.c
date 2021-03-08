#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE  1024
void a(int c);
int main() {
  int i =10;
  i = 10;
  
  //int ic=0;
  while(i!=0){
    //printf("%d\n",i);
    i--;
    int status;
    pid_t child;
    char input[MAXSIZE];    
    ssize_t checkread;
    //char *tokens[512];
    //char *argv[512];
    char *tokenn;
    char *arg;
    char *art;
    
    checkread = read(STDIN_FILENO, input, MAXSIZE);

    if (checkread != 0){
     
      input[checkread]='\0';
     
      char *tokens[512];
      char *argv[512];
      int clean=0;
      while(clean!=512){
	tokens[clean]=NULL;
	argv[clean]=NULL;
	clean++;
      }
      char cinput[MAXSIZE];
      strcpy(cinput,input);
      tokenn = strtok(input," ");
      int i =0;
      while(tokenn !=NULL){
	
	while((int)*tokenn<33){
	  ++tokenn;
	}
	while(tokenn[strlen(tokenn)-1] == '\t'){
	  tokenn[strlen(tokenn)-1]='\0';
	}
	tokens[i] = tokenn;
       
	i++;
	tokenn = strtok(NULL," ");
      }
      
       
      int j =0;
      while (tokens[j]!=NULL){
	argv[j] = tokens[j];
	//printf("ins %s\n",argv[0]);
	j++;
      }
      j--;
      int ind = 0;
      while (ind != 15){
	if((int)argv[j][ind]<33){
          argv[j][ind]='\0';
	}
	ind++;
      }
      //argv[j][1]='\0';
      
      char *ret;
      

      ret = strstr(cinput,"/");
      if(ret !=NULL){
	//fprintf(stderr,"india \n");
      }else{

	//fprintf(stderr,"china \n");
      }
      if(ret!=NULL && *ret=='/'){
	//fprintf(stderr,"ni ma");
	arg= ret;
	art= ret;
	while(strcspn(arg,"/")==0){
	  arg++;
	  art = strchr(arg,'/');
	  if (art == NULL){
	    break;
	  }
	  arg = strchr(arg,'/');
	}
	argv[0]=arg;
	argv[0] = strtok(argv[0]," ");
	//printf("howowwwwwwn1 %s\n",argv[0]);

      }

      // int exitcode = strcmp(tokens[0],"exit");
      if (strstr(tokens[0],"exit")!=NULL){
	
	exit(0);
      }else{
      }
      	
      int count = 0;
      int fla = 0;
      while(tokens[0][count]!='\0'){
	if((int)tokens[0][count]==10){
	  fla = 1;
	}
	//printf("%d %d\n",count,tokens[0][count]);
      	count++;
      }
      if (fla ==1){
	count--;
      }
      //ic++;
      //printf("ic %d\n",ic);
      tokens[0][count]='\0';
      //printf("argv %s",argv[0]);
      //printf("token %s",tokens[0]);
	//fprintf(stderr,"mark 1\n");
	//fprintf(stderr,"child argv %s",argv[0]);
	//fprintf(stderr,"mark 2\n");
	//fprintf(stderr,"child token %s\n",tokens[0]);
	//fprintf(stderr,"mark 3\n");
	//if(ic==2){
	//tokens[0][7]='\0';
	//argv[1][2]='\0';
	//int sj = 0;
	//int ij =0;
	//	while(sj!=10){
	  //  while(ij!=10){
	    // if((int)argv[sj][ij]<33){
	      //argv[sj][ij]='\0';
	      // }
	    // }
	    
	  //	}
        
	fprintf(stderr,"tokens argv %s\n",tokens[0]);
	fprintf(stderr,"tokens argv %s\n",tokens[1]);
	fprintf(stderr,"tokens argv %s\n",tokens[2]);
	fprintf(stderr,"tokens argv %s\n",tokens[3]);

	fprintf(stderr,"child argv %s\n",argv[0]);
	fprintf(stderr,"child argv %s\n",argv[1]);
	fprintf(stderr,"child argv %s\n",argv[2]);
	fprintf(stderr,"child argv %s\n",argv[3]);
	fprintf(stderr,"child argv %s\n",argv[4]);
	
	//}
        //fprintf(stderr,"child token %s\n",tokens[0]);
	// char *argv[] = {"echo","hi", NULL};



       
      
      if((child = fork())==0){
      execv(tokens[0], argv);
      perror("execv");
      exit(1);
      }
     waitpid(child,&status,0);
       }else{
	 return 0;
    }
  }
    return 0;
}
