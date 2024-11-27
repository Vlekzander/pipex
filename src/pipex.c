/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:06:47 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 22:34:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pipex.h"

static int	get_return_value(pid_t pid, t_error_code error)
{
	int	status;

	status = 0;
	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		return (status);
	}
	if (error == NONE)
		return (0);
	if (error == PERMISSION_DENIED)
		return (126);
	if (error == FILE_NOT_FOUND || error == COMMAND_NOT_FOUND)
		return (127);
	return (1);
}

static void	store_pid(t_list **list, pid_t pid)
{
	pid_t	*pid_ptr;
	t_list	*node;

	pid_ptr = malloc(sizeof(pid_t));
	if (!pid_ptr)
		return ;
	*pid_ptr = pid;
	node = ft_lstnew(pid_ptr);
	if (node == NULL)
	{
		free(pid_ptr);
		return ;
	}
	ft_lstadd_back(list, node);
}

static pid_t	retrieve_next_pid(t_list **list)
{
	t_list	*old_node;
	pid_t	pid;

	if (*list == NULL)
		return (-1);
	old_node = *list;
	pid = *(pid_t *)(old_node->content);
	*list = old_node->next;
	ft_lstdelone(old_node, free);
	return (pid);
}

static pid_t	execute_command(t_fds *fds, t_command *command)
{
	pid_t	pid;

	if (command == NULL)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (print_error("PIPEX", FORK_FAILED), -1);
	if (pid == 0)
	{
		dup2(fds->input_fd, STDIN_FILENO);
		dup2(fds->pipe[1], STDOUT_FILENO);
		close_fd(fds->input_fd);
		close_fd(fds->output_fd);
		close_fds(2, fds->pipe);
		execve(command->executable_path, command->args, command->envp);
		exit(EXIT_FAILURE);
	}
	return (pid);
}

int	execute_pipex(t_pipex_data	data)
{
	t_list			*pids;
	t_fds			*fds;
	t_command		*command;
	t_error_code	error;

	pids = NULL;
	fds = init_fds(data.input, data.output);
	while (*data.commands != NULL)
	{
		if (!prepare_fds(data, fds, &pids))
			return (1);
		if (fds->input_fd != -1)
		{
			command = prepare_command(&error, *data.commands, data.envp);
			store_pid(&pids, execute_command(fds, command));
			free_command(command);
		}
		close_fd(fds->pipe[1]);
		close_set_fd(&fds->input_fd, fds->pipe[0]);
		data.commands++;
	}
	while (pids != NULL && pids->next != NULL)
		waitpid(retrieve_next_pid(&pids), NULL, 0);
	return (free_fds(fds), get_return_value(retrieve_next_pid(&pids), error));
}
