/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

static char	**clone_environ(void)
{
	char	**copy;
	int		n;
	int		i;

	n = 0;
	while (environ[n])
		n++;
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(environ[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	seed_defaults(void)
{
	char	cwd[MSH_PATHMAX];

	if (!env_get("PWD") && getcwd(cwd, MSH_PATHMAX))
		env_set("PWD", cwd, 1);
	if (!env_get("SHLVL"))
		env_set("SHLVL", "1", 1);
}

void	env_init(void)
{
	*env_ref() = clone_environ();
	seed_defaults();
}

char	**env_array(void)
{
	if (!*env_ref())
		*env_ref() = clone_environ();
	return (*env_ref());
}
