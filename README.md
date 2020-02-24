# host_scanner
Simple tcp port scanner.

Program reads a set of host/port pair from a file then tries to make a tcp connection to each host/port pair and to send a single byte.
If operation succeeds then the pair will marked as connected, if not the pair will be marked as not connected.
At the start and on each change of a host/port state the message will be printed to the console and to the file.
The scanning operation continues until the user send a SIGINT or SIGTERM signal.

The set of host/port pair and a log file passed as command line arguments :

./host_scanner host/port_pair_file log_file


host/port pair should be on a separate line and has a format:

host_ip:host_port
