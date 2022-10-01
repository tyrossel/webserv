void	*ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t			i;
    unsigned char	*p;
    unsigned char	*q;

    i = 0;
    p = (unsigned char *)dst;
    q = (unsigned char *)src;
    while (i < n)
    {
        p[i] = q[i];
        i++;
    }
    return (dst);
}