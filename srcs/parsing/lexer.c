/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafagg <rafagg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:54:02 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/05/29 20:10:53 by rafagg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*read_token_value(char *line, int *i, t_quote_type *quoted)
{
	char			*result;
	char			*part;
	t_quote_type	q;

	result = ft_strdup("");
	while (line[*i] && !is_separator(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
		{
			part = read_quoted(line, i, &q);
			if (!part)
				return (free(result), NULL);
			*quoted = q;
		}
		else
		{
			part = read_word(line, i);
			if (!part)
				return (free(result), NULL);
		}
		result = append_part(result, part);
		if (!result)
			return (NULL);
	}
	return (result);
}

static t_token	*handle_operator(char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (new_token(ft_strdup("|"), TOKEN_PIPE, QUOTE_NONE));
	}
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		(*i) += 2;
		return (new_token(ft_strdup("<<"), TOKEN_HEREDOC, QUOTE_NONE));
	}
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		(*i) += 2;
		return (new_token(ft_strdup(">>"), TOKEN_REDIR_APPEND, QUOTE_NONE));
	}
	if (line[*i] == '<')
	{
		(*i)++;
		return (new_token(ft_strdup("<"), TOKEN_REDIR_IN, QUOTE_NONE));
	}
	(*i)++;
	return (new_token(ft_strdup(">"), TOKEN_REDIR_OUT, QUOTE_NONE));
}

static t_token	*handle_word(char *line, int *i, t_quote_type *quoted)
{
	char	*value;

	value = read_token_value(line, i, quoted);
	if (!value)
		return (NULL);
	return (new_token(value, TOKEN_WORD, *quoted));
}

t_token	*tokenize(char *line, t_shell *shell)
{
	t_token			*tokens;
	t_token			*new;
	t_quote_type	quoted;
	int				i;

	(void)shell;
	tokens = NULL;
	i = 0;
	while (line[i])
	{
		i = skip_spaces(line, i);
		if (!line[i])
			break ;
		quoted = QUOTE_NONE;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			new = handle_operator(line, &i);
		else
			new = handle_word(line, &i, &quoted);
		if (!new)
			return (free_tokens(tokens), NULL);
		token_addback(&tokens, new);
	}
	return (tokens);
}
