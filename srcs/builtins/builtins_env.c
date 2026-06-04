/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 20:16:29 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 17:52:40 by rgomes-g         ###   ########.fr       */
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

void	sort_env(char **sorted, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[j])) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
			j++;
		}
		i++;
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
