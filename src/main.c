/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:04:23 by apierret          #+#    #+#             */
/*   Updated: 2024/11/13 19:58:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "libft.h"

static void	free_strings(char **strs)
{
	size_t	i;

	if (strs == NULL)
		return ;
	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static char	*locate_command(char *cmd, char **envp)
{
	char	*fcmd;
	char	*temp;
	char	**paths;
	int		i;

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
			return (free_strings(paths), fcmd);
		free(fcmd);
		i++;
	}
	return (free_strings(paths), NULL);
}

static int	exec_command(char *cmd, int input, int output, char **envp)
{
	pid_t	pid;
	char	**args;
	char	*fcmd;

	args = ft_split(cmd, ' ');
	fcmd = locate_command(args[0], envp);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		dup2(input, STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		execve(fcmd, args, envp);
	}
	else
		wait(NULL);
	free(fcmd);
	free_strings(args);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	int		input_fd;
	int		output_fd;

	if (argc < 5)
		return (ft_putstr_fd("[PIPEX] Not enough arguments.\n", 2), 1);
	if (argc > 5)
		return (ft_putstr_fd("[PIPEX] Too much arguments.\n", 2), 1);
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1)
		return (1);
	output_fd = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (output_fd == -1)
		return (close(input_fd), 1);
	if (pipe(pipe_fds) == -1)
		return (close(input_fd), close(output_fd), 1);
	exec_command(argv[2], input_fd, pipe_fds[1], envp);
	close(pipe_fds[1]);
	close(input_fd);
	exec_command(argv[3], pipe_fds[0], output_fd, envp);
	close(pipe_fds[0]);
	close(output_fd);
	return (0);
}
