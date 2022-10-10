/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 13:00:15 by hkhalil           #+#    #+#             */
/*   Updated: 2022/10/10 13:27:22 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_builtin(t_exec *cmdtree, t_envvar **env_list)
{
    if (!ft_strcmp(cmdtree->argv[0], "exit"))
    {
        ft_exit(cmdtree);
        exit(GLOBAL);
    }
    if (!ft_strcmp(cmdtree->argv[0], "cd"))
    {
        ft_cd(cmdtree, *env_list);
        if (errno == 13 || errno == 2)
            exit(1);
        exit(0);
    }
    if (ft_check_for_pwd(cmdtree->argv[0]))
        ft_pwd();
    if (ft_check_for_echo(cmdtree->argv[0]))
        ft_echo(cmdtree);
    if (ft_check_for_env(cmdtree->argv[0]))
        ft_env(cmdtree, *env_list);
    if (!ft_strcmp(cmdtree->argv[0], "export"))
    {
        ft_export(cmdtree, env_list);
        if (GLOBAL == -2)
            exit(1);
        exit(0);
    }
    if (!ft_strcmp(cmdtree->argv[0], "unset"))
    {
        ft_unset(cmdtree, env_list);
        if (GLOBAL == -2)
            exit(1);
        exit (0);
    }
}

void exec_pipe(t_pip *piptree, t_env *envp, int *flag_out, int *flag_in, t_envvar **env_list)
{
    int     id;
    int     p[2];
    int     mid;
    int     last;
    
    if (pipe(p) < 0)
        perror("pipe error");
    if ((id = forkk()) == 0)
    {
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        executor(piptree->left, envp, flag_out, flag_in, env_list);
    }
    if ((id = forkk()) == 0)
    {
        close(p[1]);
        dup2(p[0], 0);
        close(p[0]);
        executor(piptree->right, envp, flag_out, flag_in, env_list);
    }
    close(p[0]);
    close(p[1]);
    waitpid(id, &last , 0);
    while(wait(&mid) > 0)
    {
        if ((WTERMSIG(mid) == 3 || WTERMSIG(mid) == 2) && (WTERMSIG(last) != 3 && WTERMSIG(last) != 2))
        GLOBAL = -1;
    }
    exit(WEXITSTATUS(last));
}

void exec_redir(t_redir *redtree, t_env *envp, int *flag_in, int *flag_out, t_envvar **env_list)
{
    int open_fd = 0;
    
    open_fd = open(redtree->file, redtree->mode, 0666);
    if (open_fd < 0)
        exit(1);
    if ((!(*flag_in) && !(redtree->fd)) || (!(*flag_out) && redtree->fd))
    {
        if (!(redtree->fd))
            *flag_in = 1;
        else
            *flag_out = 1;
        dup2(open_fd, redtree->fd);
    }
    close(open_fd);
    executor(redtree->cmd, envp, flag_out, flag_in, env_list);
}

void run(t_exec *cmdtree, t_env *envp)
{
    char *s = NULL;
    char *str = NULL;
    int i = -1;
    
    if (ft_strchr(cmdtree->argv[0], '/'))
    {   
        if(access(cmdtree->argv[0], F_OK) != -1)
            s = ft_strdup(cmdtree->argv[0]);
        else
            errors(cmdtree->argv[0], 4);
    }
    else
    {
        while (envp && envp->path[++i])
        {
            s  = ft_strjoin3(ft_strjoin(envp->path[i], "/"), cmdtree->argv[0]);
            if (access(s, F_OK) != -1)
                str = ft_strdup(s);
        }
        if (!str)
               str = ft_strdup(cmdtree->argv[0]);
    }
    execve(str, cmdtree->argv, envp->path);
    free(str);
    errors(cmdtree->argv[0], 0);
}

void    executor(t_cmd *tree, t_env *envp, int *flag_out, int *flag_in, t_envvar **env_list)
{
    if (tree->type != PIPE)
        find_in_redir(tree);
    if (tree->type == PIPE)
        exec_pipe((t_pip *)tree, envp, flag_out, flag_in, env_list);
    else if (tree->type == REDIR)
        exec_redir((t_redir *)tree, envp, flag_in, flag_out, env_list);
    else
    {
        if (!empty_cmd(((t_exec *)tree)->argv[0]))
            exit(0);
        exec_builtin((t_exec *)tree, env_list);
        check_dir((t_exec *)tree);
        run((t_exec *)tree, envp);
    }
}