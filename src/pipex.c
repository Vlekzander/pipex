/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:36:33 by apierret          #+#    #+#             */
/*   Updated: 2024/11/26 14:07:29 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "libft.h"
#include "pipex.h"

static int	get_status(pid_t pid)
{
	int	status;

	if (pid != -1)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}
	else
		status = 127;
	return (status);
}

static pid_t	execute_command(int input, int *pipe_fds, t_command *cmd)
{
	pid_t	pid;

	if (cmd == NULL)
		return (close(input), close(pipe_fds[1]), -1);
	pid = fork();
	if (pid == -1)
		return (print_error("PIPEX", FORK_FAILED), -1);
	if (pid == 0)
	{
		dup2(input, STDIN_FILENO);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(input);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		execve(cmd->executable_path, cmd->args, cmd->envp);
		exit(EXIT_FAILURE);
	}
	close(input);
	close(pipe_fds[1]);
	return (pid);
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

int	execute_pipex(int input, int f_output, char **cmds, char **envp)
{
	t_list		*pids;
	int			pipe_fds[2];
	t_command	*cmd;

	pids = NULL;
	while (*cmds != NULL)
	{
		if (*(cmds +1) == NULL)
		{
			close(pipe_fds[1]);
			pipe_fds[1] = f_output;
			if (f_output == -1)
				return (1);
		}
		else if (pipe(pipe_fds) == -1)
			return (print_error("PIPEX", PIPE_FAILED), 1);
		cmd = prepare_command(*cmds, envp);
		store_pid(&pids, execute_command(input, pipe_fds, cmd));
		input = pipe_fds[0];
		free_command(cmd);
		cmds++;
	}
	while (pids != NULL && pids->next != NULL)
		waitpid(retrieve_next_pid(&pids), NULL, 0);
	return (get_status(retrieve_next_pid(&pids)));
}
