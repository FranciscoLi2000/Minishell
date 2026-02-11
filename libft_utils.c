/* libft_utils.c - String utilities from libft */
/* Author: your_login */

#include "minishell.h"

/**
 * ft_strlen - Calculate length of string
 * @s: String to measure
 *
 * Return: Length of string
 */
size_t  ft_strlen(const char *s)
{
    size_t  len;
    
    len = 0;
    while (s[len])
        len++;
    return (len);
}

/**
 * ft_strdup - Duplicate string
 * @s: String to duplicate
 *
 * Return: New duplicated string or NULL on failure
 */
char    *ft_strdup(const char *s)
{
    size_t  len;
    char    *dup;
    
    len = ft_strlen(s) + 1;
    dup = malloc(len);
    if (!dup)
        return (NULL);
    ft_strlcpy(dup, s, len);
    return (dup);
}

/**
 * ft_strlcpy - Copy string to destination
 * @dst: Destination string
 * @src: Source string
 * @dstsize: Size of destination buffer
 *
 * Return: Length of source string
 */
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
    size_t  src_len;
    size_t  i;
    
    src_len = ft_strlen(src);
    if (dstsize == 0)
        return (src_len);
    i = 0;
    while (src[i] && i < dstsize - 1)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (src_len);
}

/**
 * ft_strchr - Locate first occurrence of character in string
 * @s: String to search
 * @c: Character to find
 *
 * Return: Pointer to first occurrence or NULL
 */
char    *ft_strchr(const char *s, int c)
{
    size_t  i;
    size_t  len;
    
    len = ft_strlen(s);
    i = 0;
    while (i <= len)
    {
        if (s[i] == (char)c)
            return ((char *)&s[i]);
        i++;
    }
    return (NULL);
}

/**
 * ft_strrchr - Locate last occurrence of character in string
 * @s: String to search
 * @c: Character to find
 *
 * Return: Pointer to last occurrence or NULL
 */
char    *ft_strrchr(const char *s, int c)
{
    size_t  i;
    size_t  len;
    
    len = ft_strlen(s);
    i = len;
    while (i > 0)
    {
        if (s[i] == (char)c)
            return ((char *)&s[i]);
        i--;
    }
    if (s[i] == (char)c)
        return ((char *)&s[i]);
    return (NULL);
}

/**
 * ft_strncmp - Compare two strings
 * @s1: First string
 * @s2: Second string
 * @n: Number of characters to compare
 *
 * Return: Difference between strings or 0 if equal
 */
int     ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t  i;
    
    i = 0;
    while (i < n && s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    if (i == n)
        return (0);
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}