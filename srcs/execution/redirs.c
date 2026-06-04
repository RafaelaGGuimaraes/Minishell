/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:56:54 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 15:44:18 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_redir(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file);
	}
	return (fd);
}

pid_t	exec_pipeline_node(t_cmd *cmd, t_shell *shell, int *fd_in)
{
	int		p_fd[2];
	pid_t	pid;

	if (cmd->next && pipe(p_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		exec_child(cmd, shell, *fd_in, p_fd);
	if (*fd_in != 0)
		close(*fd_in);
	if (cmd->next)
	{
		close(p_fd[1]);
		*fd_in = p_fd[0];
	}
	return (pid);
}

int	exec_pipeline(t_cmd *cmd, t_shell *shell)
{
	int				fd_in;
	pid_t			pid;
	pid_t			last_pid;
	struct sigaction	sa;
	struct sigaction	sa_old;
	int status;

	fd_in = 0;
	last_pid = -1;
	while (cmd)
	{
		pid = exec_pipeline_node(cmd, shell, &fd_in);
		if (pid == -1)
			return (1);
		last_pid = pid;
		cmd = cmd->next;
	}
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_child;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &sa_old);
	status = wait_pipeline(last_pid);
	sigaction(SIGINT, &sa_old, NULL);
	return (status);
}

int	apply_redirs(t_redir *redirs)
{
	t_redir	*curr;
	int		fd;

	curr = redirs;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
			fd = curr->fd;
		else
			fd = open_redir(curr);
		if (fd < 0)
			return (-1);
		if (curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		curr = curr->next;
	}
	return (0);
}
