/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 13:07:20 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:46:49 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *envpath(void)
{
    t_env *envp;
    char  *p;

    envp = malloc(sizeof(*envp));
    p =getenv("PATH");
    if (!p)
        return (NULL);
    envp->path = ft_split(p, ':');
    return envp;
}

char *getpath(char *buff)
{
    char *s = buff;
    char path[100];
    int i = 0;
    
    if (!ft_strncmp(s, "cd ", 3))
    {
        s = s+3;
        while (*s == ' ')
            s++;
        while (*s != ' ' && *s)
            path[i++] = *s++;
        path[i] = 0;
        s = path;
        return (s);
    }
    return NULL;
}
