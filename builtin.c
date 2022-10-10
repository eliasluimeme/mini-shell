/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 13:02:29 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:42:45 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(t_exec *node)
{
    int      i = 0;
    int      k = 0;

    while (node->argv[k])
        k++;
    if (node->argv[1])
    {
        i = 0;
        while (node->argv[1][i])
        {
            if (!ft_isdigit(node->argv[1][i]))
            {
                write(2, "exit: ", 7);
                write(2, node->argv[1], ft_strlen(node->argv[1]));
                write(2, ": numeric argument required\n", 29);
                exit(255);
            }
            i++;
        }
        if (k > 2)
        {
            write(2, "exit: too many arguments\n", 26);
            GLOBAL = 1;
            //clean everything
            return ;
        }
        exit(ft_atoi(node->argv[1]));
    }
    exit(GLOBAL);
}

void ft_export(t_exec *cmd, t_envvar **env)
{
    int i = 1;
    char    **v;
    t_envvar    *addr;
    
    
    while (cmd->argv[i])
    {
        addr = *env;
        v = ft_split(cmd->argv[i], '=');
        if (ft_strchr(cmd->argv[i], '='))
        {
            if (if_exist_add(&addr, v, 1));
            else
            { 
                if (valid_name(v[0]))
                    ft_lstadd_back(env, ft_lstadd_new(v[0], v[1]));
                else
                {  
                    GLOBAL = -2;
                    printf("export: `%s': not a valid identifier\n", cmd->argv[i]);
                }
            }
        }
        i++;
    }
}

void ft_unset(t_exec *cmd, t_envvar **env)
{
    int i = 1;

    while (cmd->argv[i])
    {
        if (!valid_name(cmd->argv[i]))
        {
            GLOBAL = -2;
            printf("unset: `%s': not a valid identifier\n",cmd->argv[i]);
        }
        else
            if_exist_delete(env, cmd->argv[i]);
        i++;
    }
}

void ft_echo(t_exec *cmd)
{
    int i = 1;
    int noption = 0;

    if (cmd->argv[i])
    {
        while (options(cmd->argv[i]))
        {
            noption = 1;
            i++;
        }
        while (cmd->argv[i])
        {
            printf("%s", cmd->argv[i]);
            i++;
            if (cmd->argv[i])
                printf(" ");
        }
    }
    if (noption == 0)
        printf("\n");
    exit(0);
}

void check_global()
{
    if (GLOBAL == -2)
        GLOBAL = 1;
    else
        GLOBAL = 0;
}

int builtin(char *buff, t_envvar **env)
{
    t_cmd   *tree;
    t_exec  *cmd;

    tree = parsecmd(buff);
    if (tree->type == EXEC)
    {
        cmd = (t_exec *)tree;
        if (!ft_strcmp(cmd->argv[0], "exit"))
        {
            ft_exit(cmd);
            return (1);
        }
        else if (!ft_strcmp(cmd->argv[0], "cd"))
        {
            ft_cd(cmd, *env);
            return (1);
        }
        else if (!ft_strcmp(cmd->argv[0], "unset"))
        {
           ft_unset(cmd, env);
           check_global();
           return (1);
        }
        else if (!ft_strcmp(cmd->argv[0], "export"))
        {
           ft_export(cmd, env);
           check_global();
           return (1);
        }
    }
    return (0);
}