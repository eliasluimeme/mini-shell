/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:21:55 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:28:35 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **if_exist_add(t_envvar **env, char **s, int flag)
{
    char    **v= malloc(sizeof(char *) *3);

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    while ( env && *env)
    {
        if (!ft_strcmp((*env)->name, s[0]))
        {
            v = malloc(sizeof(char *) *3);
            v[0] = ft_strdup((*env)->name);
            if (flag)
            {
                if (s[1])
                {
                    (*env)->value = ft_strdup(s[1]);
                    v[1] = ft_strdup((*env)->value);
                }
                else
                    v[1] = 0;
            }
            else
                v[1] = ft_strdup((*env)->value);
            v[2]=0;
            return (v);
        }
        (*env) = (*env)->next;
    }
    return (0);
}

char *if_exist_delete(t_envvar **env, char *s)
{
    t_envvar    *tmp;
    t_envvar    *prev;
    t_envvar    *next;
    t_envvar    *list;
    int         i = 0;
    int         n = ft_lstsize(*env);

    prev = *env;
    list = *env;
    while (list)
    {
        tmp= list;
        if (!ft_strcmp(list->name, s))
        {
            if (i == 0)
             {
                tmp = list;
                env = &(tmp->next);
                free(tmp->name);
                free(tmp->value);
                free(tmp);
            }
            else if (i + 1 == n)
            {
                tmp = list;
                free(tmp->name);
                free(tmp->value);
                free(tmp);
                prev->next = NULL;
            }
            else
            {
                tmp = list;
                next = tmp->next;
                free(tmp->name);
                free(tmp->value);
                free(tmp);
                prev->next=next;
            }
            break;
        }
        prev = list;
        list = list->next;
        i++;
    }
    return (0);
}

void ft_cd(t_exec *cmd, t_envvar *env)
{
    if (cmd->argv[1])
        if (chdir(cmd->argv[1]))
        {
            write(2, "cd: ",5);
            perror(cmd->argv[1]);
            GLOBAL = 1;
            return ;
        }
    if (!(cmd->argv[1]))
        while (env)
        {
            if (!ft_strcmp(env->name, "HOME"))
                chdir(env->value);
            env = env->next;
        }
        GLOBAL = 0;
}

void ft_env(t_exec *cmd, t_envvar *env)
{
    if (cmd->argv[1])
    {
        printf("env: %s: No such file or directory\n", cmd->argv[1]);
        exit(127);
    } 
    else
    {
        while (env)
        {
            printf("%s=%s\n", env->name, env->value);
            env = env->next;
        }
    }
    exit(0);
}

void ft_pwd(void)
{
    char cwd[100];

    if (getcwd(cwd, 100))
        printf("%s\n", cwd);
    exit(0);
}