<main> = "http" "{" {<http-directives> | <server>} "}"

<server> = "server" "{" {<server-directives> | <location>} "}"

<location> = "location" <WORD> "{" {<loaction-directives> | <location>} "}"

<http-directives> = {<http-directive> ";"}

<server-directives> = {<server-directive> ";"}

<location-directives> = {<location-directive> ";"}

<http-directive> = "root" <WORD>
                 | "allowed_methods" {("GET" | "POST" | "DELETE")}+
                 | "client_max_body_size" <SIZE>
                 | "autoindex" ("on" | "off")
                 | "index" <WORD>
                 | "error_page" <CODE> <WORD>

<server-directive> = <http-directive>
                   | "listen" <WORD>
                   | "server_name" <WORD>

<location-directive> = <http-directive>
                     | "return" <INT> [<WORD>]