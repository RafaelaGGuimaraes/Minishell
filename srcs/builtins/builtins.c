/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:25:37 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:17:13 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}

static int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static int	ft_echo(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (args[i] && is_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

static int	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
		ft_putendl_fd(shell->env[i++], STDOUT_FILENO);
	return (0);
}

int	run_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(shell));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, shell));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, shell));
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, shell));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args, shell));
	return (0);
}
