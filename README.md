# portscan

A port scanner written in C++

## Usage

```
portscan -a <address> -p <port> 
```

You can also set ranges with the format 

```
portscan -a <address> -r <start-end>
```

and example of an inclusive scan from port 80 to 90 would be
```
portscan -a 127.0.0.1 -r 80-90
```

## TODO
    - implement multithreading, I planned to do this using openmp but haven't gotten around to installing and getting it working on OSX yet.
