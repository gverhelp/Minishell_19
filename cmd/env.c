#include "../include/minishell.h"

int	ft_cutline(shell *st)
{
	int a;
	char **line;
	a = 0;
	line = ft_splitms(st->line, ';', st);
	while (line[a])
	{
		ft_lstadd_back(&st->cutline, ft_lstnew(ft_strdup(line[a])));
		a++;
	}
//	printf("envv1|%s|\n", (char *)st->envv->content);
	st->firstcut = st->cutline;
/*
	while (st->cutline != NULL)
	{
		printf("cut|%s|\n", (char *)st->cutline->content);
		st->cutline = st->cutline->next;
	}
	st->cutline = st->firstcut;
*/
    return (0);
}

int	ft_envv(shell *st, char **envp)
{
	int a;
	int i;
	int p;
	int e;
	int s;
	char *tmp;
	int num;
	
	a = 0;
	p = 0;
	e = 0;
	s = 0;
	tmp = NULL;
	num = 0;
	i = 0;
//	printf("envv1|%s|\n", (char *)st->envv->content);
//	printf("ok\n");
	while (envp[a])
	{
		ft_lstadd_back(&st->envv, ft_lstnew(ft_strdup(envp[a])));
		a++;
	}
	st->firstenv = st->envv;
	st->envv = st->firstenv;
	while (st->envv)
	{
		tmp = (char *)st->envv->content;
		if (!ft_strncmp(tmp, "PWD", 3))
			p = 1;
		if (!ft_strncmp(tmp, "_", 1))
			e = 1;
		if (!ft_strncmp(tmp, "SHLVL", 5))
		{
			i = 0;
			while (tmp[i] && tmp[i] != '=')
				i++;
			if (tmp[i] && tmp[i] == '=')
				i++;
			num = ft_atoi(&tmp[i]) + 1;
			st->envv->content = ft_strjoin("SHLVL=", ft_itoa(num));
			s = 1;
		}
		st->envv = st->envv->next;
	}
	st->envv = st->firstenv;
//	printf("_|%i|\n", e);
//	printf("s|%i|\n", s);
	if (p == 0)
		ft_lstadd_back(&st->envv, ft_lstnew(ft_strjoin("PWD=", ft_pwd(st))));
	if (s == 0)
		ft_lstadd_back(&st->envv, ft_lstnew(ft_strdup("SHLVL=1")));
	if (e == 0)
		(void)st;
	st->envv = st->firstenv;
/*
	while (st->envv != NULL)
	{
		printf("envv|%s|\n", (char *)st->envv->content);
		st->envv = st->envv->next;
	}
	st->envv = st->firstenv;
*/
    return (0);
}

char *ft_shlvl(char *line, int i)
{
	int a;

	a = 0;
//	printf("i|%i|\n", i);
//	printf("line |%s|\n", line);
//	if (!ft_atoi(line))
//		a = 1;
//	else
//	{
	a = ft_atoi(line);
	(void)i;
//		if (i != 0)
//			a += 1;
//	}
	if (a < 0)
		a = 0;
	if (a >= 1000)
	{
		write(1, "minishell: warning: shell level (", 33);	
		write(1, ft_itoa(a), ft_strlen(ft_itoa(a)));
		write(1, ") too high, resetting to 1\n", 27);	
		a = 1;
	}
//	printf("level |%i|\n", a);
	line = ft_itoa(a);
	line = ft_strjoin("SHLVL=", line);
//	printf("line |%s|\n", line);
	return (line);
}
