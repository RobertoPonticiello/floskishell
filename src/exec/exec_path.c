/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_bin(const char *cmd)
{
	char	*explicit;
	char	*path_env;
	char	*path_copy;

	explicit = path_handle_explicit(cmd);
	if (explicit != (char *)-1)
		return (explicit);
	path_env = env_get("PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	return (path_search_list(cmd, path_copy));
}

char	*path_handle_explicit(const char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (is_runnable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	return ((char *)-1);
}

char	*path_search_list(const char *cmd, char *path)
{
	char	*tok;
	char	*hit;
	char	*ptr;

	ptr = path;
	tok = path_next_tok(&ptr);
	while (tok)
	{
		hit = path_join(tok, cmd);
		if (hit && is_runnable(hit))
		{
			free(path);
			return (hit);
		}
		free(hit);
		tok = path_next_tok(&ptr);
	}
	free(path);
	return (NULL);
}
