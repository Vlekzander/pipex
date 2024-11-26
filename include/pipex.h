/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:21:58 by apierret          #+#    #+#             */
/*   Updated: 2024/11/26 12:22:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef enum e_error_code
{
	PIPEX_USAGE,
	FILE_NOT_FOUND,
	COMMAND_NOT_FOUND,
	PERMISSION_DENIED,
	PIPE_FAILED,
	FORK_FAILED
}	t_error_code;

typedef struct s_command
{
	char	*executable_path;
	char	**args;
	char	**envp;
}	t_command;

char		*get_error_message(t_error_code error_code);
void		print_error(char *source, t_error_code error_code);
int			open_file(char	*file_path, int write_mode);
char		*find_executable(char *command, char **envp);
t_command	*prepare_command(char *cmd, char **envp);
void		free_ddarray(char **arr);
void		free_command(t_command *command);
int			execute_pipex(int input, int f_output, char **cmds, char **envp);

#endif
