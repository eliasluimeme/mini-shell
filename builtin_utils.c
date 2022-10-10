/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iakry <iakry@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:16:17 by iakry             #+#    #+#             */
/*   Updated: 2022/10/10 12:18:51 by iakry            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_check_for_env(char *s)
{
    if (ft_strlen(s) != 3)
        return(0);
    if (ft_tolower(s[0]) == 'e' && ft_tolower(s[1]) == 'n' && ft_tolower(s[2]) == 'v')
        return(1);
    return (0);
}

int ft_check_for_pwd(char *s)
{
    if (ft_strlen(s) != 3)
        return(0);
    if (ft_tolower(s[0]) == 'p' && ft_tolower(s[1]) == 'w' && ft_tolower(s[2]) == 'd')
        return(1);
    return (0);
}

int ft_check_for_echo(char *s)
{
    if (ft_strlen(s) != 4)
        return(0);
    if (ft_tolower(s[0]) == 'e' && ft_tolower(s[1]) == 'c' && ft_tolower(s[2]) == 'h' && ft_tolower(s[3]) == 'o')
        return(1);
    return (0);
}

int options(char *s)
{
    int i = 0;

    if (s[i] != '-')
        return(0);
    i++;
    while (s[i])
    {
        if (s[i] != 'n')
            return (0);
        i++;
    }
    return (1);   
}

int valid_name(char *s)
{
    int i = 0;

    if (!ft_isalpha(s[i]))
    {
        GLOBAL = -2;
        return(0);
    }
    while (s[i])
    {
        if (!ft_isalpha(s[i]) && !ft_isdigit(s[i]) && s[i] != '_')
        {
            GLOBAL = -2;
            return (0);
        }
        i++;
    }
    return (1);
}