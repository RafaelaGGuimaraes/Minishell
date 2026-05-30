/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:32:02 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 22:08:29 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (apply_redirs(cmd->redirs, shell) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, shell));
	exec_external(cmd, shell);
}

int	execute(t_shell *shell)
{
	t_cmd	*cmd;
	int		status;

	cmd = shell->cmds;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (count_cmds(cmd) == 1)
		status = exec_cmd(cmd, shell);
	else
		status = exec_pipeline(cmd, shell);
	shell->last_exit = status;
	return (status);
}
