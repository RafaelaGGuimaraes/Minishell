/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chiaronero <chiaronero@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:16:29 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/27 22:31:02 by chiaronero       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_env_name(char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export(char **env)
{
	char	*equal;
	char	*name;
	int		j;

	j = 0;
	while (env && env[j])
	{
		equal = ft_strchr(env[j], '=');
		if (equal)
		{
			name = ft_substr(env[j], 0, equal - env[j]);
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(name, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(equal + 1, STDOUT_FILENO);
			ft_putendl_fd("\"", STDOUT_FILENO);
			free(name);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(env[j], STDOUT_FILENO);
		}
		j++;
	}
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*equal;

	if (!is_valid_env_name(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*equal = '\0';
		set_env(arg, equal + 1, shell);
		*equal = '=';
	}
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	if (!args[i])
	{
		print_export(shell->env);
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(args[i], shell) == 1)
			ret = 1;
		i++;
	}
	return (ret);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env(args[i], shell);
		i++;
	}
	return (0);
}
