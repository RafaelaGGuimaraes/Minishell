/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:46:28 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:18:36 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**copy_env(char **env)
{
	char	**copy;
	int		i;

	i = 0;
	while (env[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			free_array(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*get_env(char *name, t_shell *shell)
{
	int		i;
	int		len;

	i = find_env_index(shell->env, name);
	if (i == -1)
		return (NULL);
	len = ft_strlen(name);
	return (shell->env[i] + len + 1);
}

static int	grow_env(t_shell *shell, char *entry)
{
	char	**new_env;
	int		count;
	int		i;

	count = 0;
	while (shell->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (free(entry), -1);
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i++] = entry;
	new_env[i] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

int	set_env(char *name, char *value, t_shell *shell)
{
	char	*entry;
	int		i;

	entry = build_env_entry(name, value);
	if (!entry)
		return (-1);
	i = find_env_index(shell->env, name);
	if (i != -1)
	{
		free(shell->env[i]);
		shell->env[i] = entry;
		return (0);
	}
	return (grow_env(shell, entry));
}

int	unset_env(char *name, t_shell *shell)
{
	int	i;

	i = find_env_index(shell->env, name);
	if (i == -1)
		return (0);
	free(shell->env[i]);
	while (shell->env[i + 1])
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	shell->env[i] = NULL;
	return (0);
}
