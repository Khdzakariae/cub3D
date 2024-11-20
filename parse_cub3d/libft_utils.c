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

static void	ft_clear(char **ptr, int current)
{
	int	i;

	i = 0;
	while (i < current)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

static int	checkwords(char const *s, char c)
{
	size_t	i;
	int		words;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			while (s[i] && s[i] != c)
			{
				i++;
			}
			words++;
			continue ;
		}
		i++;
	}
	return (words);
}

static int	fill_arrays(char const *s, char c, char **ptr, int end)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		else
		{
			end = i;
			while (s[end] && s[end] != c)
				end++;
			ptr[j] = ft_substr(s, i, end - i);
			if (ptr[j] == NULL)
				return (ft_clear(ptr, j), 1);
			j++;
			i = end;
		}
	}
	return (ptr[j] = NULL, 0);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**ptr;

	if (!s)
		return (NULL);
	words = checkwords(s, c) + 1;
	ptr = malloc(words * sizeof(char *));
	if (!ptr)
		return (NULL);
	if (fill_arrays(s, c, ptr, 0))
		return (NULL);
	return (ptr);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
