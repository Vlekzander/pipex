/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:04:23 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 19:22:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex_data	data;
	int				return_value;

	if (argc != 5)
		return (print_error(NULL, PIPEX_USAGE), 1);
	data.input = open_file(argv[1], 0);
	data.output = open_file(argv[argc -1], 1);
	argv[argc -1] = NULL;
	data.commands = argv +2;
	data.envp = envp;
	return_value = execute_pipex(data);
	return (close_fd(data.input), close_fd(data.output), return_value);
}
