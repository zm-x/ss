#include "minishell.h"

static int	countword(char const *s, char c)
{
	int	i;
	int	r;

	i = 0;
	r = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		while (s[i] != c && s[i])
		{
			i++;
			if ((s[i] == c) || (s[i] == '\0'))
				r++;
		}
		while (s[i] == c)
			i++;
	}
	return (r);
}

static int	countlen(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	**free_split(char ***str)
{
	int	k;

	k = 0;
	while (*str[k])
		free(*str[k++]);
	free(str[k]);
	return (NULL);
}

static void	normfunc(const char *s, char c, int *ptr, int *jptr)
{
	int	i;

	i = 0;
	*jptr = 0;
	while (s[i] == c)
		i++;
	*ptr += i;
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		k;
	int		j;
	char	**str;

	i = 0;
	k = 0;
	if (!s)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (countword(s, c) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		normfunc(s + i, c, &i, &j);
		str[k] = (char *)malloc(countlen(s + i, c) + 1);
		if (!str[k])
			return (free_split(&str));
		while (s[i] != c && s[i])
			str[k][j++] = s[i++];
		str[k++][j] = '\0';
		normfunc(s + i, c, &i, &j);
	}
	str[k] = NULL;
	return (str);
}