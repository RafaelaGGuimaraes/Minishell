/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:53:44 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 22:06:08 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirs(t_redir *lst)
{
	t_redir	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst->next;
		free(lst->file);
		free(lst);
		lst = tmp;
	}
}

void	free_cmds(t_cmd *lst)
{
	t_cmd	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free_array(lst->args);
		free_redirs(lst->redirs);
		free(lst);
		lst = tmp;
	}
}

static int	handle_word(t_cmd *cmd, t_token *tok, t_shell *shell)
{
	char	*value;

	value = expand(tok->value, tok->quoted, shell);
	if (!value)
		return (-1);
	if (*value == '\0' && tok->quoted == QUOTE_NONE)
	{
		free(value);
		return (0);
	}
	if (add_arg(cmd, value) == -1)
		return (free(value), -1);
	return (0);
}

static t_cmd	*handle_operator(t_cmd **cmds, t_cmd *current, t_token **tok)
{
	t_token	*file;

	if ((*tok)->type == TOKEN_PIPE)
	{
		cmd_addback(cmds, current);
		current = new_cmd();
	}
	else
	{
		file = (*tok)->next;
		if (!file)
			return (NULL);
		redir_addback(&current->redirs, (*tok)->type,
			ft_strdup(file->value), file->quoted != QUOTE_NONE);
		*tok = file;
	}
	return (current);
}

t_cmd	*parse(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*current;
	t_token	*tok;

	cmds = NULL;
	current = new_cmd();
	if (!current)
		return (NULL);
	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD && handle_word(current, tok, shell) == -1)
			return (free_cmds(cmds), NULL);
		else if (tok->type != TOKEN_WORD)
		{
			current = handle_operator(&cmds, current, &tok);
			if (!current)
				return (free_cmds(cmds), NULL);
		}
		tok = tok->next;
	}
	cmd_addback(&cmds, current);
	return (cmds);
}
