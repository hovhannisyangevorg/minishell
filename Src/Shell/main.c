/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gevorg <gevorg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:40:10 by gehovhan          #+#    #+#             */
/*   Updated: 2024/02/08 23:15:49 by gevorg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <termio.h>

// size_t numofrec;

// void ft(t_ast_node *tree)
// {
// 	if (!tree)
// 		return ;
// 	printf("-------------------->%s\n", ft_get_type( tree->token_type));
// 	ft(tree->left);
// 	ft(tree->right);
// }


// 0924

void print_env_args(char** env)
{
	int i = -1;
	printf("-------------args----------\n");
	while (env && env[++i])
	{
		printf("%s\n", env[i]);
	}
	printf("-------------end args----------\n");
	
}

void	ft_get_pid(t_container cont, t_hash_table *env)
{
	(void)cont;
	pid_t pid;

    pid = fork();

    if (pid < 0)
        return ;
    else if (pid == 0)
		exit(EXIT_SUCCESS);
    else
	{
		wait(NULL);
		char* p_id = ft_itoa(pid - 1);
		ft_set_env(env, "$", p_id, 1);
		free(p_id);
    }
}

void	ft_program(char **env)
{
	t_container		container;
	char 			*line;
	t_list_token	*list;

	// signal(SIGINT, sig_handler_c);
	// rl_catch_signals = 0;
	t_hash_table *table = ft_create_env(env);
	t_hash_table* funcs = ft_create_func_table();
	// test for function table
	ft_get_pid(container, table);
	// char** env1 = ft_convert_env_to_args(table);
	// print_env_args(env1);
	while (1)
	{
		line = ft_get_line();
		if (!line)
			return ;
		if (!(*line))
		{
			free(line);
			continue;
		}
		ft_balanced(line);
        list = ft_tokenize(line, SEPARATORS);
		container = ft_parser(list);
		if (container.exit_status == SUCCESS_CODE)
			ft_executor(table, funcs, container);
		ft_free_list(list);
		free(line);
    }
    clear_history();
	ft_free_hash_table(table);
	ft_clear_hash_table(table);
	ft_free_hash_table(funcs);
	ft_clear_hash_table(funcs);
}

int main(int, char **, char **env)
{
	ft_program(env);
	return (0);	
}




































// int main()
// {
// 	int fd = open("./valgrind.out", O_WRONLY);

// 	dup2(fd, 1);

// 	// system("ls");
// 	// printf("fileeeeee\n");

	
// }



































































































// (void)env;

	// t_list_token *list = ft_init_list();

	// ft_push_back(list, TEXT, "ls");
	// ft_push_back(list, PIPE, "|");
	// ft_push_back(list, TEXT, "pwd");
	// ft_push_back(list, PIPE, "|");
	// ft_push_back(list, TEXT, "grep");
	// ft_push_back(list, WRITE, ">");
	// ft_push_back(list, TEXT, "hello");

	// t_ast_node *tree = ft_shunting_yard_build_ast(list);

	// printf("trrr = %s\n", tree->token);


	// ft_print_list(list->head);
	// char* s = ft_strdup("");
	// ft_ast_print(tree,s, 0, 1);
	// free(s);
	
	
	// ft_free_list(list);

	// free(tree->token);
	// free(tree->subshell);
	// free(tree);
	
	// free(tree->left)
	// ft_free_tree(tree);


















































































































// #include <stdio.h>

// int main() {
//     FILE *file = fopen("example.txt", "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         return 1;
//     }

//     int c;

//     // Loop to read characters until EOF is encountered
//     while ((c = fgetc(file)) != EOF) {
//         // Process the character, for example, print it
//         putchar(c);
//     }

//     // Check for EOF using feof
//     if (feof(file)) {
//         printf("\nEnd of file reached.\n");
//     } else {
//         printf("\nError reading file.\n");
//     }

//     // Close the file
//     fclose(file);

//     return 0;
// }



// #include <sys/mman.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/mman.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <unistd.h>

// int main()
// {
//     int fd = open("example.txt", O_RDONLY);
//     off_t file_size = lseek(fd, 0, SEEK_END);

//     // Map the file into memory
//     void *mapped_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

// 	if (mapped_data == MAP_FAILED) {
// 		perror("mmap");
// 		return (1);
//     }

//     // Use the mapped data...
//     printf("Contents of the mapped file:\n%s\n", (char*)mapped_data);

// 	(*(char*)mapped_data) = 'A';
//     // Unmap the file from memory
//     if (munmap(mapped_data, file_size) == -1) {
//         perror("munmap");
//         return 1;
//     }

//     // Close the file descriptor
//     close(fd);

//     return 0;
// }






















// #include <unistd.h>
// #include <stdio.h>
// #include <fcntl.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <errno.h>

// void	ft_can_reade(int fd)
// {
	
// }

// void	ft_can_write(int fd)
// {
	
// }

// ft_can_write_and_read(int fd)
// {
	
// }

// void ft_punic(const char *errorMessig)
// {
// 	if (errno == EACCES)
// 		perror("Permission denied Expands to: 13");
// 	else
// 		perror(errorMessig);
//     exit(EXIT_FAILURE);
// }

// int main(int ac, char **av)
// {
// 	int fd;
// 	int file_flag;
	
// 	fd = open(av[1], O_RDWR | O_TRUNC | O_CREAT, 0664);
// 	if (fd == -1)
// 		ft_punic("Error: Opening file Flags.");
// 	file_flag = fcntl(fd, F_GETFL);
// 	if (file_flag == -1)
// 		ft_panic("Error: Getting file flags");

// 	if (file_flag & O_RDONLY)
// 	{
// 		ft_can_reade(fd);
// 	}
// 	else if (file_flag & O_WRONLY)
// 	{
// 		ft_can_write(fd);
// 	}
// 	else if (file_flag & O_RDWR)
// 	{
// 		ft_can_write_and_read(fd);
// 	}


// 	//printf(EACCES);
	
	
// }

// #include <stdio.h>
// #include <dirent.h>

// int main()
// {
//     DIR *dir = opendir("/home/gevorg/Desktop/Minishell/Src/Executor");

//     if (dir == NULL)
// 	{
//         perror("Error opening directory");
//         return (1);
//     }

//     struct dirent *entry;
// 	// printf("###########################################################\n");
//     while ((entry = readdir(dir)) != NULL) {
//         // printf("%ld\n", entry->d_ino);
// 		printf("Name: %s, Offset: %ld\n", entry->d_name, (long)entry->d_off);
//     }
// 	// printf("###########################################################\n");
// 	// while ((entry = readdir(dir)) != NULL) {
//     //     printf("%s\n", entry->d_name);
//     // }
// 	// printf("###########################################################\n");
// 	// while ((entry = readdir(dir)) != NULL) {
//     //     printf("%ld\n", entry->d_off);
//     // }
// 	// printf("###########################################################\n");
// 	// while ((entry = readdir(dir)) != NULL) {
//     //     printf("%d\n", entry->d_reclen);
//     // }
// 	// printf("###########################################################\n");
// 	// while ((entry = readdir(dir)) != NULL) {
//     //     printf("%d\n", entry->d_type);
//     // }
// 	// printf("###########################################################\n");

//     closedir(dir);
//     return 0;
// }


// #include <stdio.h>
// #include <dirent.h>

// int main() {
//     const char *directory_path = "/path/to/your/directory";

//     DIR *dir = opendir(directory_path);

//     if (dir == NULL) {
//         perror("Error opening directory");
//         return 1;
//     }

//     struct dirent *entry;

//     // Read the first entry
//     entry = readdir(dir);

//     while (entry != NULL) {
//         printf("Name: %s, Offset: %ld\n", entry->d_name, (long)entry->d_off);

//         // Read the next entry using the offset
//         entry = readdir(dir);
//     }

//     closedir(dir);

//     return 0;
// }


































