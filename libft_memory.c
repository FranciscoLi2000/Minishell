/* libft_memory.c - Memory utilities from libft */
/* Author: your_login */

#include "minishell.h"

/**
 * ft_memset - Fill memory with constant byte
 * @b: Memory area to fill
 * @c: Byte value to fill
 * @len: Number of bytes to fill
 *
 * Return: Pointer to memory area b
 */
void    *ft_memset(void *b, int c, size_t len)
{
    size_t  i;
    unsigned char   *ptr;
    
    ptr = (unsigned char *)b;
    i = 0;
    while (i < len)
    {
        ptr[i] = (unsigned char)c;
        i++;
    }
    return (b);
}

/**
 * ft_bzero - Zero out memory area
 * @s: Memory area to zero
 * @n: Number of bytes to zero
 */
void    ft_bzero(void *s, size_t n)
{
    ft_memset(s, 0, n);
}

/**
 * ft_calloc - Allocate and zero-initialize memory
 * @count: Number of elements
 * @size: Size of each element
 *
 * Return: Pointer to allocated memory or NULL
 */
void    *ft_calloc(size_t count, size_t size)
{
    void    *ptr;
    size_t  total_size;
    
    total_size = count * size;
    ptr = malloc(total_size);
    if (!ptr)
        return (NULL);
    ft_bzero(ptr, total_size);
    return (ptr);
}

/**
 * ft_free - Safe memory deallocation
 * @ptr: Pointer to free
 */
void    ft_free(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}