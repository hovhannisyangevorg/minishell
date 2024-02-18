/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gevorg <gevorg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 20:49:46 by gevorg            #+#    #+#             */
/*   Updated: 2024/02/11 20:49:47 by gevorg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


int is_valid_identifier(char* ident)
{
	size_t i;
	
	i = 0;
	if (ft_isdigit(ident[i]))
		return (0);
	while (ident && ident[i])
	{
		if (!(ft_isalnum(ident[i]) || ident[i] == '_'))
			return (0);
		++i;
	}
	return (1);
}

int ft_handle_export_ident(char *line, t_hash_table *table)
{
	t_hash_data data;
	char* id;
	char* tmp;
	char plus;
	size_t len;

	// printf("id: %s\n", line);
	plus = 0;
	id = ft_strchr(line, '=');
	if (id)
	{
		len = id - line;
		data.data = id + 1;
		if (!*(char *)data.data)
			data.data = "";
	}
	else
	{
		len = ft_strlen(line);
		data.data = NULL;
	}
	tmp = ft_substr(line, 0, len);
	if (*tmp)
	{
		plus = tmp[ft_strlen(tmp) - 1];
		if (plus == '+')
		{
			data.key = ft_substr(tmp, 0, ft_strlen(tmp) - 1);
		}
		else
		{
			data.key = ft_substr(tmp, 0, ft_strlen(tmp));
		}
	}
	else
	{
		data.key = ft_strdup(tmp);
	}
	if (!id)
		data.type = EXPORT;
	else
		data.type = NORMAL;
	if (*data.key && is_valid_identifier(data.key))
	{
		if (plus == '+')
		{
			char* oldval; 
			if(!ft_get_env(table, data.key))
				oldval = ft_strdup("");
			else
				oldval = ft_strdup(ft_get_env(table, data.key));
			oldval = ft_gnl_strjoin(oldval, data.data);
			data.data = oldval;
			ft_set_env(table, data);
			free(oldval);
		}
		else
			ft_set_env(table, data);
	}
	else
	{
		free(tmp);
		free(data.key);
		ft_panic_shell("export: not a valid identifier", "");
		return (EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

int	ft_export(t_command* command, t_symbol_table* table)
{
	t_hash_table_arr	arr;
	size_t				i;
	size_t				status;
	
	status = EXIT_SUCCESS;
	if (command->argument && command->argument->arguments && command->argument->arguments[0] && command->argument->arguments[1])
	{
		i = 1;
		while (command->argument->arguments[i])
		{

			status = status | ft_handle_export_ident(command->argument->arguments[i], table->env);
			++i;
		}
		if (status)
			return (status);
	}
	else
	{
		arr = ft_sort_env(table->env);
		print_env_table(arr);
		free(arr.table);
	}
	return(EXIT_SUCCESS);
}
