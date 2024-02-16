/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gevorg <gevorg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:21:12 by gevorg            #+#    #+#             */
/*   Updated: 2024/02/14 23:01:32 by gevorg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

size_t	ft_pipe_count_tree(t_ast_node *tree)
{
	size_t count_left	= 0;
	size_t count_right	= 0;
	if (!tree)
		return (0);
	if (tree->token_type == PIPE)
	{
		count_left	= ft_pipe_count_tree(tree->left);
		count_right	= ft_pipe_count_tree(tree->right);
		return (count_left + count_right + 1);
	}
	count_left 	= ft_pipe_count_tree(tree->left);
	count_right = ft_pipe_count_tree(tree->right);
	return (count_left + count_right);
}

void ft_replace_all(char **line, char *src_env, char *chang_env)
{
	size_t	token_size;
	char	*token_tmp;
	char	*start_offset;
	char	*end_offset;
	
	token_size	= ft_strlen(src_env); 
	start_offset	= ft_strdup(*line);
	token_tmp	= ft_strstr(start_offset, src_env);
	while (token_tmp)
	{
		if (token_size)
		{
			end_offset = ft_strdup(token_tmp + token_size);
			if (!end_offset)
				end_offset = ft_strdup("");
			start_offset = ft_substr(start_offset, 0, token_tmp - start_offset);
			start_offset = ft_gnl_strjoin(start_offset, chang_env);
			start_offset = ft_gnl_strjoin(start_offset, end_offset);
			free(end_offset);
		}
		if (*token_tmp)
			token_tmp = ft_strstr(start_offset, src_env);
		else
			token_tmp = NULL;
	}
	free(*line);
	*line = start_offset;
}

char	*ft_substr_c(char *line, char c)
{
	size_t i = 0;
	size_t start = 0;
	while (line && line[i] && line[i] != c)
		++i;
	start = i;
	if (line[i])
		++i;
	while (line && line[i] && line[i] != c && !ft_isspace(line[i]) && line[i] != '\'' && line[i] != '\"')
		++i;
	return (ft_substr(line, start, i - start));
}

void	ft_hendle_env_variable(char **line, t_hash_table *env)
{
	char	dellim;
	char	*start_env;
	char	*token_env;
	char	*value;

	dellim		= '$';
	token_env	= NULL;
	start_env	= ft_strchr(*line, dellim);
	while(start_env)
	{
		token_env = ft_substr_c(start_env, dellim);
		if (!*token_env)
			value = ft_get_env(env, token_env);
		else
			value = ft_get_env(env, token_env + 1);
		if (value)
			ft_replace_all(line, token_env, value);
		if (*start_env)
			start_env = ft_strchr(start_env + 1, dellim);
		else
			start_env = NULL;
	}
	
}

int	ft_isquot(const char *end_of_file)
{
	if ((end_of_file && *end_of_file) && (ft_strchr((char *)end_of_file, '\"') || ft_strchr((char *)end_of_file, '\'')))
		return (1);
	else
		return (0);
}

size_t	ft_EOF_length(char *end_of_file)
{
	size_t	i;
	size_t	eof_size;
	char	crrent;

	i			= 0;
	eof_size	= 0;

	while (end_of_file && end_of_file[i])
	{
		if (end_of_file[i] == '\'' || end_of_file[i] == '\"')
		{
			crrent =  end_of_file[i++];
			while (end_of_file[i] != crrent)
			{
				++eof_size;
				++i;
			}
			if (end_of_file[i] == '\'' || end_of_file[i] == '\"')
				i++;
			continue ;
		}
		++eof_size;
		i++;
	}
	return (eof_size);
}

char *ft_ignor_EOF_quots(char *end_of_file)
{
	size_t	i;
	size_t  j;
	size_t	eof_size;
	char	crrent;
	char 	*eof_key;

	i			= 0;
	j			= 0;
	eof_size	= 0;
	eof_size	= ft_EOF_length(end_of_file);
	eof_key		= (char *)malloc(sizeof(char) * (eof_size + 1));
	if (!eof_key)
		return (NULL);

	while (end_of_file && end_of_file[i])
	{
		if (end_of_file[i] == '\'' || end_of_file[i] == '\"')
		{
			crrent =  end_of_file[i++];
			while (end_of_file[i] != crrent)
			{
				eof_key[j++] = end_of_file[i++];
			}
			if (end_of_file[i] == '\'' || end_of_file[i] == '\"')
				i++;
			continue ;
		}
		eof_key[j++] = end_of_file[i];
		i++;
	}
	eof_key[j++] = '\0';
	return (eof_key);
}

char *ft_generate_filename()
{
	static size_t	i;
	char 			*id;
	char			*filename;

	filename	= ft_strdup(HEREDOC_DIR"/.heredoc");
	if (i == 0)
	{
		i++;
		return (filename);
	}
	id			= ft_itoa(i++);
	filename	= ft_gnl_strjoin(filename, id);
	free(id);
	return (filename);
}
void ft_heredoc_handler_child(int);
void ft_heredoc_handler(int num)
{
	g_global_state.heredoc_signal = num;
	g_global_state.minishell_signal = SIGNORMAL;
	close(STDIN_FILENO);
}

int open_heredoc(t_redirect	*red, t_hash_table *env, t_io io)
{
	(void)io;
	int 	here_fd;
	int		flag;
	char	*line;
	char	*filename;
	char	*end_of_file;
	
	flag 		= 0;
	filename	= ft_generate_filename();
	here_fd 	= open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	end_of_file = ft_ignor_EOF_quots(red->argument);

	signal(SIGINT, ft_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	g_global_state.minishell_signal = SIGHEREDOC;
	if (ft_isquot(red->argument))
		flag = 1;
	// tcsetattr(STDIN_FILENO, TCSANOW, &g_global_state.orig_termios);
	int stdio 	= dup(STDIN_FILENO);
	
	while (1)
	{
		// rl_catch_signals = 0;
		// rl_already_prompted = 1;
		line = readline(">");
		if (!line || !ft_strcmp(line, end_of_file))
		{
			break ;
		}
		if (!flag)
			ft_hendle_env_variable(&line, env);
		ft_putstr_fd(line, here_fd);
		ft_putstr_fd("\n", here_fd);
		free(line);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &g_global_state.orig_termios);
	close(here_fd);
	if (g_global_state.heredoc_signal == -1)
	{
		here_fd 	= open(filename,  O_RDONLY, 0666);
	}
	else
		here_fd = -1;
	dup2(stdio, STDIN_FILENO);
	close(stdio);
	free(end_of_file);
	unlink(filename);
	free(filename);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	return (here_fd);
}

// TODO: file prmition 
void	ft_open_type(t_redirect *redirect, t_command *cmd, t_vector *fd_vector, t_hash_table *env, t_io io)
{
	int fd;

	fd = -1;
	if (redirect->base.token_type == WRITE)
	{
		fd = open(redirect->argument, O_TRUNC | O_WRONLY | O_CREAT, 0664);
		if (cmd->io.output < fd)
			cmd->io.output = fd;
		else
			ft_push_arrey(fd_vector, fd);
	}
	else if (redirect->base.token_type == READ)
	{
		fd = open(redirect->argument,  O_RDONLY);
		if (cmd->io.input < fd)
			cmd->io.input = fd;
		else
			ft_push_arrey(fd_vector, fd);
	}
	else if (redirect->base.token_type == APPEND)
	{
		fd = open(redirect->argument, O_APPEND | O_WRONLY | O_CREAT, 0664);
		if (cmd->io.output < fd)
			cmd->io.output = fd;
		else
			ft_push_arrey(fd_vector, fd);
	}
	else if (redirect->base.token_type == HEREDOC)
	{
		fd = open_heredoc(redirect, env, io);
		if (cmd->io.input < fd)
			cmd->io.input = fd;
		else
			ft_push_arrey(fd_vector, fd);
	}
}

void ft_close_fd(t_vector *fd_vector)
{
	size_t i;

	i = 0;
	while (i < fd_vector->size)
		close(fd_vector->arr[i++]);
	free(fd_vector->arr);
}

void	ft_open_all_fd(t_ast_node *ast_node, t_hash_table *env, t_io io)
{
	t_vector fd_vector;
	if (!ast_node)
		return ;
	ft_init_arrey(&fd_vector, 0);
	if (ast_node->token_type == COMMAND)
		ft_open_file(ft_ast_to_command(ast_node), env, &fd_vector, io);
	ft_open_all_fd(ast_node->left, env, io);
	ft_open_all_fd(ast_node->right, env, io);
}

void ft_child_sigint2(int num)
{
	(void)num;
	rl_replace_line("", 0);
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	tcsetattr(STDIN_FILENO, TCSANOW, &g_global_state.orig_termios);

}

void ft_executor(t_symbol_table* table, t_container cont)
{
	t_vector	pipe_fd;
	size_t 		pipe_count;
	size_t		pipe_iter;
	int			status;



	status = 0;
	int isSignaled = 0;

	if (cont.exec_type == LIST)
	{
		pipe_count = 0;
		ft_open_all_fd(ft_command_to_ast_node(cont.command), table->env, cont.fd);
		if (g_global_state.heredoc_signal == -1)
			status = ft_executor_with_list(cont.fd, cont.command, table);
		else
			status = 128 + g_global_state.heredoc_signal;
		
	}
	else
	{
		pipe_count = ft_pipe_count_tree(cont.tree->ast_node);
		ft_open_all_fd(cont.tree->ast_node, table->env, cont.fd);
		pipe_fd = ft_open_pipe_fd(pipe_count);
		pipe_iter = 0;
		if (g_global_state.heredoc_signal == -1)
		{
			ft_execute_part(cont.fd, cont.tree->ast_node, table, &pipe_fd, &pipe_iter);
			
			for (size_t i = 0; i < pipe_fd.size; i++)
			{
				close(pipe_fd.arr[i]);
			}
			for (size_t i = 0; i < pipe_count + 1; i++)
			{
				wait(&status);
				if (!isSignaled)
				{
					if (WIFSIGNALED(status))
					{
						if (WTERMSIG(status) == SIGPIPE)
							status = 0;
						else
							status = 128 + WTERMSIG(status);
						rl_catch_signals = 0;
						g_global_state.minishell_signal = SIGCHILD;
						ft_child_sigint2(SIGNORMAL);
						isSignaled = 1;
					}
					else if (WIFEXITED(status))
						status = WEXITSTATUS(status);
				}
			}
		}
		else
			status = 128 + g_global_state.heredoc_signal;
		
		ft_free_tree(cont.tree->ast_node);	
		free(cont.tree);
		// char *leak = ft_strdup("");
		// ft_ast_print(cont.tree->ast_node, leak, 0, 1);
		// free(leak);
		
	}
	g_global_state.heredoc_signal = -1;
	cont.exit_status = status;
	char* st_status = ft_itoa(cont.exit_status);
	ft_set_env(table->env, (t_hash_data){"?", st_status, HIDDEN});
	free(st_status);
	(void)pipe_count;
}
