/* globals.c - Global variables and basic initialization for Minishell */
/* Author: your_login */

#include "minishell.h"

/* Global variable - only one allowed per requirements */
t_global g_global = {0, NULL};

/**
 * init_globals - Initialize global variables
 * @envp: Environment variables from main
 */
void    init_globals(char **envp)
{
    int i;
    int env_count;
    
    g_global.exit_status = 0;
    
    /* Count environment variables */
    env_count = 0;
    while (envp[env_count])
        env_count++;
    
    /* Allocate and copy environment variables */
    g_global.env = malloc((env_count + 1) * sizeof(char *));
    if (!g_global.env)
        exit(1);
    
    for (i = 0; envp[i]; i++)
        g_global.env[i] = ft_strdup(envp[i]);
    g_global.env[env_count] = NULL;
}

/**
 * cleanup_globals - Clean up global variables
 */
void    cleanup_globals(void)
{
    int i;
    
    if (g_global.env)
    {
        for (i = 0; g_global.env[i]; i++)
            free(g_global.env[i]);
        free(g_global.env);
        g_global.env = NULL;
    }
}