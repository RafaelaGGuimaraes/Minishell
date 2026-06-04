/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:14:21 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 18:14:41 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigint_heredoc(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_done = 1;
}

int	check_sigint_hook(void)
{
	if (g_signal == SIGINT)
		rl_done = 1;
	return (0);
}

static void	heredoc_cleanup(int *p_fd, struct sigaction *sa_old)
{
	close(p_fd[0]);
	close(p_fd[1]);
	rl_event_hook = NULL;
	sigaction(SIGINT, sa_old, NULL);
}

static void	heredoc_write_line(char *line, int heredoc_quoted,
		t_shell *shell, int write_fd)
{
	char	*expanded;

	if (!heredoc_quoted)
	{
		expanded = expand(line, QUOTE_NONE, shell);
		free(line);
		line = expanded;
	}
	ft_putendl_fd(line, write_fd);
	free(line);
}

int	handle_heredoc(char *delim, int heredoc_quoted, t_shell *shell)
{
	int					p_fd[2];
	char				*line;
	struct sigaction	sa;
	struct sigaction	sa_old;

	if (pipe(p_fd) == -1)
		return (perror("pipe"), -1);
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &sa_old);
	g_signal = 0;
	rl_event_hook = check_sigint_hook;
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (free(line), heredoc_cleanup(p_fd, &sa_old), -1);
		if (!line || ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
			return (free(line), rl_event_hook = NULL,
				sigaction(SIGINT, &sa_old, NULL), close(p_fd[1]), p_fd[0]);
		heredoc_write_line(line, heredoc_quoted, shell, p_fd[1]);
	}
}
