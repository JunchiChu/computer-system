#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXSIZE 1024
void a(int c);
int main() {
    int i = 10;
    i = 10;

    // int ic=0;
    while (i != 0) {
        // printf("%d\n",i);
        i--;
        int status;
        pid_t child;
        char input[MAXSIZE];
        ssize_t checkread;
        // char *tokens[512];
        // char *argv[512];
        char *tokenn;
        char *arg;
        char *art;

        checkread = read(STDIN_FILENO, input, MAXSIZE);

        if (checkread != 0) {
            input[checkread] = '\0';
	    int sat = 0;
            int set = 0;
	    while(sat!=checkread){
	      if((int)input[sat]>33){
		set++;
	      }
	      sat++;
	    }
	    if(set!=0){
            if(checkread !=1){
            char *tokens[512];
            char *argv[512];
            int clean = 0;
            while (clean != 512) {
                tokens[clean] = NULL;
                argv[clean] = NULL;
                clean++;
            }
            char cinput[MAXSIZE];
	    int idxs = 0;
	  
	    while(idxs!=512){
	  
	      if(input[idxs]=='\t'){
                input[idxs]=' ';
		
	      }
	      idxs++;
	    }
	    
            strcpy(cinput, input);
            tokenn = strtok(input, " ");
	    if(tokenn !=NULL) {
            int i = 0;
            while (tokenn != NULL) {
	      while ((int)*tokenn < 33) {
		   ++tokenn;
		     }
		 while (tokenn[strlen(tokenn) - 1] == '\t') {
		   tokenn[strlen(tokenn) - 1] = '\0';
		      }
                tokens[i] = tokenn;
		//fprintf(stderr,"sad %s\nkjl",tokenn);
                i++;
                tokenn = strtok(NULL, " ");
            }

            int j = 0;
            while (tokens[j] != NULL) {
                argv[j] = tokens[j];
                // printf("ins %s\n",argv[0]);
                j++;
            }
            j--;
	      int ind = 0;
            while (ind != 15) {
	       if ((int)argv[j][ind] < 33) {
		   argv[j][ind] = '\0';
		      }
		 ind++;
		   }
             argv[j][1]='\0';

            char *ret;

            ret = strstr(cinput, "/");
            if (ret != NULL) {
                // fprintf(stderr,"ia \n");
            } else {
                // fprintf(stderr,"ina \n");
            }
            if (ret != NULL && *ret == '/') {
                // fprintf(stderr,"ni ma");
                arg = ret;
                art = ret;
                while (strcspn(arg, "/") == 0) {
                    arg++;
                    art = strchr(arg, '/');
                    if (art == NULL) {
                        break;
                    }
                    arg = strchr(arg, '/');
                }
                argv[0] = arg;
                argv[0] = strtok(argv[0], " ");
                // printf("howowwwwwwn1 %s\n",argv[0]);
            }

            // int exitcode = strcmp(tokens[0],"exit");
            if (strstr(tokens[0], "exit") != NULL) {
                exit(0);
            } else {
                // printf("exit code tokens %d  %s",exitcode,tokens[0]);
                // printf("injecttion1 %s\n",tokens[0]);
                // printf("injecttion1 %d\n",tokens[0][0]);
                // printf("injecttion1 %d\n",tokens[0][1]);
                // printf("injecttion1 %d\n",tokens[0][2]);
                // printf("injecttion1 %d\n",tokens[0][3]);
                // printf("injecttion1 %d\n",tokens[0][4]);
                // printf("injecttion1 %d\n",tokens[0][5]);
            }
            // char k;
            // int asci = (int) k;

            // printf("injecttion1 %s\n",tokens[0]);
            int count = 0;
            int fla = 0;
            while (tokens[0][count] != '\0') {
                if ((int)tokens[0][count] == 10) {
                    fla = 1;
                }
                // printf("%d %d\n",count,tokens[0][count]);
                count++;
            }
            if (fla == 1) {
                count--;
            }
            // ic++;
            // printf("ic %d\n",ic);
            tokens[0][count] = '\0';
	    /* 
            fprintf(stderr, "tokens argv %s\n", tokens[0]);
            fprintf(stderr, "tokens argv %s\n", tokens[1]);
            fprintf(stderr, "tokens argv %s\n", tokens[2]);
            fprintf(stderr, "tokens argv %s\n", tokens[3]);

            fprintf(stderr, "child argv %s\n", argv[0]);
            fprintf(stderr, "child argv %s\n", argv[1]);
            fprintf(stderr, "child argv %s\n", argv[2]);
            fprintf(stderr, "child argv %s\n", argv[3]);
            fprintf(stderr, "child argv %s\n", argv[4]);
            */

            // char *argv[] = {"echo","hi", NULL};
            int build_in = 0;
            if (tokens[0][0] != '/') {
                if (strcmp(tokens[0], "cd") == 0) {
                    
		  //tokens[1][19]='\0';
		    chdir(tokens[1]);
		    //printf("infact %s\n",tokens[1]);
                    build_in = 5;
                } else if (strcmp(tokens[0], "rm") == 0) {
                    build_in = 5;
                    unlink(tokens[1]);
                } else if (strcmp(tokens[0], "ln") == 0) {
                    build_in = 5;
                    link(tokens[1], tokens[2]);
                } else {
                    build_in = 0;
                }
            }
            if (build_in == 0) {
                if ((child = fork()) == 0) {
                    execv(tokens[0], argv);
                    perror("execv");
                    exit(1);
                }
            }
            waitpid(child, &status, 0);
	    }
	    }
	    }
        } else {
            return 0;
        }
    }
    return 0;
}
