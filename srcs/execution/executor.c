/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:32:02 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 18:26:42 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_fork(t_cmd *cmd, t_shell *shell)
{
	pid_t				pid;
	struct sigaction	sa;
	struct sigaction	sa_old;
	int					status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		setup_signals_child();
		if (apply_redirs(cmd->redirs) == -1)
			exit(1);
		exec_external(cmd, shell);
	}
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_child;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &sa_old);
	status = wait_child(pid);
	sigaction(SIGINT, &sa_old, NULL);
	return (status);
}

static void	setup_pipe_child(int fd_in, int *p_fd, t_cmd *cmd)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next)
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[0]);
		close(p_fd[1]);
	}
}

void	exec_child(t_cmd *cmd, t_shell *shell, int fd_in, int *p_fd)
{
	setup_signals_child();
	setup_pipe_child(fd_in, p_fd, cmd);
	if (apply_redirs(cmd->redirs) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, shell));
	exec_external(cmd, shell);
}

static int	apply_redirs_no_cmd(t_redir *redirs)
{
	t_redir	*curr;
	int		fd;

	curr = redirs;
	while (curr)
	{
		fd = open_redir(curr);
		if (fd < 0)
			return (1);
		close(fd);
		curr = curr->next;
	}
	return (0);
}

int	execute(t_shell *shell)
{
	t_cmd	*cmd;
	int		status;

	cmd = shell->cmds;
	if (!cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->redirs)
			status = apply_redirs_no_cmd(cmd->redirs);
		else
			return (0);
	}
	else if (count_cmds(cmd) == 1)
		status = exec_cmd(cmd, shell);
	else
		status = exec_pipeline(cmd, shell);
	shell->last_exit = status;
	return (status);
}
