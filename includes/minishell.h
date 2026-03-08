/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 01:31:35 by riramana          #+#    #+#             */
/*   Updated: 2026/01/18 09:02:59 by andyandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define PROMPT "minishell$>"

extern int		g_signal;

typedef struct s_exec_data
{
	char		***cmds;
	char		**paths;
	char		**envp;
	char		**out_files;
	int			*out_modes;
	int			*heredoc_fds;
	int			*input_fds;
	char		**cmd_strs;
	pid_t		*pids;
	int			n;
	char		*original_cmd;
}				t_exec_data;

typedef struct s_child_exec
{
	char		*exec_path;
	char		**cmd_args;
	char		**envp;
	char		*out_file;
	int			out_mode;
	int			heredoc_fd;
	int			input_fd;
}				t_child_exec;

typedef struct s_shell_data
{
	char		**envp;
	int			last_exit_status;
	char		*current_cmd;
}				t_shell_data;

typedef struct s_exec_params
{
	char		**cmd_args;
	char		**envp;
	char		*out_file;
	int			out_mode;
	int			heredoc_fd;
	int			input_fd;
}				t_exec_params;

typedef struct s_pipeline_data
{
	char		**cmd_strs;
	char		***cmds;
	char		**paths;
	char		**envp;
	int			n;
	int			last_exit_status;
	char		**out_files;
	int			*out_modes;
	int			*heredoc_fds;
	int			*input_fds;
	char		*original_cmd;
}				t_pipeline_data;

typedef struct s_redir_data
{
	char		**out_file;
	int			*out_mode;
	int			should_open;
	int			has_error;
	int			is_syntax_error;
}				t_redir_data;

typedef struct s_builtin_redir
{
	char		*out_file;
	int			out_mode;
	int			syntax_err;
}				t_builtin_redir;

typedef struct s_echo_redir
{
	int			heredoc_fd;
	int			input_fd;
	char		*out_file;
	int			out_mode;
}				t_echo_redir;

typedef struct s_pipeline_redir
{
	char		**out_files;
	int			*out_modes;
	int			*heredoc_fds;
	int			*input_fds;
	int			n;
}				t_pipeline_redir;

void			cleanup_params(t_exec_params *params);
int				external_ret(t_exec_params	*params, int syn_er);
int				syntax_error(int *syntax_error);
void			init_echo_redir(t_echo_redir *redir);
void			assignment_result(t_pipeline_data *p_data,
					t_pipeline_redir *redir);
void			free_stru(t_exec_data *data);
void			init_stru_data(t_pipeline_data *data, int i);
void			msg_exit_builtin(char *str);
void			init_exec_param(t_exec_params *params);
void			close_child_fds(t_exec_data *data, int n);
int				is_out_or_in(char *command, t_exec_params *params,
					int *syntax_err, char **clns);
int				is_in_or_out(char *command, t_exec_params *params,
					int *syntax_err, char **clns);
int				init_two(int *arg1, int *arg2);
void			msg_export(char *name, char *cmd);
void			free_split(char **split);
char			*find_executable_path(char *cmd_name, char **envp);
int				execute_command(char *command, t_shell_data *data);
int				ft_strcmp(char *s1, char *s2);
int				is_numeric_string(char *str);
void			execute_child(int i, int n, int (*pipes)[2], t_exec_data *data);
int				handle_command_not_found(char **cmd_args);
int				handle_cd_command(char **cmd_args, t_shell_data *data);
int				handle_echo_command(char *command, t_shell_data *data);
void			expand_args(char **args, t_shell_data *data);
char			**split_echo_args(char *command);
char			**split_echo_args_with_data(char *command, t_shell_data *data);
int				check_quotes(char *command);
char			*extract_quoted(char *str, int *pos, t_shell_data *data);
char			*extract_unquoted(char *str, int *pos, t_shell_data *data);
char			*extract_arg(char *str, int *pos);
int				is_valid_n_flag(char *arg);
char			*extract_arg_with_data(char *str, int *pos, t_shell_data *data);
int				count_echo_args(char *str);
int				execute_child_process(t_child_exec *data);
int				execute_external_command(t_exec_params *params);
int				handle_parent_process(char **cmd_args);
int				setup_builtin_redirect(char *out_file, int out_mode,
					int *saved_stdout);
char			**prepare_builtin_args(char *command, char **out_file,
					int *out_mode);
char			**prepare_builtin_args_with_data(char *command, char **out_file,
					int *out_mode, t_shell_data *data);
char			**prepare_builtin_args_ext(char *command,
					t_builtin_redir *redir, t_shell_data *data);
int				handle_builtin_command(char *command, t_shell_data *data);
int				handle_fork_error(char **cmd_args, char *exec_path);
int				is_whitespace(char *str);
int				execute_pipeline(t_pipeline_data *p_data);
void			print_heredoc_warning(char *delimiter);
int				execute_pipeline_wrapper(char *command, t_shell_data *data);
int				count_pipes(char *command);
char			**split_pipeline(char *command);
void			free_pipeline_data(char ***cmds, char **paths, int n);
void			free_redir_data(t_pipeline_redir *redir);
int				handle_dot_builtin(char **cmd_args);
void			close_parent_pipe_ends(int (*pipes)[2], int n);
char			*trim_whitespace(char *str);
int				parse_args_with_data(char *command, char **args,
					t_shell_data *data);
int				handle_external_cmd(char *command, t_shell_data *data);
int				prepare_pipeline_data(t_pipeline_data *data);
void			execute_child(int i, int n, int (*pipes)[2], t_exec_data *data);
void			close_parent_pipes(int i, int (*pipes)[2]);
int				create_pipes(int (*pipes)[2], int n);
int				init_pipeline_memory(int (**pipes)[2], pid_t **pids, int n);
void			cleanup_fork_error(int (*pipes)[2], pid_t *pids, int n);
int				wait_all_children(pid_t *pids, int n);
char			*build_filename_parts(char *line, int *pos);
char			*extract_quoted_filename(char *str, int *pos);
char			*extract_unquoted_filename(char *str, int *pos);
int				handle_pwd_command(void);
int				handle_env_command(char **envp);
int				handle_neofetch(void);
int				prepare_cmd_data(char **cmd_strs, char ***cmds, char **paths,
					int n);
int				validate_pipeline_syntax(char **cmd_strs, int n);
int				check_overflow(char *str, int sign);
int				handle_echo_cmd(char *command, t_shell_data *data);
int				allocate_pipeline_arrays(t_pipeline_data *data);
int				handle_pipeline_command(char *command, t_shell_data *data);
int				setup_pipeline(char **cmd_strs, char ****cmds, char ***paths,
					t_pipeline_data *p_data);
int				has_pipe(char *command);
int				find_next_redir(char *cleaned, int *i);
int				init_exec_data(t_exec_data *data, t_pipeline_data *p_data);
int				validate_pipe_syntax(char *command);
int				has_redir_before_pipe(char *cmd, int sq, int dq);
int				is_echo_command(char *command);
int				is_builtin(char *cmd);
int				parse_args(char *command, char **args);
int				fork_and_execute(int i, int n, int (*pipes)[2],
					t_exec_data *data);
void			setup_child_input(int i, int (*pipes)[2], t_exec_data *data);
int				handle_pwd_command(void);
void			close_echo_fds(t_echo_redir *redir);
int				handle_env_command(char **envp);
void			signal_thing(int sig);
void			setup_child_output(int i, int n, int (*pipes)[2],
					t_exec_data *data);
void			close_all_pipes(int (*pipes)[2], int n);
void			signal_exec(int sig);
int				env_count(char **envp);
char			**copy_env(char **envp);
char			*get_env_value(char **envp, char *name);
int				find_env_index(char **envp, char *name);
char			**set_env_value(char **envp, char *name, char *value);
int				process_cmd_helper(char ***cmds, char **paths, int i);
char			**add_env_entry(char **envp, char *new_entry);
char			**remove_env_entry(char **envp, char *name);
void			swap_env_entries(char **a, char **b);
int				is_right_identifier(char *str);
void			print_sorted_env(char **envp);
int				handle_export_command(t_shell_data *data, char **cmd_args);
int				handle_unset_command(t_shell_data *data, char **cmd_args);
int				execute_builtin_in_child(char *builtin_name, char **cmd_args,
					char **envp);
int				execute_builtin_in_pipeline(char *cmd_name, char **cmd_args,
					char **envp);
char			*expand_string(char *str, t_shell_data *data);
char			*expand_command_line(char *line, t_shell_data *data);
int				is_valid_var_char(char c);
char			*parse_output_redirections(char *line, char **out_file,
					int *out_mode);
char			*parse_output_redir_with_err(char *line, char **out_file,
					int *out_mode, int *syntax_err);
int				is_redir_operator(char *str, int pos, int in_sq, int in_dq);
void			handle_filename_cases(char *filename, t_redir_data *data,
					int mode);
int				is_input_redir_operator(char *str, int pos, int in_sq,
					int in_dq);
void			skip_quotes(char *str, int *pos, int *in_sq, int *in_dq);
char			*extract_filename(char *line, int *pos);
char			*build_cleaned_cmd(char *line, int redir_start, int redir_end);
char			*get_next_token(char *line, int pos);
int				handle_redir_error(char **out_file, char *token);
char			*extract_filename_with_error(char *line, int *pos,
					char **out_file);
char			*process_redirection(char *line, int *i, t_redir_data *data);
char			*handle_input_file_redir(char *line, int *i, int *heredoc_fd,
					int *input_fd);
int				has_more_output_redirections(char *line, int start_pos);
t_redir_data	init_redir_data(char **out_file, int *out_mode);
int				open_and_check_file(char *filename, int mode);
int				get_redir_mode(int op_len);
char			*handle_file_extraction(char *line, int *i, char **out_file);
int				process_redir_opening(char *filename, char **out_file, int mode,
					int *has_error);
int				expand_redir_filename(char **filename, int mode,
					t_shell_data *data);
int				has_input_redirection(char *command);
int				output_comes_before_input(char *command);
char			*handle_heredoc_redir(char *line, int *i, int *heredoc_fd,
					int *input_fd);
int				handle_heredoc(char *delimiter);
char			*parse_input_redirections(char *line, int *heredoc_fd,
					int *input_fd);
char			*parse_input_redir_with_err(char *line, int *heredoc_fd,
					int *input_fd, int *syntax_err);
int				is_input_redir_operator(char *str, int pos, int in_sq,
					int in_dq);
char			*extract_heredoc_delimiter(char *line, int *pos);
char			*process_input_redirection(char *line, int *i, int *heredoc_fd,
					int *input_fd);
int				open_input_file(char *filename);
char			*process_single_input_redir(char *result, int *i,
					int *heredoc_fd, int *input_fd);
char			*parse_input_loop(char *result, int *heredoc_fd, int *input_fd);

#endif
