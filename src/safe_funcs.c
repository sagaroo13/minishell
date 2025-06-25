#include "../include/minishell.h"

void *safe_malloc(size_t size, bool calloc_flag)
{
    void *ptr;

    if (size == 0)
        perror(RED"[ERROR]"RESET" Memory allocation failed.");
    if (calloc_flag)
        ptr = calloc(1, size);
    else
        ptr = malloc(size);
    if (!ptr)
        perror(RED"[ERROR]"RESET" Memory allocation failed.");
    return (ptr);
}

void *safe_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;

	if (new_size == 0)
		return (NULL);
	if (old_size == new_size)
		return (ptr);
	new_ptr = realloc(ptr, new_size);
	if (!new_ptr)
		perror(RED"[ERROR]"RESET" Memory reallocation failed.");
	return (new_ptr);
}

void safe_getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror(RED"[ERROR]"RESET" getcwd failed.");
}

void safe_chdir(const char *path)
{
	if (chdir(path) == -1)
		perror(RED"[ERROR]"RESET" chdir failed.");
}

void safe_close(int fd)
{
	if (close(fd) == -1)
		perror(RED"[ERROR]"RESET" close failed.");
}
int safe_open(const char *path, t_open_flags flags)
{
	int fd = 0;

	if (flags == READ)
		fd = open(path, O_RDONLY);
	else if (flags == WRITE)
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (flags == APPEND)
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(RED"[ERROR]"RESET" open failed.");
	return (fd);
}
void safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		perror(RED"[ERROR]"RESET" dup2 failed.");
}

int	safe_dup(int fd)
{
	int new_fd;

	new_fd = dup(fd);
	if (new_fd == -1)
		perror(RED"[ERROR]"RESET" dup failed.");
	return (new_fd);
}
