/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	overwrite_slot(int idx, char *pair, int overwrite)
{
	char	**env;

	env = env_array();
	if (overwrite)
	{
		free(env[idx]);
		env[idx] = pair;
	}
	else
		free(pair);
}

static void	append_slot(char *pair, int n)
{
	char	**newer;
	char	**env;
	int		i;

	env = env_array();
	newer = malloc(sizeof(char *) * (n + 2));
	if (!newer)
	{
		free(pair);
		return ;
	}
	i = 0;
	while (i < n)
	{
		newer[i] = env[i];
		i++;
	}
	newer[i] = pair;
	newer[i + 1] = NULL;
	free(*env_ref());
	*env_ref() = newer;
}

int	env_set(const char *name, const char *value, int overwrite)
{
	char	*pair;
	int		idx;

	if (!name || !value || ft_strchr(name, '='))
		return (-1);
	pair = env_pair_new(name, value);
	if (!pair)
		return (-1);
	idx = env_find(name);
	if (idx >= 0)
	{
		overwrite_slot(idx, pair, overwrite);
		return (0);
	}
	append_slot(pair, env_count());
	return (0);
}
