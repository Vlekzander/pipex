/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:45:05 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 20:28:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "pipex.h"

t_command	*prepare_command(t_error_code *error, char *cmd, char **envp)
{
	t_command	*command;

	command = (t_command *) malloc(sizeof(t_command));
	if (command == NULL)
		return (NULL);
	command->envp = envp;
	command->args = ft_split(cmd, ' ');
	command->executable_path = find_executable(error, command->args[0], envp);
	if (command->executable_path == NULL)
		return (free_command(command), NULL);
	return (command);
}

void	free_command(t_command *command)
{
	if (command == NULL)
		return ;
	free(command->executable_path);
	free_ddarray(command->args);
	free(command);
}

t_fds	*init_fds(int input_fd, int output_fd)
{
	t_fds	*fds;

	fds = (t_fds *) ft_calloc(1, sizeof(t_fds));
	if (fds == NULL)
		return (NULL);
	fds->pipe[0] = -1;
	fds->pipe[1] = -1;
	fds->input_fd = input_fd;
	fds->output_fd = output_fd;
	return (fds);
}

void	free_fds(t_fds	*fds)
{
	if (fds == NULL)
		return ;
	close_fds(2, fds->pipe);
	close_fd(fds->input_fd);
	close_fd(fds->output_fd);
	free(fds);
}

void	free_ddarray(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
