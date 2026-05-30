/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:56:54 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 20:10:33 by rafagg           ###   ########.fr       */
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

char	*find_executable(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
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
