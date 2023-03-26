#include "cgi.hpp"

CGI::CGI(Request request, Server server, string url)
{
    int pipefd[2];
    string body; // should get it from the request
    stringstream buf(body);
    char **args = getScriptName(request, server, url);

    cin.rdbuf(buf.rdbuf());
    pid_t pid = fork();
    if (pid == 0) 
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execve(args[0], args, NULL);
        exit(1);
    }
    close(pipefd[1]);
    char buffer[4096]= "";
    ssize_t nread;
    while ((nread = read(pipefd[0], buffer, sizeof(buffer))) > 0)
        _body += string(buffer);
    close(pipefd[0]);
    int status;
    waitpid(pid, &status, 0);
}

char **CGI::_envToChar()
{
    char **ret = new char*[_env.size() + 1];
    int i=0;

    for (;i < _env.size(); i++)
    {
        ret[i] = new char[_env[i].size() + 1];
        ret[i] = (char *)_env[i].c_str();
    }
    ret[i] = NULL;
    return ret;
}

// char **CGI::getScriptName(Request request, Server server, string url)
// {
//     map<string, string> map = server._locations[server.getMatchedLocation()]._directives;
//     vector<string> cgi_info;
//     char **args = new char*[3];

//     if (map.find("cgi_info") != map.end())
//         cgi_info = Request::getVector(map["cgi_info"]);
//     else
//         cgi_info = Request::getVector(server.getCGIINFO().empty()); // Youssef need to add it
//     if (cgi_info.empty())
//         return NULL;
//     string end = url.erase(0, url.find_last_of('.'));
//     if (end == ".py")
//     {
//         if (find(cgi_info.begin(), cgi_info.end(), ".py") != cgi_info.end())
//         {
//             char *args[0] = "python3";
//             char *args[1] = (char *)url.c_str();
//             char *args[2] = NULL;
//         }
//     }
//     return nullptr;
// }
