/* minishell.h - Header file for Minishell project */
/* Author: your_login */
/* Description: Data structures and function declarations for Minishell */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <string.h>

/* Global variables - only one allowed */
typedef struct s_global {
    int     exit_status;
    char    **env;
}   t_global;

/* Command structures */
typedef struct s_token {
    char    *str;
    int     type;  // 0=word, 1=pipe, 2=redirection_in, 3=redirection_out, 4=redirection_append
    int     quote_type;  // 0=none, 1=single, 2=double
}   t_token;

typedef struct s_cmd {
    char    **argv;
    int     argc;
    char    *infile;
    char    *outfile;
    int     append_mode;  // 0=overwrite, 1=append
    int     pipe_in;
    int     pipe_out;
}   t_cmd;

/* Builtin functions */
int     ft_echo(char **argv, int argc);
int     ft_cd(char **argv, int argc);
int     ft_pwd(void);
int     ft_export(char **argv, int argc, char ***env);
int     ft_unset(char **argv, int argc, char ***env);
int     ft_env(char **env);
int     ft_exit(char **argv, int argc);

/* Command parsing and execution */
t_token *tokenize_input(char *input);
t_cmd   *parse_tokens(t_token *tokens);
void    execute_command(t_cmd *cmd, char ***env);
void    execute_pipeline(t_cmd *cmd_list, int cmd_count, char ***env);

/* Environment variable handling */
char    *expand_env_vars(char *str, char **env);
char    *get_env_value(char *key, char **env);
void    set_env_value(char *key, char *value, char ***env);

/* Redirection handling */
int     setup_redirections(t_cmd *cmd);
void    cleanup_redirections(int old_stdin, int old_stdout);

/* Builtin utilities */
int     is_builtin(char *cmd);
char    *find_executable(char *cmd, char **env);
int     count_tokens(t_token *tokens);
void    free_tokens(t_token *tokens);
void    free_cmd(t_cmd *cmd);

/* Memory management */
void    *ft_calloc(size_t count, size_t size);
void    ft_free(void **ptr);

/* String utilities (from libft) */
size_t  ft_strlen(const char *s);
char    *ft_strjoin(char const *s1, char const *s2);
char    *ft_strdup(const char *s);
char    **ft_split(char const *s, char c);
int     ft_atoi(const char *str);
void    *ft_memset(void *b, int c, size_t len);
void    ft_bzero(void *s, size_t n);
char    *ft_strchr(const char *s, int c);
char    *ft_strrchr(const char *s, int c);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
int     ft_strcmp(const char *s1, const char *s2);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);
char    *ft_substr(char const *s, unsigned int start, size_t len);
int     ft_isalnum(int c);

/* Global management */
void    init_globals(char **envp);
void    cleanup_globals(void);

/* Simple parser */
t_token *create_simple_tokenizer(char *input);

/* External global variable */
extern t_global g_global;

#endif