#include "../include/minishell.h"



void handle_sigint(int sig)
{
    (void)sig;

    //clear current line
    write(STDOUT_FILENO, "\n", 1);
    //erase the current line
    rl_replace_line("", 0);
    //move the cursor to the beginning of the line
    rl_on_new_line();
    //redisplay prompt
    rl_redisplay();

}

void init_signals(void) {
    
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
}