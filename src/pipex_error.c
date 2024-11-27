/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:21:10 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 19:22:16 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_error_message(t_error_code error)
{
	if (error == PIPEX_USAGE)
		return ("[USAGE] pipex <input> <cmd1> <cmd2> <output>");
	if (error == FILE_NOT_FOUND)
		return ("No such file or directory");
	if (error == COMMAND_NOT_FOUND)
		return ("Command not found");
	if (error == PERMISSION_DENIED)
		return ("Permission denied");
	if (error == PIPE_FAILED)
		return ("Pipe failed");
	if (error == FORK_FAILED)
		return ("Fork failed");
	return ("Error not found");
}

void	print_error(char *source, t_error_code error)
{
	if (source != NULL)
	{
		ft_putstr_fd(source, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(get_error_message(error), 2);
	ft_putstr_fd("\n", 2);
}
