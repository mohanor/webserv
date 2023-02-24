# 85 modules
# 214 directives
# 208 variables


# this {} named contexts

# The main context is here, outside any other contexts

# The Events Context
events {
}

# The HTTP Context

# This context will contain all of the directives and other contexts necessary to define how the program will handle HTTP.

# http (and events) is children of the main context
http {

# directives

# 1: the default locations for access and error logs => access_log error_log 
# 2: configure asynchronous I/O for file operations => aio sendfile direction
# 3: the server’s statuses when errors occur => error_page
# 4: configure compression => gzip gzip_disable
# 5: fine-tune the TCP keep alive settings => keepalive_disable keepalive_requests keepalive_timeout
# 6: rules that Nginx will follow to try to optimize packets and system calls => sendfile tcp_nodelay tcp_nopush
# 7: directives configure an application-level document root and index files => root index
# 8: various hash tables that are used to store different types of data : *_hash_bucket_size *_hash_max_size for server_names, types, variables

# contexts

# the server context is declared within the http context

# you can declate multiple server contexts,  because each instance defines a specific virtual server to handle client requests. You can have as many server blocks as you need, each of which can handle a specific subset of connections.

server {
# first server context

# This context type is also the first that Nginx must use to select an algorithm

    listen
    server_name 

    try_files : configure files to try to respond to requests
    return and rewrite : issue redirects and rewrites
    set : set arbitrary variables



# the location context
location match_modifier location_match {

}



}

server {
# second server context
}

}


config_file := directive*
directive := name parameters? block?
block := "{" directive* "}"
name := /[a-zA-Z][a-zA-Z0-9_]*/
parameters := parameter ("," parameter)*
parameter := /[^\s{]+/
