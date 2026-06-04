/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:58:01 by rgomes-g          #+#    #+#             */
/*   Updated: 2026/06/04 15:03:08 by rgomes-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal = 0;

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp);
	shell->cmds = NULL;
	shell->tokens = NULL;
	shell->last_exit = 0;
}

static void	check_signal(t_shell *shell)
{
	if (g_signal == SIGINT)
		shell->last_exit = 130;
	g_signal = 0;
}

static void	process_line(t_shell *shell, char *line)
{
	shell->tokens = tokenize(line, shell);
	if (shell->tokens && syntax_check(shell->tokens) == 0)
	{
		shell->cmds = parse(shell->tokens, shell);
		if (shell->cmds)
		{
			if (expand_heredocs(shell->cmds, shell) == 0)
				shell->last_exit = execute(shell);
		}
		free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	free_tokens(shell->tokens);
	shell->tokens = NULL;
}

static void	run_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		check_signal(shell);
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		process_line(shell, line);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	setup_signals();
	run_loop(&shell);
	free_tokens(shell.tokens);
	free_cmds(shell.cmds);
	free_array(shell.env);
	return (shell.last_exit);
}
