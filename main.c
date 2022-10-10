/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 10:20:10 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:39:26 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int GLOBAL;

int complete_pipe(char *buff)
{
    int i = 0;
    int c = 0;
    int k = 0;

    while (buff[i])
    {
        if (!is_white_space(buff[i]))
        {
            c = buff[i];
            if (c == '|')
            {
                k++;
                if (k > 1)
                    return (1);
            }
            else if (k)
                k--;
        }
        i++;
    }
    if (c == '|')
        return (1);
    return (0);
}

char *getcmd()
{
    char *buff = NULL;
    char cwd[100];
    char str[100] = "\033[1;31mMιɳιSԋҽʅʅ『➜』\033[0;34m";

    getcwd(cwd, 100);
    if (isatty(fileno(stdin)))
        buff = readline(strcat(strcat(str, cwd), "『➜』\033[0m"));
    if (buff)
        return buff;
    return NULL;
}

int main(int ac, char **av, char **env)
{
    static char     *buff = "";
    int             flag_in = 0;
    int             flag_out = 0;
    int             exits = 0;
    int             pid = 0;
    t_env           *envp;
    t_envvar        *env_list;

    (void)ac;
    (void)av;
    printheader();
    envp = envpath();
    env_list = init_envvar(env);
    while ((buff = getcmd()))
    {
        signal(SIGQUIT,SIG_IGN);
	    signal(SIGINT, handler);
        if (buff && *buff)
            add_history(buff);
        if (!empty_cmd(buff) || builtin(buff, &env_list))
        {
            free(buff);
            continue;
        }
        if ((pid = forkk()) == 0)
        {
            signal(SIGQUIT,SIG_DFL);
	        signal(SIGINT, SIG_DFL);
            executor(parsecmd(buff), envp, &flag_out, &flag_in, &env_list);
            free(buff);
        }
        signal(SIGINT,SIG_IGN);
        wait(&exits);
        if ((WTERMSIG(exits) == 3 || WTERMSIG(exits) == 2) && GLOBAL != -1)
            GLOBAL = 128 + WTERMSIG(exits);
        else
            GLOBAL = WEXITSTATUS(exits);
    }
    return(0);
}