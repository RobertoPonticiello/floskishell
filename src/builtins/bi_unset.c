/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <rpontici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 10:00:00 by rpontici          #+#    #+#             */
/*   Updated: 2026/05/22 10:00:00 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_unset(char **argv)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!ident_ok(argv[i]))
		{
			write(2, "minishell: unset: `", 19);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "': not a valid identifier\n", 26);
			status = 1;
		}
		else
			env_unset(argv[i]);
		i++;
	}
	return (status);
}
