/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:54:08 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:18:06 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>' || c == '\0');
}

int	skip_spaces(char *line, int i)
{
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (i);
}

char	*read_word(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && !is_separator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	*read_quoted(char *line, int *i, t_quote_type *quoted)
{
	char	quote_char;
	char	*word;
	int		start;

	quote_char = line[*i];
	if (quote_char == '\'')
		*quoted = QUOTE_SINGLE;
	else
		*quoted = QUOTE_DOUBLE;
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (!line[*i])
		return (NULL);
	word = ft_substr(line, start, *i - start);
	(*i)++;
	return (word);
}

t_token	*new_token(char *value, t_token_type type, t_quote_type quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}
