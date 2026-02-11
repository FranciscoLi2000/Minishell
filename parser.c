/* parser.c - Basic command parsing for Minishell */
/* Author: your_login */

#include "minishell.h"

/**
 * count_tokens - Count tokens in token array
 * @tokens: Token array
 *
 * Return: Number of tokens
 */
int     count_tokens(t_token *tokens)
{
    int     count;
    
    count = 0;
    while (tokens[count].str)
        count++;
    return (count);
}

/**
 * free_tokens - Free token array
 * @tokens: Token array to free
 */
void    free_tokens(t_token *tokens)
{
    int     i;
    
    if (!tokens)
        return;
    
    i = 0;
    while (tokens[i].str)
    {
        free(tokens[i].str);
        i++;
    }
    free(tokens);
}

/**
 * free_cmd - Free command structure
 * @cmd: Command to free
 */
void    free_cmd(t_cmd *cmd)
{
    int     i;
    
    if (!cmd)
        return;
    
    if (cmd->argv)
    {
        i = 0;
        while (cmd->argv[i])
        {
            free(cmd->argv[i]);
            i++;
        }
        free(cmd->argv);
    }
    
    if (cmd->infile)
        free(cmd->infile);
    if (cmd->outfile)
        free(cmd->outfile);
    
    free(cmd);
}

/**
 * create_simple_tokenizer - Basic tokenization of input
 * @input: Input string to tokenize
 *
 * Return: Array of tokens or NULL on failure
 */
t_token *create_simple_tokenizer(char *input)
{
    t_token *tokens;
    char    **words;
    int     word_count;
    int     i;
    
    if (!input || ft_strlen(input) == 0)
        return (NULL);
    
    /* For now, use simple space splitting */
    words = ft_split(input, ' ');
    if (!words)
        return (NULL);
    
    /* Count words */
    word_count = 0;
    while (words[word_count])
        word_count++;
    
    /* Allocate tokens */
    tokens = malloc((word_count + 1) * sizeof(t_token));
    if (!tokens)
    {
        /* Free words */
        i = 0;
        while (words[i])
            free(words[i++]);
        free(words);
        return (NULL);
    }
    
    /* Fill tokens */
    i = 0;
    while (i < word_count)
    {
        tokens[i].str = words[i];
        tokens[i].type = 0;  /* word */
        tokens[i].quote_type = 0;  /* none */
        i++;
    }
    tokens[word_count].str = NULL;  /* NULL terminator */
    
    free(words);  /* Only free the array, not the strings */
    
    return (tokens);
}

/**
 * find_executable - Find executable file in PATH or as relative/absolute path
 * @cmd: Command to find
 * @env: Environment variable array
 *
 * Return: Full path to executable or NULL if not found
 */
char    *find_executable(char *cmd, char **env)
{
    char    **paths;
    char    *full_path;
    size_t  i;
    struct  stat st;
    
    if (!cmd)
        return (NULL);
    
    /* Check if it's an absolute or relative path */
    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    /* Search in PATH */
    char *path_env = get_env_value("PATH", env);
    if (!path_env)
        return (NULL);
    
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(full_path, cmd);
        
        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            /* Found executable */
            size_t j = 0;
            while (paths[j])
                free(paths[j++]);
            free(paths);
            return (full_path);
        }
        
        free(full_path);
        i++;
    }
    
    /* Clean up */
    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
    
    return (NULL);
}

/**
 * execute_external_command - Execute external command
 * @cmd: Command to execute
 * @env: Environment variable array
 *
 * Return: Exit status
 */
int     execute_external_command(t_cmd *cmd, char **env)
{
    pid_t   pid;
    char    *executable;
    int     status;
    
    executable = find_executable(cmd->argv[0], env);
    if (!executable)
    {
        fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
        g_global.exit_status = 127;
        return (127);
    }
    
    pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "fork: %s\n", strerror(errno));
        free(executable);
        g_global.exit_status = 1;
        return (1);
    }
    
    if (pid == 0)
    {
        /* Child process */
        execve(executable, cmd->argv, env);
        fprintf(stderr, "%s: %s\n", cmd->argv[0], strerror(errno));
        free(executable);
        exit(127);
    }
    else
    {
        /* Parent process */
        waitpid(pid, &status, 0);
        free(executable);
        
        if (WIFEXITED(status))
        {
            g_global.exit_status = WEXITSTATUS(status);
            return (WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            g_global.exit_status = WTERMSIG(status) + 128;
            return (WTERMSIG(status) + 128);
        }
        
        g_global.exit_status = 1;
        return (1);
    }
}

/**
 * execute_command - Execute a single command
 * @cmd: Command to execute
 * @env: Environment variable array
 */
void    execute_command(t_cmd *cmd, char ***env)
{
    if (!cmd || !cmd->argv[0])
        return;
    
    /* Check if it's a builtin command */
    if (is_builtin(cmd->argv[0]))
    {
        /* Execute builtin */
        if (ft_strcmp(cmd->argv[0], "echo") == 0)
            ft_echo(cmd->argv, cmd->argc);
        else if (ft_strcmp(cmd->argv[0], "cd") == 0)
            ft_cd(cmd->argv, cmd->argc);
        else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
            ft_pwd();
        else if (ft_strcmp(cmd->argv[0], "export") == 0)
            ft_export(cmd->argv, cmd->argc, env);
        else if (ft_strcmp(cmd->argv[0], "unset") == 0)
            ft_unset(cmd->argv, cmd->argc, env);
        else if (ft_strcmp(cmd->argv[0], "env") == 0)
            ft_env(*env);
        else if (ft_strcmp(cmd->argv[0], "exit") == 0)
            ft_exit(cmd->argv, cmd->argc);
    }
    else
    {
        /* Execute external command */
        execute_external_command(cmd, *env);
    }
}