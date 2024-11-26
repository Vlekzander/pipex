/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:17:06 by apierret          #+#    #+#             */
/*   Updated: 2024/11/26 12:37:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

char	*get_error_message(t_error_code error_code)
{
	if (error_code == PIPEX_USAGE)
		return ("[USAGE] pipex <input> <cmd1> <cmd2> <output>");
	if (error_code == FILE_NOT_FOUND)
		return ("No such file or directory");
	if (error_code == COMMAND_NOT_FOUND)
		return ("Command not found");
	if (error_code == PERMISSION_DENIED)
		return ("Permission denied");
	if (error_code == PIPE_FAILED)
		return ("Pipe failed");
	if (error_code == FORK_FAILED)
		return ("Fork failed");
	return ("Error not found");
}

void	print_error(char *source, t_error_code error_code)
{
	char	*message;

	if (source != NULL)
	{
		ft_putstr_fd(source, 2);
		ft_putstr_fd(": ", 2);
	}
	message = get_error_message(error_code);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}
