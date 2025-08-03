/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:33:09 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/25 22:24:32 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void set_exit_status_direct(int code)
{
    g_last_exit_status.last_exit_code = code;
    g_last_exit_status.exit_called = true;
} 
void update_last_exit_status(t_last_exit_status *status_struct, int new_status)
{
    if (WIFEXITED(new_status)) {
        status_struct->last_exit_code = WEXITSTATUS(new_status);
      //  printf("Process exited normally with code %d\n", status_struct->last_exit_code);
    } else if (WIFSIGNALED(new_status)) {
        status_struct->last_exit_code = 128 + WTERMSIG(new_status);
       // printf("Process terminated by signal %d\n", WTERMSIG(new_status));
    } else if (new_status >= 0 && new_status <= 255) {
        // 🚨 Consideramos que es un código directo (por ejemplo de un builtin)
        status_struct->last_exit_code = new_status;
        //printf("Process exited (builtin or manual) with code %d\n", new_status);
    } else {
        // Caso extraño
        status_struct->last_exit_code = 1;
        printf("Warning: Unexpected status value: %d\n", new_status);
    }
    status_struct->exit_called = true;
}

void expand_exit_status(char **args, t_last_exit_status *status)
{
    for (int i = 0; args[i]; i++)
    {
        // Si el argumento empieza con "$?"
        if (ft_strncmp(args[i], "$?", 2) == 0)
        {
            char *rest = args[i] + 2;  // Todo después de "$?"
            char *exit_code_str = ft_itoa(status->last_exit_code);
            char *new_arg = ft_strjoin(exit_code_str, rest);
            
            free(args[i]);
            free(exit_code_str);
            args[i] = new_arg;
            break;
        }
    }
}


