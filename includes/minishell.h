/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:40:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard C Library
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// POSIX System Calls
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <signal.h>

// Directory operations
# include <dirent.h>

// Termcap Library
# include <termcap.h>

// Terminal attributes
# include <termios.h>

// GNU Readline Library
# include <readline/readline.h>
# include <readline/history.h>

extern int  g_exit_code;

typedef struct s_token
{
    char    *str;
    int     quoted;
    int     type;
}   t_token;

typedef struct s_command
{
    t_token **tokens;
    int     in_fd;
    int     out_fd;
    int     cmd_type;
    int     has_heredoc;
}   t_command;

typedef struct s_pipeline
{
    char **envp;
    char **segments;
    pid_t *pids;
    int nbr_segments;
    t_command *cmds;
}       t_pipeline;

typedef struct s_pipe_context
{
    int in_fd;
    int *fd;
    int last;
} t_pipe_context;

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

// ==================== EXECUTION ====================
int         run_builtin(char ***envp, t_token **cmd);
void        process_command(char ***envp, char *line);
int         is_folder(char *arg);
t_token     **prepare_command(char *segment, int *in_fd, int *out_fd, char ***envp);
void        setup_redirections(int in_fd, int out_fd, int *save_in, int *save_out);
void        restore_redirections(int save_in, int save_out);
short int   is_builtin(const char *cmd);
int         execute_command(char *path, t_token **tokens, char **envp);
char        **prepare_argv_from_tokens(t_token **tokens);

// ==================== ENVIRONMENT ====================
char        *get_env_value(char **envp, const char *name);
char        *get_path(char **envp, char **cmd);
char        **copy_envp(char **envp);
int         env_size(char **env);
char        **env_realloc_add(char **env);
int         env_add(char ***env_ptr, const char *new_var);

// ==================== PIPING ====================
void        execute_pipeline(char **envp, char **segments);
void        execute_cmd(char **envp, t_token **cmd);
void        close_pipe(int *fd);
void        parent_cleanup(int *in_fd, int *fd, int i, int num);
void        wait_for_all(pid_t *pids, int count);
t_token     **handle_redirections(t_token **cmd, char **envp, int *in_fd, int *out_fd);
int         handle_heredoc(const char *delim, int quoted, char **envp, int *in_fd);
void        pipeline_loop(t_pipeline *pipeline);
int         open_infile(char *file, int *in_fd);
int         open_outfile(char *file, int *out_fd);
int         open_appendfile(char *file, int *out_fd);

// ==================== PARSING ====================
// Tokenization
t_token     *new_token(const char *str, int quoted, int type);
t_token     **tokenize_command(char const *s, char c, char **envp);
char        **split_pipes(const char *line);
t_token     **split_redirs(t_token **arr);

// Expansion
void        remove_quotes(char *str);
void        restore_marked_spaces(char *str);
char        *append_literal(char *result, char *str, int start, int i);
char        *expand_var(char *str, int *var_len);
char        *append_expanded_var(char *result, char *str, int *i, char **envp);
char        *build_expanded_str(char *str, char **envp);
t_token     **split_expanded_tokens(t_token **arr);

// ==================== UTILS ====================
void        free_cmd(char **cmd);
void        free_tokens(t_token **arr);
int         count_strings(char **arr);
void        error_msg(const char *prefix, const char *msg);
void        error_perror(const char *context);

#endif
