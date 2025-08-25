/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:40:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/25 13:21:56 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard C Library
# include <stdio.h>    // printf, perror
# include <stdlib.h>   // malloc, free, exit, getenv
# include <string.h>   // strerror

// POSIX System Calls
# include <unistd.h>   // write, access, read, close, fork, getcwd, chdir, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <sys/wait.h> // wait, waitpid, wait3, wait4
# include <sys/stat.h> // stat, lstat, fstat
# include <fcntl.h>    // open
# include <sys/ioctl.h>// ioctl
# include <signal.h>   // signal, sigaction, sigemptyset, sigaddset, kill

// Directory operations
# include <dirent.h>   // opendir, readdir, closedir

// Termcap Library
# include <termcap.h>  // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

// Terminal attributes
# include <termios.h>  // tcsetattr, tcgetattr

// GNU Readline Library
# include <readline/readline.h> // readline, add_history
# include <readline/history.h>  // rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay

extern int  g_exit_code;

// Tokenizer
typedef struct s_token
{
    char    *str;       // actual token text
    int     quoted;     // 0 = unquoted, 1 = single, 2 = double
    int     type;       // e.g. WORD, REDIR_IN, REDIR_OUT, HEREDOC, PIPE
}   t_token;

// Pipe data
typedef struct s_pipeline_data
{
    char **envp;
	char **segments;
	pid_t *pids;
	int nbr_segments;
}	t_pipeline_data;

typedef struct s_pipe_info
{
    int in_fd;
    int *fd;
    int last;
} t_pipe_info;

// ==================== BUILTINS ====================
short int   custom_cd(char ***envp, t_token **args);
short int   custom_echo(t_token **args);
short int   custom_env(char **envp, t_token **args);
short int   custom_exit(t_token **args);
short int   custom_export(char ***env, t_token **args);
short int   custom_pwd(void);
short int   custom_unset(char ***envp, t_token **args);
void        print_env(char **env, int *index, int size);
void        sort_index(char **env, int *index, int size);
void        init_export_index(int *index, int size);
int         is_valid_name(const char *name);
int         update_or_add_env(char ***env, char *arg);

// ==================== CONTROLLER ====================
void        process_command(char ***envp, char *line);
int         run_builtin(char ***envp, t_token **cmd);

// ==================== CONTROLLER_HELPER ====================
int         is_folder(char *arg);
t_token     **prepare_command(char *segment, int *in_fd, int *out_fd, char ***envp);
void        setup_redirections(int in_fd, int out_fd, int *save_in, int *save_out);
void        restore_redirections(int save_in, int save_out);
short int   is_builtin(const char *cmd);

// ==================== HANDLER ====================
int         execute_command(char *path, t_token **tokens, char **envp);

// ==================== ENV LOOKUP ====================
char        *get_env_value(char **envp, const char *name);
char        *get_path(char **envp, char **cmd);

// ==================== ENV UTILS ====================
char        **copy_envp(char **envp);
int         env_size(char **env);
char        **env_realloc_add(char **env);
int         env_add(char ***env_ptr, const char *new_var);

// ==================== PIPING ====================
void        execute_cmd(char **envp, t_token **cmd);
void        close_pipe(int *fd);
void        parent_cleanup(int *in_fd, int *fd, int i, int num);
void        wait_for_all(pid_t *pids, int count);
void        execute_pipeline(char **envp, char **segments);
t_token     **handle_redirections(t_token **cmd, char **envp, int *in_fd, int *out_fd);
int         handle_heredoc(const char *delim, int quoted, char **envp, int *in_fd);
void        pipeline_loop(t_pipeline_data *pipeline);

// ==================== PARSING ====================
void        remove_quotes(char *str);
char        *append_literal(char *result, char *str, int start, int i);
char        *expand_var(char *str, int *var_len);
char        *append_expanded_var(char *result, char *str, int *i, char **envp);
char        *build_expanded_str(char *str, char **envp);
t_token     **split_expanded_tokens(t_token **arr);
t_token     **split_redirs(t_token **arr);
t_token     **tokenize_command(char const *s, char c, char **envp);
char        **split_pipes(const char *line);
t_token     *new_token(const char *str, int quoted, int type);
void        free_tokens(t_token **arr);
char        **prepare_argv_from_tokens(t_token **tokens);

// ==================== MISC ====================
void        free_cmd(char **cmd);
void        free_tokens(t_token **arr);
short int   is_builtin(const char *cmd);
int         count_strings(char **arr);

#endif
