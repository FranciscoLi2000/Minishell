/* main.c - Main program for Minishell */
/* Author: your_login */

#include "minishell.h"

/**
 * setup_signal_handlers - Set up signal handlers
 */
void    setup_signal_handlers(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

/**
 * cleanup_resources - Clean up resources before exit
 */
void    cleanup_resources(void)
{
    cleanup_globals();
}

/**
 * read_input - Read input from user
 *
 * Return: Input string or NULL on EOF/error
 */
char    *read_input(void)
{
    char    *line;
    size_t  bufsize;
    
    line = NULL;
    bufsize = 0;
    
    printf("minishell> ");
    fflush(stdout);
    
    if (getline(&line, &bufsize, stdin) == -1)
    {
        free(line);
        return (NULL);
    }
    
    /* Remove trailing newline */
    size_t len = ft_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    
    return (line);
}

/**
 * process_command - Process a single command line
 * @input: Input command line
 * @env: Environment variable array
 */
void    process_command(char *input, char ***env)
{
    t_token *tokens;
    char    *expanded_input;
    
    if (!input || ft_strlen(input) == 0)
        return;
    
    /* Expand environment variables */
    expanded_input = expand_env_vars(input, *env);
    if (expanded_input)
    {
        free(input);
        input = expanded_input;
    }
    
    /* Tokenize input */
    tokens = create_simple_tokenizer(input);
    if (!tokens)
    {
        free(input);
        return;
    }
    
    /* For now, execute the first token as a simple command */
    if (tokens[0].str && !ft_strcmp(tokens[0].str, "exit"))
    {
        char *exit_args[] = {"exit", NULL};
        ft_exit(exit_args, 1);
    }
    
    /* Create simple command structure */
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        free_tokens(tokens);
        free(input);
        return;
    }
    
    cmd->argc = 0;
    cmd->argv = malloc(sizeof(char *));
    cmd->argv[0] = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append_mode = 0;
    cmd->pipe_in = 0;
    cmd->pipe_out = 0;
    
    /* Fill command arguments */
    int i = 0;
    while (tokens[i].str)
    {
        char **new_argv = malloc((cmd->argc + 2) * sizeof(char *));
        if (!new_argv)
            break;
        
        int j = 0;
        while (j < cmd->argc)
        {
            new_argv[j] = cmd->argv[j];
            j++;
        }
        new_argv[cmd->argc] = tokens[i].str;
        new_argv[cmd->argc + 1] = NULL;
        
        if (cmd->argv)
            free(cmd->argv);
        cmd->argv = new_argv;
        cmd->argc++;
        
        i++;
    }
    
    /* Execute command */
    if (cmd->argc > 0)
        execute_command(cmd, env);
    
    /* Clean up */
    free_cmd(cmd);
    free_tokens(tokens);
    free(input);
}

/**
 * main - Main entry point for Minishell
 * @argc: Argument count
 * @argv: Argument values
 * @envp: Environment variables
 *
 * Return: Exit status
 */
int     main(int argc, char **argv, char **envp)
{
    char    *input;
    
    (void)argc;
    (void)argv;
    
    /* Initialize global variables */
    init_globals(envp);
    
    /* Set up signal handlers */
    setup_signal_handlers();
    
    /* Main loop */
    while (1)
    {
        input = read_input();
        if (!input)
        {
            /* EOF or error */
            printf("exit\n");
            break;
        }
        
        process_command(input, &g_global.env);
    }
    
    /* Clean up */
    cleanup_resources();
    
    return (g_global.exit_status);
}