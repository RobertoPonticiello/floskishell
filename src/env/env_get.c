/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get(const char *name)
{
	char	**env;
	int		nlen;
	int		i;

	env = env_array();
	if (!name || !env)
		return (NULL);
	nlen = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, nlen) == 0 && env[i][nlen] == '=')
			return (env[i] + nlen + 1);
		i++;
	}
	return (NULL);
}

int	env_find(const char *name)
{
	char	**env;
	int		nlen;
	int		i;

	env = env_array();
	if (!name || !env)
		return (-1);
	nlen = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, nlen) == 0 && env[i][nlen] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	env_count(void)
{
	char	**env;
	int		n;

	env = env_array();
	n = 0;
	if (env)
		while (env[n])
			n++;
	return (n);
}

char	*env_pair_new(const char *name, const char *value)
{
	char	*pair;
	int		nlen;
	int		vlen;
	int		i;
	int		j;

	nlen = ft_strlen(name);
	vlen = ft_strlen(value);
	pair = malloc(nlen + vlen + 2);
	if (!pair)
		return (NULL);
	i = 0;
	while (i < nlen)
	{
		pair[i] = name[i];
		i++;
	}
	pair[i++] = '=';
	j = 0;
	while (j < vlen)
		pair[i++] = value[j++];
	pair[i] = '\0';
	return (pair);
}
