/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gevorg <gevorg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 20:50:10 by gevorg            #+#    #+#             */
/*   Updated: 2024/02/11 20:50:11 by gevorg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_unset(t_command* command, t_symbol_table* table)
{
	(void)command;
	(void)table;
	ft_pop_entity(table->env, command->argument->arguments[1]);
	return(0);
}
