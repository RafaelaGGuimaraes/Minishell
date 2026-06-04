/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:30:26 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 15:00:07 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_executable(cmd->args[0], shell);
	if (!path)
	{
		if (shell->exit_status == 126)
		{
			cleanup_shell(shell);
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putendl_fd(": command not found", 2);
		cleanup_shell(shell);
		exit(127);
	}
	if (execve(path, cmd->args, shell->env) == -1)
	{
		perror(cmd->args[0]);
		free(path);
		cleanup_shell(shell);
		exit(126);
	}
}

int	count_cmds(t_cmd *cmd)
{
	int	n;

	n = 0;
	while (cmd)
	{
		n++;
		cmd = cmd->next;
	}
	return (n);
}

int	wait_pipeline(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		last_status;

	last_status = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	return (last_status);
}

int	expand_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				redir->fd = handle_heredoc(redir->file, redir->heredoc_quoted, shell);
				if (redir->fd < 0)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	handle_heredoc(char *delim, int heredoc_quoted, t_shell *shell)
{
	int		p_fd[2];
	char	*line;
	char	*expanded;

	if (pipe(p_fd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (!heredoc_quoted)
		{
			expanded = expand(line, QUOTE_NONE, shell);
			free(line);
			line = expanded;
		}
		ft_putendl_fd(line, p_fd[1]);
		free(line);
	}
	close(p_fd[1]);
	return (p_fd[0]);
}
