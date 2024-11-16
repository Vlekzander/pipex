/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:04:23 by apierret          #+#    #+#             */
/*   Updated: 2024/11/16 22:18:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
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

static int	exec_command(char *cmd, int input, int output, char **envp)
{
	pid_t	pid;
	char	**args;
	char	*fcmd;
	int		status;

	args = ft_split_args(cmd);
	fcmd = locate_command(args[0], envp);
	if (fcmd == NULL)
		return (free_ddarray(args), 127);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		dup2(input, STDIN_FILENO);
		dup2(output, STDOUT_FILENO);
		execve(fcmd, args, envp);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	free(fcmd);
	free_ddarray(args);
	return (status);
}

static void	ft_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	int		input_fd;
	int		output_fd;
	int		result;

	if (argc != 5)
		return (ft_putstr_fd("[USAGE] pipex <input> <cmd1> <cmd2> <output>\n",
				2), 1);
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1)
		perror(argv[1]);
	if (pipe(pipe_fds) == -1)
		return (perror("pipex"), close(input_fd), 1);
	if (input_fd != -1)
		if (exec_command(argv[2], input_fd, pipe_fds[1], envp) == 127)
			ft_cmd_not_found(argv[2]);
	close(pipe_fds[1]);
	close(input_fd);
	output_fd = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (output_fd == -1)
		return (perror(argv[4]), close(pipe_fds[0]), 1);
	result = exec_command(argv[3], pipe_fds[0], output_fd, envp);
	if (result == 127)
		ft_cmd_not_found(argv[3]);
	return (close(pipe_fds[0]), close(output_fd), result);
}
