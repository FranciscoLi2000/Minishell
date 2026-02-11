/* builtins.c - Builtin commands implementation for Minishell */
/* Author: your_login */

#include "minishell.h"
#include <limits.h>

/**
 * is_builtin - Check if command is a builtin
 * @cmd: Command to check
 *
 * Return: 1 if builtin, 0 otherwise
 */
int     is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "echo") == 0 ||
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "env") == 0 ||
        ft_strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

/**
 * ft_echo - Implement echo builtin
 * @argv: Command arguments
 * @argc: Argument count
 *
 * Return: 0 on success
 */
int     ft_echo(char **argv, int argc)
{
    int     i;
    int     newline;
    
    newline = 1;  /* Default: print newline */
    i = 1;
    
    /* Check for -n flag */
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0')
    {
        newline = 0;
        i = 2;
    }
    
    /* Print arguments */
    while (i < argc)
    {
        printf("%s", argv[i]);
        if (i < argc - 1)
            printf(" ");
        i++;
    }
    
    if (newline)
        printf("\n");
    
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_cd - Implement cd builtin
 * @argv: Command arguments
 * @argc: Argument count
 *
 * Return: 0 on success, 1 on error
 */
int     ft_cd(char **argv, int argc)
{
    char    *path;
    char    *home;
    
    if (argc > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        g_global.exit_status = 1;
        return (1);
    }
    
    if (argc == 1)
    {
        /* cd without arguments - go to HOME */
        home = getenv("HOME");
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            g_global.exit_status = 1;
            return (1);
        }
        path = home;
    }
    else
    {
        path = argv[1];
    }
    
    if (chdir(path) == -1)
    {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        g_global.exit_status = 1;
        return (1);
    }
    
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_pwd - Implement pwd builtin
 *
 * Return: 0 on success
 */
int     ft_pwd(void)
{
    char    *cwd;
    
    cwd = malloc(PATH_MAX);
    if (!cwd)
    {
        g_global.exit_status = 1;
        return (1);
    }
    
    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        fprintf(stderr, "pwd: %s\n", strerror(errno));
        free(cwd);
        g_global.exit_status = 1;
        return (1);
    }
    
    printf("%s\n", cwd);
    free(cwd);
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_export - Implement export builtin
 * @argv: Command arguments
 * @argc: Argument count
 * @env: Environment variable array
 *
 * Return: 0 on success
 */
int     ft_export(char **argv, int argc, char ***env)
{
    int     i;
    char    *key;
    char    *value;
    
    if (argc == 1)
    {
        /* Display all environment variables */
        i = 0;
        while ((*env)[i])
        {
            printf("declare -x %s\n", (*env)[i]);
            i++;
        }
        g_global.exit_status = 0;
        return (0);
    }
    
    i = 1;
    while (i < argc)
    {
        /* Parse key=value format */
        key = argv[i];
        value = ft_strchr(argv[i], '=');
        if (value)
        {
            *value = '\0';
            value++;
            set_env_value(key, value, env);
            *(value - 1) = '=';  /* Restore the '=' */
        }
        i++;
    }
    
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_unset - Implement unset builtin
 * @argv: Command arguments
 * @argc: Argument count
 * @env: Environment variable array
 *
 * Return: 0 on success
 */
int     ft_unset(char **argv, int argc, char ***env)
{
    int     i;
    int     j;
    size_t  env_size;
    char    **new_env;
    
    if (argc == 1)
    {
        g_global.exit_status = 0;
        return (0);
    }
    
    /* Count environment variables */
    env_size = 0;
    while ((*env)[env_size])
        env_size++;
    
    /* Create new environment without specified variables */
    new_env = malloc((env_size + 1) * sizeof(char *));
    if (!new_env)
    {
        g_global.exit_status = 1;
        return (1);
    }
    
    i = 0;
    j = 0;
    while ((*env)[i])
    {
        int should_skip = 0;
        int k = 1;
        
        while (k < argc)
        {
            if (ft_strcmp((*env)[i], argv[k]) == 0)
            {
                should_skip = 1;
                break;
            }
            k++;
        }
        
        if (!should_skip)
        {
            new_env[j++] = (*env)[i];
        }
        else
        {
            free((*env)[i]);
        }
        i++;
    }
    new_env[j] = NULL;
    
    free(*env);
    *env = new_env;
    
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_env - Implement env builtin
 * @env: Environment variable array
 *
 * Return: 0 on success
 */
int     ft_env(char **env)
{
    int     i;
    
    i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    
    g_global.exit_status = 0;
    return (0);
}

/**
 * ft_exit - Implement exit builtin
 * @argv: Command arguments
 * @argc: Argument count
 *
 * Return: This function does not return
 */
int     ft_exit(char **argv, int argc)
{
    int     exit_code;
    
    if (argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        g_global.exit_status = 1;
        return (1);
    }
    
    if (argc == 1)
    {
        exit_code = g_global.exit_status;
    }
    else
    {
        exit_code = ft_atoi(argv[1]);
    }
    
    printf("exit\n");
    exit(exit_code);
    return (0);  /* This line never reached */
}