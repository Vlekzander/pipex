/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 22:23:11 by apierret          #+#    #+#             */
/*   Updated: 2024/11/27 22:36:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex_data	data;
	int				return_value;

	if (argc < 4)
		return (ft_putstr_fd("[USAGE] pipex <input> <cmd1> .. <cmdN> <output>\n"
				, 2), 1);
	data.input = open_file(argv[1], 0);
	data.output = open_file(argv[argc -1], 1);
	argv[argc -1] = NULL;
	data.commands = argv +2;
	data.envp = envp;
	return_value = execute_pipex(data);
	return (close_fd(data.input), close_fd(data.output), return_value);
}
