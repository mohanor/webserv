#include "cgi.hpp"

CGI::CGI(Request request, Server server, string url)
{
    int pipefd[2];
    int save0 = dup(0);
    int save1 = dup(1);
    char **args = getScriptName(request, server, url);
    if (args != NULL)
        _resp = "";
    else
    {
        pipe(pipefd);
        pid_t pid = fork();
        if (pid == 0) 
        {
            chdir(url.erase(url.find_last_of('/'),url.length()).c_str());
            string body = "" + request.getBody(); // should get it from the request
            FILE *tmp = tmpfile();
    	    fputs(body.c_str(), tmp);
        	rewind(tmp);
            dup2(fileno(tmp), 0);
		    close(fileno(tmp));
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            execve(args[0], args, NULL);
            exit(1);
        }
        waitpid(pid, NULL, 0);
        close(pipefd[1]);
        char buffer[4096] = "";
        size_t nread;
        while ((nread = read(pipefd[0], buffer, sizeof(buffer))) > 0)
            _resp += buffer;
        close(pipefd[0]);
        dup2(save0, STDIN_FILENO);
        dup2(save1,STDOUT_FILENO);
        close(save0);
        close(save1);
    }
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

char **CGI::setENV(Request request, Server server, string method)
{
    _env.push_back("REQUEST_METHOD=" + method);
    _env.push_back("SERVER_SOFTWARE=webserv/1.0");
	_env.push_back("SERVER_NAME=" + server.getServerName());
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_PORT=" + server.getPort());
	_env.push_back("REQUEST_METHOD=" + request.getMethod());
	_env.push_back("SCRIPT_NAME=" + _scriptName);
	_env.push_back("CONTENT_TYPE=" + request.getValueOf("Content-Type")); // Matef should add this
	_env.push_back("QUERY_STRING=" + request.getQueryString()); // Matef should add this
    return (_envToChar());
}

char **CGI::getScriptName(Request request, Server server, string url)
{
    map<string, string> map = server._locations[server.getMatchedLocation()]._directives;
    vector<string> cgi_info;
    char **args = new char*[3];

    if (map.find("cgi_info") != map.end())
        cgi_info = Request::getVector(map["cgi_info"]);
    else
        cgi_info = Request::getVector(server.getCgiInfo());
    if (cgi_info.empty())
        return NULL;
    string _url = url;
    string end = url.erase(0, url.find_last_of('.'));
    if (end == ".py")
    {
        if (find(cgi_info.begin(), cgi_info.end(), ".py") != cgi_info.end())
            char *args[] = {(char *)cgi_info[2].c_str(), (char *)_url.c_str(), NULL};
    }
    return NULL;
}
