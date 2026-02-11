/* libft_string.c - Advanced string utilities from libft */
/* Author: your_login */

#include "minishell.h"

/**
 * ft_substr - Create substring from string
 * @s: Source string
 * @start: Start index of substring
 * @len: Length of substring
 *
 * Return: New substring or NULL on failure
 */
char    *ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t  s_len;
    size_t  substr_len;
    char    *substr;
    
    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    substr_len = s_len - start;
    if (substr_len > len)
        substr_len = len;
    substr = malloc(substr_len + 1);
    if (!substr)
        return (NULL);
    ft_strlcpy(substr, s + start, substr_len + 1);
    return (substr);
}

/**
 * ft_strjoin - Join two strings
 * @s1: First string
 * @s2: Second string
 *
 * Return: New joined string or NULL on failure
 */
char    *ft_strjoin(char const *s1, char const *s2)
{
    size_t  s1_len;
    size_t  s2_len;
    char    *joined;
    
    if (!s1 && !s2)
        return (ft_strdup(""));
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
    
    s1_len = ft_strlen(s1);
    s2_len = ft_strlen(s2);
    joined = malloc(s1_len + s2_len + 1);
    if (!joined)
        return (NULL);
    ft_strlcpy(joined, s1, s1_len + 1);
    ft_strlcpy(joined + s1_len, s2, s2_len + 1);
    return (joined);
}

/**
 * count_words - Helper function to count words in string
 * @str: String to count
 * @delim: Delimiter character
 *
 * Return: Number of words
 */
static size_t count_words(const char *str, char delim)
{
    size_t  count;
    size_t  i;
    int     in_word;
    
    count = 0;
    i = 0;
    in_word = 0;
    while (str[i])
    {
        if (str[i] != delim && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (str[i] == delim)
        {
            in_word = 0;
        }
        i++;
    }
    return (count);
}

/**
 * ft_split - Split string by delimiter
 * @s: String to split
 * @c: Delimiter character
 *
 * Return: Array of strings or NULL on failure
 */
char    **ft_split(char const *s, char c)
{
    size_t  word_count;
    size_t  i;
    size_t  j;
    size_t  start;
    char    **result;
    
    if (!s)
        return (NULL);
    
    word_count = count_words(s, c);
    result = malloc((word_count + 1) * sizeof(char *));
    if (!result)
        return (NULL);
    
    i = 0;
    j = 0;
    while (s[i] && j < word_count)
    {
        while (s[i] == c)
            i++;
        if (s[i] == '\0')
            break;
        start = i;
        while (s[i] && s[i] != c)
            i++;
        result[j] = ft_substr(s, start, i - start);
        if (!result[j])
        {
            while (j > 0)
                free(result[--j]);
            free(result);
            return (NULL);
        }
        j++;
    }
    result[word_count] = NULL;
    return (result);
}

/**
 * ft_atoi - Convert string to integer
 * @str: String to convert
 *
 * Return: Converted integer
 */
int     ft_atoi(const char *str)
{
    int     result;
    int     sign;
    size_t  i;
    
    result = 0;
    sign = 1;
    i = 0;
    
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
           str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
        i++;
    
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return (result * sign);
}

/**
 * ft_strcmp - Compare two strings
 * @s1: First string
 * @s2: Second string
 *
 * Return: Difference between strings or 0 if equal
 */
int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;
    
    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}