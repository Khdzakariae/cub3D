#include "main.h"

bool space_checker(char *line)
{
	while (*line)
	{
		if (*line != ' ')
			return (false);
		line++;
	}
	return (true);
}

char    **ft_realloc(char **cube_file, int line_count)
{
    char **tmp;
    int i;

    tmp = (char **)malloc(sizeof(char *) * line_count + 1);
    if (!tmp)
    {
        printf("Error\nFailed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (i < line_count - 1)
    {
        tmp[i] = cube_file[i];
        i++;
    }
    free(cube_file);
    cube_file = tmp;
    return (cube_file);
}

char	*ft_strdup(char *s)
{
	char	*ptr;
	size_t	i;

	i = 0;
	ptr = malloc(ft_strlen(s) + 1 * sizeof(char));
	if (!ptr)
		return (NULL);
	while (s[i])
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen((char*)s))
		return (ft_strdup(""));
	if (len > ft_strlen((char *)s) - start)
		len = ft_strlen((char *)s) - start;
	ptr = (char *)malloc(len + 1 * sizeof(char));
	if (!ptr)
		return (NULL);
	while (i < len && s + start)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
};

static int	is_set(char const *set, char ret)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == ret)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && is_set(set, s1[start]))
		start++;
	end = ft_strlen((char *)s1);
	while (end && is_set(set, s1[end - 1]))
		end--;
	return (ft_substr((char *)s1, start, end - start));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((i < n) && (s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
		{
			break ;
		}
		i++;
	}
	if (i == n || (s1[i] == '\0' && s2[i] == '\0'))
	{
		return (0);
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

bool ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}