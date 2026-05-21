/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "ft_printf.h"

# define MSH_PROMPT "minishell$ "
# define MSH_EXPORT_FLAG "\001"
# define MSH_EXPBUF 4096
# define MSH_NAMECAP 256
# define MSH_PATHMAX 4096

extern volatile sig_atomic_t	g_signal;

typedef enum e_tktype
{
	TK_WORD,
	TK_PIPE,
	TK_IN,
	TK_OUT,
	TK_HDOC,
	TK_APPEND
}	t_tktype;

typedef struct s_tok
{
	char			*raw;
	t_tktype		kind;
	int				quoted;
	struct s_tok	*nxt;
}	t_tok;

typedef struct s_cmd
{
	char			*bin;
	char			**argv;
	int				fd_in;
	int				fd_out;
	int				is_builtin;
	int				redir_err;
	struct s_cmd	*nxt;
}	t_cmd;

typedef struct s_builtin
{
	const char	*id;
	int			(*fn)(char **);
}	t_builtin;

/* state accessors */
int			*status_ref(void);
char		***env_ref(void);

/* signals */
void		signals_init(void);
void		signals_child(void);
void		signals_silence_parent(void);
void		signals_hdoc(void);

/* lexer */
t_tok		*lex_line(const char *src);
t_tok		*tok_new(const char *raw, t_tktype kind);
int			is_op_start(const char *src, int i);
t_tok		*lex_op(const char *src, int *idx);
char		*lex_quoted_word(const char *src, int *idx, char q);
char		*lex_bare_word(const char *src, int *idx);

/* quotes & escape */
void		quote_strip_list(t_tok *list);
char		*quote_strip_str(char *src);
int			esc_apply(const char *s, int *i, char *dst, int *j);
void		qs_single(char *src, int *i, char *dst, int *j);
void		qs_double(char *src, int *i, char *dst, int *j);
void		qs_step(char *src, int *i, char *dst, int *j);

/* expansion */
void		expand_list(t_tok **head);
char		*expand_str(const char *src);
int			expand_dollar(const char *src, int *si, char *dst, int *di);
int			expand_status(char *dst, int *di);
int			expand_name(const char *src, int start, int len, char *dst);
int			var_name_len(const char *src, int start);

/* parser */
t_cmd		*parse(const char *line);
int			syntax_ok(t_tok *toks);
t_cmd		*build_cmds(t_tok *toks);
int			apply_redir(t_cmd *cmd, t_tok *tk);
int			hdoc_one(t_cmd *cmd, t_tok *tk);
int			hdoc_many(t_cmd *cmd, t_tok *tk);
int			hdoc_count(t_tok *tk);
char		*hdoc_tmp_name(void);
int			hdoc_read(char *delim, int fd, int quoted);
int			hdoc_stage(char *delim, int quoted, int *out_fd);
void		hdoc_swap_fd(t_cmd *cmd, int fd);
int			hdoc_collect(t_tok *start, char **delims, int cap);

/* cmd build helpers */
int			is_builtin_name(const char *name);
t_cmd		*cmd_new(void);
t_cmd		*cmd_finalize(t_cmd *cmd, int n, char **argv);
void		cmd_push(t_cmd **head, t_cmd **tail, t_cmd *node);
t_tok		*cmd_skip_to_next(t_tok *tk);
t_tok		*skip_hdoc_cluster(t_tok *cur);

/* memory cleanup */
void		tok_free(t_tok *list);
void		cmd_free(t_cmd *list);
void		cmd_close_fds(t_cmd *cmd);

/* exec */
int			run_pipeline(t_cmd *list);
int			resolve_status(int status);
int			run_multi(t_cmd *list, int n);
int			cmd_count(t_cmd *list);
void		child_dispatch(t_cmd *cmd);
void		child_pipes_setup(t_cmd *cmd, int **pipes, int n, int i);
void		apply_cmd_fds(t_cmd *cmd);
int			**alloc_pipes(int n);
void		pipes_close_all(int **pipes, int n);
void		pipes_free(int **pipes, int n);
void		wait_all(pid_t *pids, int n);
void		spawn_all(t_cmd *list, int **pipes, pid_t *pids, int n);

/* path resolution */
char		*resolve_bin(const char *cmd);
char		*path_join(const char *dir, const char *cmd);
int			is_runnable(const char *path);
char		*path_next_tok(char **ptr);
char		*path_search_list(const char *cmd, char *path);
char		*path_handle_explicit(const char *cmd);

/* environment */
void		env_init(void);
char		*env_get(const char *name);
int			env_set(const char *name, const char *value, int overwrite);
int			env_unset(const char *name);
char		**env_array(void);
char		*env_pair_new(const char *name, const char *value);
int			env_find(const char *name);
int			env_count(void);
int			ident_ok(const char *name);

/* builtins */
int			bi_echo(char **argv);
int			bi_cd(char **argv);
int			bi_pwd(char **argv);
int			bi_env(char **argv);
int			bi_export(char **argv);
int			bi_unset(char **argv);
int			bi_exit(char **argv);
t_builtin	*bi_table(void);
void		run_builtin(t_cmd *cmd);
void		bi_print_exports(void);
void		bi_print_one(char *pair);

#endif
