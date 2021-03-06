/* XXX: Preprocessor instruction to enable basic macros; do not modify. */
#include <stddef.h>
#include <string.h>

/*
 * parse()
 *
 * - Description: creates the token and argv arrays from the buffer character
 *   array
 *
 * - Arguments: buffer: a char array representing user input, tokens: the tokenized
 *   input, argv: the argument array eventually used for execv()
 *
 * - Usage:
 *
 *      For the tokens array:
 *
 *      cd dir -> [cd, dir]
 *      [tab]mkdir[tab][space]name -> [mkdir, name]
 *      /bin/echo 'Hello world!' -> [/bin/echo, 'Hello, world!']
 *
 *      For the argv array:
 *
 *       char *argv[4];
 *       argv[0] = echo;
 *       argv[1] = 'Hello;
 *       argv[2] = world!';
 *       argv[3] = NULL;
 *
 * - Hint: for this part f the assignment, you are allowed to use the built
 *   in string functions listed in the handout
 */
void parse(char buffer[1024], char *tokens[512], char *argv[512]) {
  char *strpt, *tokenn, *arg, *art,*tokenn2;
  tokenn = strtok(buffer," ");
  int i=0;
  while(tokenn != NULL){
    while (*tokenn == '\t'){
      ++tokenn;
    }
    while (tokenn[strlen(tokenn)-1] == '\t'){
      tokenn[strlen(tokenn)-1]='\0';
    }
    tokens[i] = tokenn;
    i++;
    tokenn = strtok(NULL," ");
  }
  



  
   int j = 0;
   while (tokens[j]!=NULL){
       argv[j] = tokens[j];
       j++;
   }

  
   if (buffer[0]=='/'){
     //printf("buffer %s\n",buffer);
     arg = buffer;
     art = buffer;
     while (strcspn(arg,"/")==0){
       arg++;
     
       art = strchr(arg,'/');
       if (art == NULL){
	 break;
       }
       arg = strchr(arg,'/');
       
       
     }
     printf("trtr%s %s\n",buffer,arg);

     
     argv[0]=arg;
   }
  
}
