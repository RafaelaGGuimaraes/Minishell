/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:46:45 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:18:45 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_part(char *result, char *part)
{
	char	*tmp;

	tmp = ft_strjoin(result, part);
	free(result);
	free(part);
	return (tmp);
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	shell_error(char *cmd, char *msg, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
	(void)code;
}

void	cleanup_shell(t_shell *shell)
{
	free_tokens(shell->tokens);
	free_cmds(shell->cmds);
	free_array(shell->env);
	shell->tokens = NULL;
	shell->cmds = NULL;
	shell->env = NULL;
}
