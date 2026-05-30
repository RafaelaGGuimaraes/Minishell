/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:53:58 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 20:10:50 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	cmd_addback(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	add_arg(t_cmd *cmd, char *value)
{
	char	**new;
	int		count;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new = malloc(sizeof(char *) * (count + 2));
	if (!new)
		return (-1);
	i = 0;
	while (i < count)
	{
		new[i] = cmd->args[i];
		i++;
	}
	new[i++] = value;
	new[i] = NULL;
	free(cmd->args);
	cmd->args = new;
	return (0);
}

void	redir_addback(t_redir **lst, t_token_type type, char *file, int hq)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->file = file;
	new->heredoc_quoted = hq;
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
