/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:21:38 by apierret          #+#    #+#             */
/*   Updated: 2024/11/26 12:37:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "pipex.h"

t_command	*prepare_command(char *cmd, char **envp)
{
	t_command	*command;

	command = (t_command *) malloc(sizeof(t_command));
	if (command == NULL)
		return (NULL);
	command->envp = envp;
	command->args = ft_split(cmd, ' ');
	command->executable_path = find_executable(command->args[0], envp);
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
