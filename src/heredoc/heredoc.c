/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:30:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/05 15:01:28 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <errno.h>

/*
** Crea un nome file temporaneo unico per gli heredoc
*/
char	*create_temp_filename(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*filename;
	char		*temp;
	char		*pid_str;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
		
	pid_str = ft_itoa(getpid());
	if (!pid_str)
	{
		free(num_str);
		return (NULL);
	}
	
	temp = ft_strjoin("/tmp/heredoc_", num_str);
	free(num_str);
	if (!temp)
	{
		free(pid_str);
		return (NULL);
	}
	
	filename = ft_strjoin(temp, "_");
	free(temp);
	if (!filename)
	{
		free(pid_str);
		return (NULL);
	}
	
	temp = ft_strjoin(filename, pid_str);
	free(filename);
	free(pid_str);
	
	return (temp);
}

/*
** Espande le variabili in una linea dell'heredoc se necessario
*/
static int	expand_heredoc_line(char *line, int fd, int has_quotes)
{
	char	*expanded;
	
	// Se il delimiter è quotato, non espandere le variabili
	if (has_quotes)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		return (0);
	}
	
	// Espandi le variabili nella linea
	expanded = expand_string(line);
	if (!expanded)
		return (-1);
	
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (0);
}

/*
** Legge il contenuto di un heredoc gestendo delimiters multipli
*/
static int	read_heredoc_content(char *delimiter, int fd, char **other_delimiters, int delimiter_count, int has_quotes)
{
	char	*line;
	char	*clean_delimiter;
	int		i;

	// Rimuovi le quotes dal delimiter se presenti
	clean_delimiter = process_token_quotes(delimiter);
	if (!clean_delimiter)
		return (-1);

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf(2, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", clean_delimiter);
			break;
		}
		
		// Controlla se è il nostro delimiter
		if (ft_strncmp(line, clean_delimiter, ft_strlen(clean_delimiter)) == 0 
			&& ft_strlen(line) == ft_strlen(clean_delimiter))
		{
			free(line);
			break;
		}
		
		// Controlla se è un altro delimiter (nel caso di heredoc multipli)
		// In questo caso lo trattiamo come contenuto normale
		if (other_delimiters)
		{
			for (i = 0; i < delimiter_count; i++)
			{
				if (other_delimiters[i] && 
					ft_strncmp(line, other_delimiters[i], ft_strlen(other_delimiters[i])) == 0 
					&& ft_strlen(line) == ft_strlen(other_delimiters[i]))
				{
					// È un altro delimiter, ma lo scriviamo come contenuto
					break;
				}
			}
		}
		
		// Passa il flag has_quotes per controllare l'espansione
		if (expand_heredoc_line(line, fd, has_quotes) == -1)
		{
			free(line);
			free(clean_delimiter);
			return (-1);
		}
		free(line);
	}
	
	free(clean_delimiter);
	return (0);
}

/*
** Conta gli heredoc consecutivi in una sequenza di token
*/
static int	count_consecutive_heredocs(t_token *start)
{
	t_token	*current;
	int		count;

	count = 0;
	current = start;
	
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_HEREDOC)
			count++;
		current = current->next;
	}
	
	return (count);
}

/*
** Gestisce heredoc multipli come << EOF1 << EOF2
** In bash, per il comando "cat << EOF1 << EOF2", bash legge prima EOF2 e poi EOF1
** Il risultato finale è che solo il contenuto di EOF1 viene passato a cat
*/
int	handle_multiple_heredocs(t_command *cmd, t_token *start_token)
{
	t_token		*current;
	char		**delimiters;
	int			count;
	int			i;
	char		*filename;
	int			fd;
	int			result;

	// Conta gli heredoc e raccoglie i delimitatori
	count = count_consecutive_heredocs(start_token);
	if (count <= 1)
		return (handle_single_heredoc(cmd, start_token));
	
	delimiters = malloc(sizeof(char *) * count);
	if (!delimiters)
		return (-1);
	
	// Raccogli tutti i delimitatori
	current = start_token;
	i = 0;
	while (current && current->type != TOKEN_PIPE && i < count)
	{
		if (current->type == TOKEN_HEREDOC && current->next)
		{
			delimiters[i] = current->next->value;
			i++;
		}
		current = current->next;
	}
	
	// Leggi tutti gli heredoc in ordine inverso (come bash)
	for (i = count - 1; i >= 0; i--)
	{
		filename = create_temp_filename();
		if (!filename)
		{
			free(delimiters);
			return (-1);
		}
		
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
		if (fd == -1)
		{
			free(filename);
			free(delimiters);
			return (-1);
		}
		
		// Leggi il contenuto dell'heredoc
		result = read_heredoc_content(delimiters[i], fd, NULL, 0, 0);
		close(fd);
		
		if (result == -1)
		{
			unlink(filename);
			free(filename);
			free(delimiters);
			return (-1);
		}
		
		// Se questo è l'ultimo heredoc (i == 0), usalo come input
		if (i == 0)
		{
			// Riapri in lettura
			fd = open(filename, O_RDONLY);
			unlink(filename);
			free(filename);
			
			if (fd == -1)
			{
				free(delimiters);
				return (-1);
			}
			
			// Chiudi il precedente input fd se esiste
			if (cmd->in_fd >= 0)
			{
				close(cmd->in_fd);
				cmd->in_fd = -1;
			}
			cmd->in_fd = fd;
		}
		else
		{
			// Non è l'ultimo, rimuovi il file
			unlink(filename);
			free(filename);
		}
	}
	
	free(delimiters);
	return (0);
}

/*
** Gestisce un singolo heredoc
*/
int	handle_single_heredoc(t_command *cmd, t_token *curr)
{
	char	*filename;
	int		fd;

	filename = create_temp_filename();
	if (!filename)
		return (-1);
	
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(filename);
		return (-1);
	}
	
	if (read_heredoc_content(curr->next->value, fd, NULL, 0, curr->next->has_quotes) == -1)
	{
		close(fd);
		unlink(filename);
		free(filename);
		return (-1);
	}
	
	close(fd);
	
	// Riapri in lettura
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	
	if (fd == -1)
		return (-1);
	
	// Chiudi il precedente input fd se esiste
	if (cmd->in_fd >= 0)
	{
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	cmd->in_fd = fd;
	return (0);
}

/*
** Conta gli heredoc consecutivi in una sequenza di token
*/
int	count_heredocs(t_token *start)
{
	return (count_consecutive_heredocs(start));
} 