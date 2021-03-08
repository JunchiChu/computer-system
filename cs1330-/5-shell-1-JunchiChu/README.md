# shell 1 🐚

Handout is [here](https://docs.google.com/document/d/1syEp4jNJVALczciKAznSC8rVCTN1vfr3dYleBxGy5KY/edit?usp=sharing). Remember that the `man` pages will be your best friend during this project. Good luck!

### Structure of my shell
`TODO: fill out`
First I null terminate the input string by using the result of read function.
I use strtok to split input to tokens, argvs. I changed argv[0] to make sure get rid of '/'
I check if the first token is "exit", and should response properly.
I also handle edge cases if there exist mulitple input redirect, which is not valid. If the first input is redirect, we should shift our argv array by 2 positions.
Then we set build in flag, if we detect "cd" "rm" "ln" in the tokens, we will call the build in function calls respectively.
If there is no build in commands, we will fork it and process the rest of command. We use a whilp loop to check if redirection exists, and if there exists such redirections, we will open file, call the right mode argument in each open. After that, use another whilp to shift our argv by 2 positions per each redirection exists. 
And we also call waitpid to wait child process. 
### How to compile 
`TODO: fill out`
Read my makefile you will figure it out.

### Extra features
`TODO: fill out`
Yes, if you type my birthday in command, my credit card information will show up! Can you make a guess. 

### Any bugs explained
`TODO: fill out`
Tons of bugs.
bug1:I was not null terminate the input string so everything screw up
bug2: I was not clean tokens and argvs for each iteration, so the previous tokens could mess up the current tokens
bug3: i use strstr to check > and >>, which is incorrect, check >> also handle case for >. The right call is strcmp
bug4: I was not check edge cases when there are 2 input redirections
bug5: When I open a file in >> mode, I should figure out the right mode, which is 0777