/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:42:58 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 15:45:25 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

extern int	g_signal;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
}	t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}	t_quote_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type	quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				heredoc_quoted;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char	**env;
	t_cmd	*cmds;
	int		last_exit;
	int		exit_status;
	t_token	*tokens;
}	t_shell;

t_token		*tokenize(char *line, t_shell *shell);
void		free_tokens(t_token *lst);

/* lexer_utils.c — leitura e criação de tokens */
t_token		*new_token(char *value, t_token_type type, t_quote_type quoted);
char		*read_word(char *line, int *i);
char		*read_quoted(char *line, int *i, t_quote_type *quoted);
int			skip_spaces(char *line, int i);
int			is_separator(char c);

/* lexer_list.c — manipulação da lista de tokens */
void		token_addback(t_token **lst, t_token *new);

/* parser.c — parsing principal */
t_cmd		*parse(t_token *tokens, t_shell *shell);
void		free_cmds(t_cmd *lst);
void		free_redirs(t_redir *lst);

/* parser_utils.c — criação e manipulação de cmds */
t_cmd		*new_cmd(void);
void		cmd_addback(t_cmd **lst, t_cmd *new);
int			add_arg(t_cmd *cmd, char *value);
void		redir_addback(t_redir **lst, t_token_type type, char *file, int hq);

/* expand.c — expande $VAR e $? respeitando o contexto de aspas */
char		*expand(char *str, t_quote_type quoted, t_shell *shell);

/* syntax.c — valida a sequência de tokens antes do parse       */
int			syntax_check(t_token *tokens);
char		*find_executable(char *cmd, t_shell *shell);

/* executor.c — único ponto de entrada que o main.c chama       */
int			execute(t_shell *shell);

/* executor_utils.c — funções de comando único                  */
int			exec_cmd(t_cmd *cmd, t_shell *shell);

/* executor_external.c — resolução de path e execve          	*/
int			exec_pipeline(t_cmd *cmd, t_shell *shell);
int			count_cmds(t_cmd *cmd);
void		exec_external(t_cmd *cmd, t_shell *shell);
int			expand_heredocs(t_cmd *cmds, t_shell *shell);

/* builtins.c — interface pública mínima para o executor        */
int			is_builtin(char *cmd);
int			run_builtin(t_cmd *cmd, t_shell *shell);

/* builtins_utils.c + builtins_env.c — públicas por cruzar arquivos */
int			ft_cd(char **args, t_shell *shell);
int			ft_exit(char **args, t_shell *shell);
int			ft_export(char **args, t_shell *shell);
int			ft_unset(char **args, t_shell *shell);

/* redirs.c — gerenciamento de redirecionamentos e heredoc */
void		exec_child(t_cmd *cmd, t_shell *shell, int fd_in, int *p_fd);
int			wait_pipeline(pid_t last_pid);
int			handle_heredoc(char *file, int heredoc_quoted, t_shell *shell);
int			exec_pipeline(t_cmd *cmd, t_shell *shell);
int			apply_redirs(t_redir *redirs);
void		handle_sigint_child(int sig);

/* signals.c — apenas setup_signals é chamado pelo main.c       */
void		setup_signals(void);
void		setup_signals_child(void);

/* env.c — manipulação das variáveis de ambiente */
char		**copy_env(char **env);
char		*get_env(char *name, t_shell *shell);
int			set_env(char *name, char *value, t_shell *shell);
int			unset_env(char *name, t_shell *shell);

/* env_utils.c — auxiliares do env */
int			find_env_index(char **env, char *name);
char		*build_env_entry(char *name, char *value);

/* utils.c — funções utilitárias gerais                         */
void		free_array(char **arr);
void		shell_error(char *cmd, char *msg, int code);
char		*append_part(char *result, char *part);
void		cleanup_shell(t_shell *shell);

#endif
