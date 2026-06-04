/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:55:21 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 13:17:55 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_char(char *str, char c)
{
	char	*new;
	int		len;
	int		i;

	len = ft_strlen(str);
	new = malloc(len + 2);
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i++] = c;
	new[i] = '\0';
	free(str);
	return (new);
}

char	*expand_exit(t_shell *shell)
{
	return (ft_itoa(shell->last_exit));
}

char	*expand_var(char *str, int *i, t_shell *shell)
{
	int		start;
	char	*var;
	char	*value;

	(*i)++;
	if (!str[*i] || !(ft_isalnum(str[*i]) || str[*i] == '_'))
		return (ft_strdup("$"));
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	if (!var)
		return (NULL);
	value = get_env(var, shell);
	free(var);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*expand_char(char *result, char *str, int *i, t_shell *shell)
{
	char	*tmp;

	if (str[*i] == '$' && str[*i + 1] == '?')
	{
		*i += 2;
		tmp = expand_exit(shell);
	}
	else if (str[*i] == '$')
		tmp = expand_var(str, i, shell);
	else
		return (append_char(result, str[(*i)++]));
	if (!tmp)
		return (free(result), NULL);
	return (append_part(result, tmp));
}

char	*expand(char *str, t_quote_type quoted, t_shell *shell)
{
	char	*result;
	int		i;

	if (quoted == QUOTE_SINGLE)
		return (ft_strdup(str));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		result = expand_char(result, str, &i, shell);
		if (!result)
			return (NULL);
	}
	return (result);
}
