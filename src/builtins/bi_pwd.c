/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_pwd(char **argv)
{
	char	cwd[MSH_PATHMAX];

	(void)argv;
	if (getcwd(cwd, MSH_PATHMAX))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	ft_putendl_fd("minishell: pwd: error retrieving current directory", 2);
	return (1);
}
