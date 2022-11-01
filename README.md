# Webserv CPP98


# Get Started
```
make
```

```
./webserv configuration_file.json
```

# Parsing
## Parse Configuration File
![[parsing_json_file.png]]

- Configuration file must be JSON file
- See how to parse JSON [here](https://www.json.org/json-en.html)

### Configurations values for Server

| Key                  | Type                      | Value                                                               | 
| :--------------      |:-----------------------   |:-----------------------------------------------------------------   |
| address              | STRING                    | IP address on which the server is listening on                      | 
| port                 | INT                       | port on which the server is listening on                            |
| server_name          | STRING or ARRAY           | names used to connect to the server                                 |

### Configurations values for Locations

| Key                  | Type                      | Value                                                               | 
| :--------------      |:-----------------------   |:-----------------------------------------------------------------   |
| root                 | STRING                    | Root path of the server                                             |
| index                | STRING or ARRAY           | names of index files                                                |
| cgi_bin              | STRING                    | path to the cgi exe by default                                      |
| max_client_body_size | INT                       | Max size of body client request                                     |
| auto_index           | BOOL                      | Set the auto_index on or off                                        |
| disabled_methods     | STRING or ARRAY           | Set the disable methods for all server or location                  |
| error_pages          | OBJECT - String:INT ARRAY | path to custom error pages:errors code                              |
| cgi_extensions       | ARRAY                     | Files extensions where CGI will be executed                         |
| locations            | ARRAY of OBJECT           | each location is a mini server, where some configuration can change |
| location_path        | STRING                    | Set the disable methods for all server or location                  |

### Configurations values for Redirections

| Key                  | Type                      | Value                                                               | 
| :--------------      |:-----------------------   |:-----------------------------------------------------------------   |
| redirections         | ARRAY of OBJECT           | each objects contains old_url, status and can have new_url          |
| old_url              | STRING                    | old url you want redirect                                           |
| new_url              | STRING                    | new url who old url will be redirected on                           |
| status               | INT                       | status link to the redirection                                      |

- First part of configuration is server by default == location by default
- Each locations are considered as little server
- Only `address, port and server_names` cannot be redefined in a location
- If a configuration is not present in a location, this configuration key will take the default value in default location

## Parse Client Request

# Execution

## Select()

![[select_prototype.png]]

- allow your program to monitor multiple file descriptor (max 1024)
- will be waiting for _timeout_ and return a value :
  - -1 : error
  - 0 : if timeout
  - > 0 : total number of bits that are set in readfds, writefds, exceptfds

## Principale Loop

![[principale_loop_resume.png]]

## Explanation
- After select(), 3 loops are crafted :
  - sendResponse : will iterate in `_ready_fd` vector, and send response for each `_ready_fd` already built in `_responses` map
  - requestProcess : will iterate `_active_servers` map, read and process each request sent by client
  - catchCommunication : will iterator `_servers` vector, use accept() function to bind a socket and insert a pair (socket, Server) in map


# Authors

made by [Bima](https://github.com/Bima42), [YvanoffP](https://github.com/YvanoffP) and [Tyrossel](https://github.com/tyrossel)
