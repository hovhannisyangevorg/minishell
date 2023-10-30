/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gevorg <gevorg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 22:23:33 by gehovhan          #+#    #+#             */
/*   Updated: 2023/10/30 14:14:39 by gevorg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H


# include "tokenizer_struct.h"
# include "shell.h"

t_stack 	*ft_initialize_stack();
int 		ft_is_empty(t_stack *stack);
void		ft_push_stack(t_stack *stack, char item);
char 		ft_pop_stack(t_stack *stack);
void 		ft_free_stack(t_stack *stack);
void		ft_is_balanc(char *expression);
void		ft_init_list(t_list_token *list);
void		ft_push_back(t_list_token *list, int type, const char *token);
void		ft_push_front(t_list_token *list, int type, const char *token);
void		ft_pop_back(t_list_token *list);
void		ft_pop_front(t_list_token *list);
void		ft_free_list(t_list_token *list);
void		ft_print_list(t_list_token list);
t_list_token *ft_multi_split(char* str, char* delims);
char		*ft_get_line();
void		sig_handler_c(int signum);



void 		ft_ordering(t_list_token *list);



#endif
