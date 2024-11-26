/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:04:23 by apierret          #+#    #+#             */
/*   Updated: 2024/11/26 11:09:03 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	input_fd;
	int	output_fd;
	int	status;

	if (argc != 5)
		return (print_error(NULL, PIPEX_USAGE), 1);
	input_fd = open_file(argv[1], 0);
	output_fd = open_file(argv[argc -1], 1);
	argv[argc -1] = NULL;
	status = execute_pipex(input_fd, output_fd, argv +2, envp);
	return (close(input_fd), close(output_fd), status);
}
