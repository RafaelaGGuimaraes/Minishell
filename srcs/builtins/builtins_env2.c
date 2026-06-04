/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 17:49:34 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 17:59:20 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export_line(char *entry)
{
	char	*equal;
	char	*name;

	equal = ft_strchr(entry, '=');
	if (equal)
	{
		name = ft_substr(entry, 0, equal - entry);
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
		ft_putendl_fd(entry, STDOUT_FILENO);
	}
}

void	print_export(char **env)
{
	char	**sorted;
	int		len;
	int		j;

	len = 0;
	while (env && env[len])
		len++;
	sorted = malloc(sizeof(char *) * (len + 1));
	if (!sorted)
		return ;
	j = 0;
	while (j < len)
	{
		sorted[j] = env[j];
		j++;
	}
	sorted[len] = NULL;
	sort_env(sorted, len);
	j = 0;
	while (sorted[j])
		print_export_line(sorted[j++]);
	free(sorted);
}
