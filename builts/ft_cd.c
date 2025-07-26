/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:50:58 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/20 13:50:11 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int other_path(char **args)
{
    char *oldpwd;

    oldpwd = getenv("OLDPWD");
    if (ft_strcmp(args[1], "..") == 0)
    {
        if (chdir("..") != 0)
           return (write(1, "cd: error\n", 10), 0);
        return (1);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        if (oldpwd && chdir(oldpwd) != 0)
            return (write(1, "cd: OLDPWD not set\n", 20), 0);
        return (1);
    }
    else if(args[1][0] == '/')
    {
        if (chdir(args[1]) != 0)
            return (write(1, "cd: error\n", 10), 0);
        return (1);
    }
    else if (chdir(args[1]) != 0)
      return (write(1, "cd: error\n", 10), 0); 
    return (1); 
}    

int exec_cd(char **args)
{ 
    if (args[1] == NULL)
    {
        if (chdir(getenv("HOME")) != 0)
            return (write(2, "cd: error\n", 10), 1);  // Error → 1
        return (0);  // Éxito
    }

    if (chdir(args[1]) != 0)
        return (write(2, "cd: error\n", 10), 1);  // Error
    return (0);  // Éxito
}

