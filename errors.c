/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:05:19 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:06:40 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void errors(char *name, int flag)
{
    if (flag == 4)
    {
        if (errno == 22)
        {
            write(2, name, ft_strlen(name));
            write(2, ": is a directory\n", 18);
            exit(126);
        }
        perror(name);
        exit(126);
    }
    if (flag == 2 || flag == 3)
    {
        write(2,name, ft_strlen(name));
        if (flag == 2)
            write(2, ": Permission denied\n", 21);
        else
             write(2, ": No such file or directory\n", 29);
        exit(1);
    }
    if (errno == 2 || errno == 13)
    {
        if (ft_strchr(name, '/'))
                perror(name);
        else
        {
            write(2, name, ft_strlen(name));
            write(2, ": command not found\n",21);
        }
        if (errno == 2)
            exit(127);
        else if (errno == 13)
            exit(126);
    } 
}

void check_in_files(t_cmd *redir)
{
    t_redir   *tmp;
    int     fd;

        tmp = (t_redir *)redir;
        fd = open(tmp->file, tmp->mode);
        if (fd < 0)
        { 
            close(fd);
            if (!access(tmp->file, F_OK))
                errors(tmp->file, 2);
            else
                errors(tmp->file, 3);
        }
        close(fd);
}

void find_in_redir(t_cmd *tree)
{
    t_redir  *tree2;

    if (tree->type == REDIR)
    {
        tree2 = (t_redir *)tree;
        find_in_redir(tree2->cmd);
        if (tree2->fd == 0)
           check_in_files(tree); 
    }
}