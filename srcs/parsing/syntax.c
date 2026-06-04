/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:53:39 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:18:25 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_redir(t_token *tok)
{
	while (tok)
	{
		if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
			|| tok->type == TOKEN_REDIR_APPEND || tok->type == TOKEN_HEREDOC)
		{
			if (!tok->next || tok->next->type != TOKEN_WORD)
				return (-1);
		}
		tok = tok->next;
	}
	return (0);
}

int	check_pipe(t_token *tok)
{
	if (!tok)
		return (0);
	if (tok->type == TOKEN_PIPE)
		return (-1);
	while (tok)
	{
		if (tok->type == TOKEN_PIPE)
		{
			if (!tok->next || tok->next->type == TOKEN_PIPE)
				return (-1);
		}
		tok = tok->next;
	}
	return (0);
}

int	syntax_check(t_token *tokens)
{
	if (!tokens)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		return (-1);
	}
	if (check_pipe(tokens) == -1)
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 39);
		return (-1);
	}
	if (check_redir(tokens) == -1)
	{
		write(STDERR_FILENO, "syntax error near redirection\n", 30);
		return (-1);
	}
	return (0);
}
