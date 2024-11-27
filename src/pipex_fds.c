/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:59:56 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 22:34:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "pipex.h"


void	close_fd(int fd)
{
	if (fd != -1)
		close(fd);
}

void	close_fds(size_t count, int *fds)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		close_fd(fds[i]);
		i++;
	}
}

void	close_set_fd(int *fd, int new_fd)
{
	close_fd(*fd);
	*fd = new_fd;
}

int	prepare_fds(t_pipex_data data, t_fds *fds, t_list **pids)
{
	if (*(data.commands +1) == NULL)
	{
		if (fds->output_fd == -1)
			return (free_fds(fds), ft_lstclear(pids, free), 0);
		close_set_fd(&fds->pipe[1], fds->output_fd);
	}
	else if (pipe(fds->pipe) == -1)
		return (free_fds(fds), ft_lstclear(pids, free),
			print_error("PIPEX", PIPE_FAILED), 0);
	return (1);
}
