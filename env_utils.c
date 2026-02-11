/* env_utils.c - Environment variable utilities for Minishell */
/* Author: your_login */

#include "minishell.h"

/**
 * get_env_value - Get value of environment variable
 * @key: Environment variable key
 * @env: Environment variable array
 *
 * Return: Value string or NULL if not found
 */
char    *get_env_value(char *key, char **env)
{
    size_t  key_len;
    size_t  i;
    
    if (!key || !env)
        return (NULL);
    
    key_len = ft_strlen(key);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
            return (env[i] + key_len + 1);
        i++;
    }
    return (NULL);
}

/**
 * set_env_value - Set environment variable
 * @key: Environment variable key
 * @value: Value to set
 * @env: Environment variable array pointer
 */
void    set_env_value(char *key, char *value, char ***env)
{
    size_t  i;
    size_t  key_len;
    char    *new_var;
    
    if (!key || !value || !env)
        return;
    
    key_len = ft_strlen(key);
    
    /* Check if variable already exists */
    i = 0;
    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], key, key_len) == 0 && (*env)[i][key_len] == '=')
        {
            /* Variable exists, replace it */
            free((*env)[i]);
            new_var = ft_strjoin(key, "=");
            new_var = ft_strjoin(new_var, value);
            (*env)[i] = new_var;
            return;
        }
        i++;
    }
    
    /* Variable doesn't exist, add it */
    char **new_env = malloc((i + 2) * sizeof(char *));
    if (!new_env)
        return;
    
    /* Copy existing variables */
    size_t j = 0;
    while (j < i)
    {
        new_env[j] = (*env)[j];
        j++;
    }
    
    /* Add new variable */
    new_var = ft_strjoin(key, "=");
    new_var = ft_strjoin(new_var, value);
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    
    /* Free old environment and replace */
    free(*env);
    *env = new_env;
}

/**
 * expand_env_vars - Expand environment variables in string
 * @str: String with potential environment variables
 * @env: Environment variable array
 *
 * Return: New string with expanded variables or NULL on failure
 */
char    *expand_env_vars(char *str, char **env)
{
    size_t  i;
    size_t  start;
    char    *result;
    char    *var_name;
    char    *var_value;
    char    *temp;
    
    if (!str)
        return (NULL);
    
    result = ft_strdup("");
    i = 0;
    
    while (str[i])
    {
        if (str[i] == '$')
        {
            i++;  /* Skip $ */
            start = i;
            
            /* Find variable name */
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                i++;
            
            if (i > start)
            {
                /* Extract variable name */
                var_name = ft_substr(str, start, i - start);
                
                /* Get variable value */
                var_value = get_env_value(var_name, env);
                if (var_value)
                {
                    temp = result;
                    result = ft_strjoin(result, var_value);
                    free(temp);
                }
                free(var_name);
            }
            else
            {
                /* Invalid variable name, just add $ */
                temp = result;
                result = ft_strjoin(result, "$");
                free(temp);
            }
        }
        else
        {
            /* Regular character */
            char char_str[2] = {str[i], '\0'};
            temp = result;
            result = ft_strjoin(result, char_str);
            free(temp);
            i++;
        }
    }
    
    return (result);
}

/**
 * ft_isalnum - Check if character is alphanumeric
 * @c: Character to check
 *
 * Return: 1 if alphanumeric, 0 otherwise
 */
int     ft_isalnum(int c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9'));
}