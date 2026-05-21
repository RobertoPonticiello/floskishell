/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hdoc_read.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	matches_delim(char *line, char *delim)
{
	return (ft_strncmp(line, delim, ft_strlen(delim)) == 0
		&& ft_strlen(line) == ft_strlen(delim));
}

static void	push_line(char *line, int fd, int quoted)
{
	char	*expanded;

	if (quoted)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		return ;
	}
	expanded = expand_str(line);
	if (!expanded)
		return ;
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static int	read_loop(char *delim, int fd, int quoted)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal != SIGINT)
				ft_fprintf(2, "minishell: warning: here-document"
					" delimited by end-of-file (wanted `%s')\n", delim);
			break ;
		}
		if (matches_delim(line, delim))
		{
			free(line);
			break ;
		}
		push_line(line, fd, quoted);
		free(line);
	}
	return (0);
}

int	hdoc_read(char *delim, int fd, int quoted)
{
	char	*clean;
	int		saved_stdin;
	int		ret;

	clean = quote_strip_str(delim);
	if (!clean)
		return (-1);
	saved_stdin = dup(STDIN_FILENO);
	signals_hdoc();
	ret = read_loop(clean, fd, quoted);
	signals_init();
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	free(clean);
	if (g_signal == SIGINT)
	{
		*status_ref() = 130;
		g_signal = 0;
		return (-1);
	}
	return (ret);
}
