#include "cgi.hpp"

CGI::CGI(Request request, Server server, string body)
{
    int pipefd[2];
    stringstream buf(body);

    if (pipe(pipefd) == -1)
        cerr << "Error: Failed to create pipe\n";
    cin.rdbuf(buf.rdbuf());
    pid_t pid = fork();
    if (pid == -1)
        cerr << "Error: Failed to fork\n";
    else if (pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execve(args[0], args, _envToChar());
        cerr << "Error: Failed to execute CGI script\n";
        exit(1);
    }
    close(pipefd[1]);
    char buffer[4096];
    size_t nread;

    while ((nread = read(pipefd[0], buffer, sizeof(buffer))) > 0) 
        _body += buffer;
    int status;
    waitpid(pid, &status, 0);
    close(pipefd[0]);
}

char **CGI::_envToChar()
{
    char **ret = new char*[_env.size() + 1];

    for (int i=0; i < _env.size(); i++)
    {
        ret[i] = new char[_env[i].size() + 1];
        ret[i] = (char *)_env[i].c_str();
    }
    return ret;
}

