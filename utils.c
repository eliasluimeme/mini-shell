/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 10:09:49 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 13:29:48 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int empty_cmd(char *buff)
{
    int i = 0;

    while (buff[i])
    {
        if (!is_white_space(buff[i]))
            return (1);
        i++;
    }
    return (0);
}

void check_dir(t_exec *cmdtree)
{
    DIR *dir;
    
    dir = opendir(cmdtree->argv[0]);
    if (dir)
    {
        closedir(dir);
        errors(cmdtree->argv[0], 4);
    }
}

void	handler(int sig)
{
    sig  = 0;
    GLOBAL = 1;
	// rl_replace_line("", 0);
	// rl_on_new_line();
	write(1, "\n", 1);
	// rl_redisplay();
}
