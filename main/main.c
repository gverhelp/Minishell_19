#include "../include/minishell.h"

void ft_init_struct(shell *st)
{
	st->ret = 0;
	st->quotes = 0;
	st->quotes2 = 0;
	st->line = NULL;
	st->pwd = NULL;
	st->pwdcp = NULL;
	st->oldpwd = NULL;
	st->firsttok = NULL;
	st->tmpq = NULL;
	st->tokens = NULL;
	st->envv = NULL;
	st->firstenv = NULL;
	st->flagdq = 0;
	st->status = 0;
	st->cutline = 0;
	st->pipe = NULL;
	st->firstpipe = NULL;
	st->pat = NULL;
	st->fdout = 1;
	st->fdone = 1;
	st->pipefd = 0;
	st->errorredir = 0;
	st->rd = 0;
}

int commandline(int argc, char **argv, char **envp, shell *st)
{
	t_list *tmp;

	tmp = NULL;
	(void)argc;
	(void)argv;
	if (ft_tokens(st) == 1)
	{	
//		if (st->status == 2)
//		{
//			ft_exfree2(st, tmp);
//			return (1);
//		}
//		else
			st->status = 1;
	}
	else
	{
		if (st->tokens)
			ft_cleantokens(st);
		if (ft_command(st, envp) == 1)
		{
			ft_exfree2(st, tmp);
			return (1);
		}
	}
	close(st->fdout);
	if (st->fdout == 0)
		st->fdout = dup2(st->fdone, 0);
	else
		st->fdout = dup2(st->fdone, 1);
	ft_exfree(st, tmp);
	return (0);
}

int ft_pipe(int argc, char **argv, char **envp, shell *st)
{
	int		pop[2];
	pid_t	cpid;

	if (!st->pipe->next)
	{
		if (commandline(argc, argv, envp, st) == 1)
			return (1);
		return (0);
	}
	if (st->pipe->next)
	{
		if (pipe(pop) > 0)
			exit(1);
		if ((cpid = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}
		if (cpid == 0)
		{
			pid = cpid;
			close(pop[0]);
			st->fdout = dup2(pop[1], 1);
			if (commandline(argc, argv, envp, st) == 1)
				return (1);
			exit(0);
		}
		else if (cpid > 0)
		{
			close(pop[1]);
			st->fdout = dup2(pop[0], 0);
			wait(NULL);
		}
	}
	return (0);
}

int mainprocess(int argc, char **argv, char **envp, shell *st)
{
	t_list *tmp;

	tmp = NULL;
	lstcmd(st, st->line);
	ft_cutline(st);
//	printf("wtf\n");
	while (st->cutline)
	{
		ft_cutpipe(st);
		st->fdone = dup(st->fdout);
		while (st->pipe)
		{
			if (ft_pipe(argc, argv, envp, st) == 1)
			{
				ft_freecutpipe(st, tmp);
				ft_freecutline(st, tmp);
				return (1);
			}
			st->pipe = st->pipe->next;	
		}
		ft_freecutpipe(st, tmp);
		st->cutline = st->cutline->next;
	}
	ft_freecutline(st, tmp);
	return (0);
}

void signalhandler(int signum)
{
	(void)signum;
//	printf("pid|%i|\n", pid);
	if (pid == 0)
	{
		kill(0, 0);
		pid = 1;
	}
	else
	{
		write(2,"\n>>",3);
		prompt = 1;
	}
//	printf("pid2|%i|\n", pid);
}

int main(int argc, char **argv, char **envp)
{
	shell	st;
	t_list	*tmp;

	ft_init_struct(&st);
	pid = 1;
	prompt = 0;
	tmp = NULL;
//	write(1,"\n",1);
//	write(1,"by Aglorios and Gverhelp\n",25);
//	write(1,"\n",1);
//	envp = 0;
	ft_envv(&st, envp);
	if  (argc > 1 && !ft_strncmp(argv[1], "-c", 2))
	{
		st.line = ft_strdup(argv[2]);
		mainprocess(argc, argv, envp, &st);
	}
	else
	{
		signal(SIGINT, signalhandler);
		while(1)
		{
			if (prompt == 0)
				write(2,">>",2);
			prompt = 0;
			if (get_next_line3d(0, &st.line) != 1)
			{
				ft_exfree2(&st, tmp);
				write(1, "exit\n", 5);
				return(0);
			}
			if (mainprocess(argc, argv, envp, &st) == 1)
				break;
		}
	}
//	ft_freetab(envp);
	if (st.status != 0)
		return (st.status);
	return (0);
}
