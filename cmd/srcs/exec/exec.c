/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aglorios <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 15:42:11 by aglorios          #+#    #+#             */
/*   Updated: 2019/10/22 16:43:58 by aglorios         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int		ft_error3(shell *st, char *tmp, int i, struct stat b)
{
	if (tmp[i + 1] != '\0'
		&& !ft_strncmp((char *)st->tokens->content, "whoami\0", 7))
		return (ft_notfound_error(st, 127));
	if (tmp[i + 1] == '\0' && stat((char *)st->tokens->content, &b) == -1)
		return (ft_directory_error(st, 127));
	return (1);
}

int		ft_error2(shell *st, char *tmp, int i, struct stat b)
{
	char *cmd;

	cmd = NULL;
	if (tmp[i + 1] == '\0' || tmp[i + 1] == ':'
		|| tmp[ft_strlen(tmp) - 1] == ':')
	{
		if (ft_error3(st, tmp, i, b) == 1)
			return (1);
		else
			return (0);
	}
	if (!ft_strchr((char *)st->tokens->content, '/') && tmp[i + 1] != ':')
	{
		cmd = st->cmdexec;
		if (!ft_strcmp(cmd, (char *)st->tokens->content))
			return (ft_notfound_error(st, 127));
	}
	if (ft_error4(st, cmd, b) == 1)
		return (1);
	else
		return (0);
}

int		ft_error(shell *st, struct stat b)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	i = 0;
	while (st->envv)
	{
		tmp = (char *)st->envv->content;
		if (!ft_strncmp("PATH=", tmp, 5))
			break ;
		st->envv = st->envv->next;
		tmp = NULL;
	}
	st->envv = st->envv;
	if (tmp != NULL)
	{
		while (tmp[i] && tmp[i] != '=')
			i++;
	}
	if (tmp == NULL)
	{
		if (!ft_strncmp((char *)st->tokens->content, "whoami\0", 7))
			ft_free_list(st->envv, st->firstenv);
		return (ft_directory_error(st, 127));
	}
	if (ft_error2(st, tmp, i, b) == 1)
		return (1);
	else
		return (0);
}

void	ft_exec2(shell *st, int a)
{
	if (st->cutline->next)
		nc = 1;
	if (!st->pipe->next)
	{
		pid2 = a;
		wait(&a);
		st->status = a / 256;
	}
}

int		ft_exec(shell *st)
{
	char		**ar;
	char		**en;
	struct stat	b;
	int			i;
	int			a;

	ar = NULL;
	en = NULL;
	i = 0;
	if (ft_error(st, b) == 0)
		return (0);
	a = fork();
	ar = ft_tabreturn(st->tokens);
	en = ft_tabreturn(st->envv);
	if (a == 0)
		i = execve((char *)st->cmdexec, ar, en);
	if (errno == 13)
	{
		ft_permission_error(st, 126, st->cmdexec);
		return (0);
	}
	ft_exec2(st, a);
	ft_freetab(ar);
	ft_freetab(en);
	return (i);
}
