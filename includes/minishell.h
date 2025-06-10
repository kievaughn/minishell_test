/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:40:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/04 21:05:07 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

// Standard input/output and string
# include <stdio.h>      // printf, perror, snprintf, etc.
# include <stdlib.h>     // malloc, free, exit, getenv
# include <string.h>     // strerror, strlen, strcpy, etc.

// POSIX system calls & file operations
# include <unistd.h>     // read, write, access, fork, execve, pipe, dup, dup2, chdir, getcwd, isatty, close, ttyname
# include <fcntl.h>      // open, O_* flags for open()
# include <sys/stat.h>   // stat, lstat, fstat
# include <sys/types.h>  // pid_t, mode_t, etc. (needed for many system calls)
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill

// Directory operations
# include <dirent.h>     // opendir, readdir, closedir

// Terminal and tty control
# include <termios.h>    // tcsetattr, tcgetattr
# include <termcap.h>    // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>  // ioctl

// Terminal slot number
# include <unistd.h>     // ttyslot (POSIX, sometimes needs unistd.h)

// Readline library
# include <readline/readline.h>   // readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>    // add_history, rl_clear_history

// ==================== BUILTIN ====================
short int custom_cd(char **envp, char **args);
short int custom_exit(char **args);

// ==================== CLEANUP ====================
void    free_cmd(char **cmd);

// ==================== CONTROLLER ====================
void    process_command(char **envp, char *line);

// ==================== HANDLER ====================
void    run_builtin(char **envp, char **cmd);

// ==================== HELPERS ====================
void        execute_command(char *path, char **cmd, char **envp);
short int   is_builtin(const char *cmd);

// ==================== UTILS ====================
char    *get_env_value(char **envp, const char *name);
char    **get_env_path(char **envp, const char *name);
char    *get_path(char **envp, char **cmd);

#endif