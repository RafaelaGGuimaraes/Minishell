/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:31:25 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 18:27:50 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	wait_child(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	restore_stdio(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	exec_builtin_cmd(t_cmd *cmd, t_shell *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	status;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return (perror("dup"), 1);
	if (apply_redirs(cmd->redirs) == -1)
	{
		restore_stdio(stdin_backup, stdout_backup);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	{
		close(stdin_backup);
		close(stdout_backup);
		return (run_builtin(cmd, shell));
	}
	status = run_builtin(cmd, shell);
	restore_stdio(stdin_backup, stdout_backup);
	return (status);
}

static int	apply_redirs_and_restore(t_redir *redirs)
{
	t_redir	*curr;
	int		fd;

	curr = redirs;
	while (curr)
	{
		fd = open_redir(curr);
		if (fd < 0)
			return (-1);
		close(fd);
		curr = curr->next;
	}
	return (0);
}

int	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->redirs)
			return (apply_redirs_and_restore(cmd->redirs));
		return (0);
	}
	if (is_builtin(cmd->args[0]))
		return (exec_builtin_cmd(cmd, shell));
	return (exec_fork(cmd, shell));
}
