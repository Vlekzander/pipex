/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:04:23 by apierret          #+#    #+#             */
/*   Updated: 2024/11/18 18:43:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "libft.h"
#include "utils.h"

static char	*locate_command(char *cmd, char **envp)
{
	char	*fcmd;
	char	*temp;
	char	**paths;
	size_t	i;

	i = 0;
	while (envp[i] != NULL && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	paths = ft_split(envp[i] +5, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		temp = ft_strjoin(paths[i], "/");
		fcmd = ft_strjoin(temp, cmd);
		free(temp);
		if (access(fcmd, X_OK) == 0)
			return (free_ddarray(paths), fcmd);
		free(fcmd);
		i++;
	}
	return (free_ddarray(paths), NULL);
}

static int	exec_commands(int input, char *f_output, char **cmds, char **envp)
{
	pid_t	pid;
	char	**args;
	char	*cmd;
	int		fds[2];
	int		status;

	if (f_output == NULL || cmds == NULL || envp == NULL)
		return (1);
	args = NULL;
	cmd = NULL;
	pid = 0;
	while (*cmds != NULL)
	{
		args = ft_split_args(*cmds);
		if (args == NULL)
			return (1);
		cmd = locate_command(args[0], envp);
		if (cmd == NULL)
		{
			ft_putstr_fd(*cmds, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		if (*(cmds + 1) == NULL)
		{
			if (cmd == NULL)
				return (perror(f_output), free_ddarray(args), free(cmd), close(fds[0]), close(fds[1]), close(input), 127);
			close(fds[1]);
			fds[1] = open(f_output, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fds[1] == -1)
				return (perror(f_output), free_ddarray(args), free(cmd), close(fds[0]), close(fds[1]), close(input), 1);
		}
		else
		{
			if (pipe(fds) == -1)
				return (perror("pipes"), free_ddarray(args), free(cmd), close(input), 1);
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork"), free_ddarray(args), free(cmd), close(fds[0]), close(fds[1]), close(input), 1);
		if (pid == 0)
		{
			dup2(input, STDIN_FILENO);
			dup2(fds[1], STDOUT_FILENO);
			close(input);
			close(fds[0]);
			close(fds[1]);
			execve(cmd, args, envp);
			exit(EXIT_FAILURE);
		}
		close(input);
		close(fds[1]);
		input = fds[0];
		free_ddarray(args);
		free(cmd);
		cmds++;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	close(fds[0]);
	close(fds[1]);
	close(input);
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	char	*output;
	int		fds[2];

	if (argc != 5)
		return (ft_putstr_fd("[USAGE] pipex <input> <cmd1> <cmd2> <output>\n",
				2), 1);
	input = open(argv[1], O_RDONLY);
	if (input == -1)
	{
		perror(argv[1]);
		if (pipe(fds) == -1)
			return (perror("pipes"), 1);
		close(fds[1]);
		input = fds[0];
	}
	output = argv[argc -1];
	argv[argc -1] = NULL;
	return (exec_commands(input, output, argv +2, envp));
}
