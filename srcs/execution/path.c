/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:56:54 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 22:03:33 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*try_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

static char	**get_paths(t_shell *shell)
{
	char	*path;

	path = get_env("PATH", shell);
	if (!path)
		return (NULL);
	return (ft_split(path, ':'));
}

static char	*find_in_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full;
	int		i;

	paths = get_paths(shell);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full = try_path(paths[i], cmd);
		if (full)
			return (free_array(paths), full);
		i++;
	}
	free_array(paths);
	return (NULL);
}

char	*find_executable(char *cmd, t_shell *shell)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (NULL);
		if (access(cmd, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": Permission denied", 2);
			exit(126);
		}
		return (ft_strdup(cmd));
	}
	return (find_in_path(cmd, shell));
}

