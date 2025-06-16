/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:40:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/16 16:10:43 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

// Standard C Library
# include <stdio.h>             // printf, perror
# include <stdlib.h>            // malloc, free, exit, getenv
# include <string.h>            // strerror

// POSIX System Calls
# include <unistd.h>            // write, access, read, close, fork, getcwd, chdir, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <sys/wait.h>          // wait, waitpid, wait3, wait4
# include <sys/stat.h>          // stat, lstat, fstat
# include <fcntl.h>             // open
# include <sys/ioctl.h>         // ioctl
# include <signal.h>            // signal, sigaction, sigemptyset, sigaddset, kill

// Directory operations
# include <dirent.h>            // opendir, readdir, closedir

// Termcap Library
# include <termcap.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

// Terminal attributes
# include <termios.h>           // tcsetattr, tcgetattr

// GNU Readline Library
# include <readline/readline.h> // readline, add_history
# include <readline/history.h>  // rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay

// ==================== BUILTIN ====================
short int   custom_cd(char **envp, char **args);
short int   custom_exit(char **args);
short int   custom_echo(char **arg);
short int   custom_pwd();
void        custom_export(char **envp);

// ==================== CLEANUP ====================
void    free_cmd(char **cmd);

// ==================== CONTROLLER ====================
void    process_command(char **envp, char *line);

// ==================== HANDLER ====================
void    run_builtin(char **envp, char **cmd);

// ==================== HELPERS ====================
void        execute_command(char *path, char **cmd, char **envp);
short int   is_builtin(const char *cmd);
char        **copy_envp(char **envp);

// ==================== UTILS ====================
char    *get_env_value(char **envp, const char *name);
char    **get_env_path(char **envp, const char *name);
char    *get_path(char **envp, char **cmd);

#endif