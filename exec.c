#include "minishell.h"

int open_pathcd(shell *st, char *path)
{
//	struct stat b;
	(void)st;
//	printf("|%s|\n", path);
//	printf("|%i|\n", errno);
//	if (!opendir(path))
//	{
	//	printf("|%i|\n", errno);
	if (errno == 13)
	{
		if (!ft_strcmp((char *)st->tokens->content, "cd\0"))
		{
			write(1, "minishell: ", 11);
			write(1, "cd: ", 4);
			write(1, path, ft_strlen(path));
			write(1, ": Permission denied\n", 20);
		}
		else
		{
			write(1, "minishell: ", 11);
			write(1, path, ft_strlen(path));
			write(1, ": Permission denied\n", 20);
		}
//			st->status = 126;
		return (0);
	}
	if (errno == 2)
	{
		if (!ft_strcmp((char *)st->tokens->content, "cd\0"))
		{
			write(1, "minishell: ", 11);
			write(1, "cd: ", 4);
			write(1, path, ft_strlen(path));
			write(1, ": No such file or directory\n", 28);
		}
//			st->status = 126;
		return (0);
	}
	return (1);
}

int	check_path(shell *st, char *dollars)
{
	char *cmd;
	char *path;
	char *cmppath;
	char *tmp;
	int i;
	struct stat b;
	char **tab;

	tmp = NULL;
	tab = NULL;
	path = NULL;
	cmd = (char *)st->tokens->content;
	cmppath = ft_strdup("PATH=");
	i = 0;
	while (st->envv)
	{
		tmp = ft_strdup((char *)st->envv->content);
		if (!ft_strncmp(cmppath, tmp, ft_strlen(cmppath)))
		{
			i = 0;
			while (tmp[i] != '=')
				i++;
			if (tmp[i] == '=')
				i++;
			path = ft_strdup(&tmp[i]);
			break;
		}
		st->envv = st->envv->next;
	}
	st->envv = st->firstenv;
//	printf("path|%s|\n", path);
	//////////////////////////////////// fin recup path
//	printf("tokens|%s|\n", (char *)st->tokens->content);
//	printf("cmd |%s|\n", cmd);

//	if (!ft_strcmp(cmd, "cd\0") && st->tokens->next)
//	{
//		if (!open_path(st, (char *)st->tokens->next->content))
//			return (0);
//	}
/*	if (!ft_strncmp(cmd, "./", 2))
	{
		if (!open_path(st, (char *)st->tokens->content))
			return (0);
	}
*/	//////////////////////////////////// open path
	tab = ft_split(path, ':');
	i = 0;
	tmp = NULL;
	cmppath = NULL;
//	printf("dollars |%s|\n", dollars);
//	printf("dollars1 |%s|\n", dollars);
	if (dollars[i] && ft_strchr(dollars, ' '))
	{
		st->cmdexec = ft_strdup("");
		while (dollars[i] && dollars[i] == ' ')
			i++;
		if (!dollars[i])
			return (0);
		while (dollars[i] && dollars[i] != ' ')
		{
			st->cmdexec = ft_charjoin(st->cmdexec, dollars[i]);
			i++;
		}
		cmd = st->cmdexec;
		if (!ft_strcmp(cmd, "echo")) /////////////////////////////// check commmmande builtin
		{
	//		printf("cmd |%s|\n", cmd);
			return (0);
		}
		st->tokens->content = cmd; ///////////// remplacer token
		while (dollars[i]) ////////////////////////// modif while
		{
			cmppath = ft_strdup("");
			while (dollars[i] == ' ')
				i++;
			while (dollars[i] && dollars[i] != ' ')
			{
				cmppath = ft_charjoin(cmppath, dollars[i]);
				i++;
			}
//			printf("st->tokens|%s|\n", (char *)st->tokens->content);
			ft_lstadd_back(&st->tokens, ft_lstnew(cmppath)); ///////////// remplacer token
			i++;
		}
	}
	else
		st->cmdexec = ft_strdup((char *)st->tokens->content);
//	printf("cmppath|%s|\n", cmppath);
//	printf("cmd |%s|\n", st->cmdexec);
	i = 0;
	while (tab[i])
	{
//		printf("tab|%s|\n", tab[i]);
		if (!ft_strchr(cmd, '/'))
		{
//			printf("tokens|%s|\n", (char *)st->tokens->content);
			if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
				!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export") ||
				!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"))
				return (0);
			tmp = ft_strjoin(tab[i], "/");
			tmp = ft_strjoin(tmp, cmd);
			if (stat(tmp, &b) != -1)
			{
			//	st->tokens->content = tmp;
				if (cmppath)
					st->cmdexec = ft_strjoin(tmp, cmppath);
				else
					st->cmdexec = tmp;
				return (1);
			}
		}
		i++;
	}
	if (stat(cmd, &b) == -1)
		return (0);
	return (1);
}

static char **ft_tabreturn(t_list *list)
{
	t_list *tmp;
	char **tab;
	int i;
	
	tab = NULL;
	i = 0;
	tmp = list;
	if ((tab = malloc(sizeof(char *) * ft_lstsize(list) + 1)) == NULL)
		return (0);
	while (list)
	{
		tab[i] = ft_strdup((char *)list->content);
		i++;
		list = list->next;
	}
	tab[i] = NULL;
	list = tmp;
	return (tab);
}

int ft_exec(shell *st)
{
	char **ar;
	char **en;
	struct stat b;
	int i;
	int a;

	ar = NULL;
	en = NULL;
	if (stat(st->cmdexec, &b) == -1)
		return (0);
	a = fork();
	ar = ft_tabreturn(st->tokens);
	en = ft_tabreturn(st->envv);
/*
	i = 0;
	printf("st->cmd|%s|\n", st->cmdexec);
	while (ar[i])
	{
		printf("ar|%s|\n", ar[i]);
		i++;
	}
	i = 0;
*/
//	printf("st->cmd1|%s|\n", st->cmdexec);
//	if (!ft_strncmp(st->cmdexec, "path/", 5))
//	{
//		printf("st->cmd2|%s|\n", st->cmdexec);
//		st->cmdexec = ft_strjoin("./", st->cmdexec);
//		if (!open_path(st, st->cmdexec))
//			return (0);
//	}
	if (a == 0)
		i = execve((char *)st->cmdexec, ar, en);
//		i = execve((char *)st->tokens->content, ar, en);
//		printf("e|%i|\n", errno);
	wait(&a);
	st->status = a/256;
//	printf("st->status|%i|\n", st->status);
	return (i);
}
