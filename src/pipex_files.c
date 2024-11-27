/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:35:18 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 19:22:21 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipex.h"

int	open_file(char *file_path, int write_mode)
{
	if (!write_mode && access(file_path, F_OK) == -1)
		return (print_error(file_path, FILE_NOT_FOUND), -1);
	if ((!write_mode && access(file_path, R_OK) == -1)
		|| (write_mode && access(file_path, F_OK) == 0
			&& access(file_path, W_OK) == -1))
		return (print_error(file_path, PERMISSION_DENIED), -1);
	if (write_mode)
		return (open(file_path, O_WRONLY | O_TRUNC | O_CREAT, 0644));
	return (open(file_path, O_RDONLY));
}

static char	*absolute_path_exec(t_error_code *error, char *command)
{
	if (access(command, F_OK) == -1)
		*error = FILE_NOT_FOUND;
	else if (access(command, X_OK) == -1)
		*error = PERMISSION_DENIED;
	else
		return (ft_strdup(command));
	return (print_error(command, *error), NULL);
}

char	*find_executable(t_error_code *error, char *command, char **envp)
{
	char	*executable;
	char	*temp_path;
	char	**paths;
	size_t	i;

	if (ft_strchr(command, '/') != NULL)
		return (absolute_path_exec(error, command));
	i = 0;
	while (envp[i] != NULL && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (*error = COMMAND_NOT_FOUND, print_error(command, *error), NULL);
	paths = ft_split(envp[i] +5, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		temp_path = ft_strjoin(paths[i++], "/");
		executable = ft_strjoin(temp_path, command);
		if (access(executable, X_OK) == 0)
			return (free(temp_path), free_ddarray(paths), executable);
		free(temp_path);
		free(executable);
	}
	return (*error = COMMAND_NOT_FOUND, free_ddarray(paths),
		print_error(command, *error), NULL);
}
