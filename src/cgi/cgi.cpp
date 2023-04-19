/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matef <matef@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 06:37:45 by yel-khad          #+#    #+#             */
/*   Updated: 2023/04/18 21:47:47 by matef            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

CGI::CGI(Request request, Server server, string url, string method) : _request(request) , _server(server) , _url(url) , _method(method)
{
    int pipefd[2];
    int save0 = dup(0);
    int save1 = dup(1);

    cout << "#" << _request.getValueOf("Cookie") << "#" << endl;
    getScriptName();
    pipe(pipefd);
    pid_t pid;
    if ((pid = fork()) < 0 || !_args)
    {
        _resp = "error";
        return ;
    }
    if (pid == 0) 
    {
        if (chdir(url.erase(url.find_last_of('/'),url.length()).c_str()) != 0)
            exit(1);
        string body = "" + _request.getBody();
        FILE *tmp = std::tmpfile();
        if (!tmp)
            exit(1);
        std::fputs(body.c_str(), tmp);
        std::rewind(tmp);
        dup2(::fileno(tmp), 0);
        close(::fileno(tmp));
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        alarm(300);
        execve(_args[0], _args, setENV());
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    char buffer[4096] = "";
    size_t nread;
    close(pipefd[1]);
    while ((nread = read(pipefd[0], buffer, sizeof(buffer))) > 0)
        _resp += buffer;
    close(pipefd[0]);
    dup2(save0, STDIN_FILENO);
    dup2(save1, STDOUT_FILENO);
    close(save0);
    close(save1);
    delete[] _args;
    if (WIFSIGNALED(status) || status != 0)
        _resp = "error";
}

char **CGI::_envToChar()
{
    char **ret = new char*[_env.size() + 1];
    size_t i = 0;

    for (;i < _env.size(); i++)
    {
        ret[i] = new char[_env[i].size() + 1];
        ret[i] = (char *)_env[i].c_str();
    }
    ret[i] = NULL;
    return ret;
}

string CGI::getResp()
{
    return _resp;
}

char **CGI::setENV()
{
    _env.push_back("REQUEST_METHOD=" + _method);
    _env.push_back("SERVER_SOFTWARE=webserv/1.0");
	_env.push_back("SERVER_NAME=" + _server.getServerName());
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("SERVER_PORT=" + to_string(_server.getPort()[0]));
	_env.push_back("REQUEST_METHOD=" + _request.getMethod());
	_env.push_back("SCRIPT_NAME=" + _scriptName);
	_env.push_back("CONTENT_TYPE=" + _request.getValueOf("Content-Type"));
	_env.push_back("QUERY_STRING=" + _request.getQueryString());
	_env.push_back("REDIRECT_STATUS=200");
    _env.push_back("SCRIPT_FILENAME=" + string(strdup((Request::getVector(_url, '/').back()).c_str())));
    _env.push_back("CONTENT_LENGTH=" + to_string(_request.getBody().length()));
    _env.push_back("DOCUMENT_ROOT=" + _server.getRoot());
    _env.push_back("HTTP_COOKIE=" + _request.getValueOf("Cookie"));

    
    return (_envToChar());
}

bool CGI::isPython()
{
    string url = _url;
    string end = url.erase(0, url.find_last_of('.'));
    if (end == ".py")
        return true;
    return false;
}

void    CGI::getScriptName()
{
    map<string, string> map = _server._locations[_server.getMatchedLocation()]._directives;
    vector<string> cgi_info;
    char **args = new char*[3];
    _args = NULL;
    string cgi_key = (isPython()) ? "cgi_info_py" : "cgi_info_php";
    if (map.find(cgi_key) != map.end())
        cgi_info = Request::getVector(map[cgi_key]);
    else if (cgi_key == "cgi_info_py")
        cgi_info = Request::getVector(_server.getCgiInfoPY());
    else
        cgi_info = Request::getVector(_server.getCgiInfoPHP());
    if (cgi_info.empty()){
        return ;}

    args[0] = strdup(cgi_info[1].c_str());
    args[1] = strdup((Request::getVector(_url, '/').back()).c_str());
    args[2] = NULL;
    _args = args;
}
