/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:20:25 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 19:21:53 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft.h"

typedef enum e_error_code
{
	NONE,
	PIPEX_USAGE,
	FILE_NOT_FOUND,
	COMMAND_NOT_FOUND,
	PERMISSION_DENIED,
	PIPE_FAILED,
	FORK_FAILED
}	t_error_code;

typedef struct s_pipex_data
{
	int		input;
	int		output;
	char	**commands;
	char	**envp;
}	t_pipex_data;

typedef struct s_command
{
	char	*executable_path;
	char	**args;
	char	**envp;
}	t_command;

typedef struct s_fds
{
	int	pipe[2];
	int	input_fd;
	int	output_fd;
}	t_fds;

char		*get_error_message(t_error_code error);
void		print_error(char *source, t_error_code error);
int			open_file(char *file_path, int write_mode);
char		*find_executable(t_error_code *error, char *command, char **envp);
int			execute_pipex(t_pipex_data	data);
t_command	*prepare_command(t_error_code *error, char *cmd, char **envp);
void		free_command(t_command *command);
void		free_ddarray(char **arr);
t_fds		*init_fds(int input_fd, int output_fd);
void		free_fds(t_fds	*fds);
void		close_fd(int fd);
void		close_fds(size_t count, int *fds);
void		close_set_fd(int *fd, int new_fd);
int			prepare_fds(t_pipex_data data, t_fds *fds, t_list **pids);
#endif
