/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:23:19 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 20:12:18 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	ft_putendl_fd("exit", 2);
	if (!args[1])
		exit(shell->last_exit);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	exit(ft_atoi(args[1]) % 256);
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1] || ft_strncmp(args[1], "--", 3) == 0)
		path = get_env("HOME", shell);
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env("OLDPWD", shell);
		if (!path)
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	}
	else
		path = args[1];
	return (path);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	path = get_cd_path(args, shell);
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (free(oldpwd), 1);
	}
	newpwd = getcwd(NULL, 0);
	set_env("OLDPWD", oldpwd, shell);
	set_env("PWD", newpwd, shell);
	free(oldpwd);
	free(newpwd);
	return (0);
}
