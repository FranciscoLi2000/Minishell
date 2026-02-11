/* simple_minishell.c - Simplified Minishell for testing */
/* Author: your_login */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* Simple string functions */
size_t  ft_strlen(const char *s)
{
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

int     ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

char    *ft_strdup(const char *s)
{
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);
    if (dup)
    {
        strcpy(dup, s);
    }
    return dup;
}

/* Builtin functions */
int     builtin_echo(char **argv, int argc)
{
    int i = 1;
    int newline = 1;
    
    if (argc > 1 && strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    
    while (i < argc)
    {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
        i++;
    }
    
    if (newline) printf("\n");
    return 0;
}

int     builtin_pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        return 0;
    }
    return 1;
}

int     builtin_cd(char **argv, int argc)
{
    if (argc > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    
    char *path = (argc == 1) ? getenv("HOME") : argv[1];
    
    if (!path)
    {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    
    if (chdir(path) == -1)
    {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        return 1;
    }
    
    return 0;
}

int     builtin_exit(char **argv, int argc)
{
    printf("exit\n");
    if (argc > 1)
        exit(atoi(argv[1]));
    exit(0);
    return 0;
}

int     builtin_env(void)
{
    extern char **environ;
    char **env = environ;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    return 0;
}

/* Execute external command */
int     execute_external(char **argv)
{
    pid_t pid = fork();
    
    if (pid == -1)
    {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        return 1;
    }
    
    if (pid == 0)
    {
        /* Child process */
        extern char **environ;
        execvp(argv[0], argv);
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(127);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return WTERMSIG(status) + 128;
        return 1;
    }
}

/* Check if builtin */
int     is_builtin(char *cmd)
{
    return (cmd && (
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "exit") == 0));
}

/* Execute command */
void    execute_command(char **argv, int argc)
{
    if (argc == 0 || !argv[0])
        return;
    
    if (strcmp(argv[0], "echo") == 0)
        builtin_echo(argv, argc);
    else if (strcmp(argv[0], "pwd") == 0)
        builtin_pwd();
    else if (strcmp(argv[0], "cd") == 0)
        builtin_cd(argv, argc);
    else if (strcmp(argv[0], "env") == 0)
        builtin_env();
    else if (strcmp(argv[0], "exit") == 0)
        builtin_exit(argv, argc);
    else
        execute_external(argv);
}

/* Simple tokenizer */
char    **tokenize_input(char *input)
{
    char **tokens = malloc(sizeof(char *) * 1024);
    int token_count = 0;
    char *token = strtok(input, " \t\n");
    
    while (token && token_count < 1023)
    {
        tokens[token_count++] = ft_strdup(token);
        token = strtok(NULL, " \t\n");
    }
    tokens[token_count] = NULL;
    
    return tokens;
}

/* Free tokens */
void    free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

/* Main function */
int     main(void)
{
    char    *input = NULL;
    size_t  bufsize = 0;
    
    while (1)
    {
        printf("minishell> ");
        fflush(stdout);
        
        if (getline(&input, &bufsize, stdin) == -1)
        {
            printf("\n");
            break;
        }
        
        /* Remove newline */
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
            input[len - 1] = '\0';
        
        if (strlen(input) == 0)
            continue;
        
        /* Tokenize and execute */
        char **tokens = tokenize_input(input);
        if (tokens[0])
        {
            execute_command(tokens, 0); /* argc not used in simple version */
        }
        free_tokens(tokens);
    }
    
    free(input);
    return 0;
}