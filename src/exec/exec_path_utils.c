/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_runnable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}

char	*path_next_tok(char **ptr)
{
	char	*start;
	char	*end;

	if (!*ptr || !**ptr)
		return (NULL);
	start = *ptr;
	end = start;
	while (*end && *end != ':')
		end++;
	if (*end == ':')
	{
		*end = '\0';
		*ptr = end + 1;
	}
	else
		*ptr = end;
	return (start);
}

char	*path_join(const char *dir, const char *cmd)
{
	char	*full;
	int		dl;
	int		cl;
	int		i;
	int		j;

	dl = ft_strlen(dir);
	cl = ft_strlen(cmd);
	full = malloc(dl + cl + 2);
	if (!full)
		return (NULL);
	i = 0;
	while (i < dl)
	{
		full[i] = dir[i];
		i++;
	}
	full[i++] = '/';
	j = 0;
	while (j < cl)
		full[i++] = cmd[j++];
	full[i] = '\0';
	return (full);
}
