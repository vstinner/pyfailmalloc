file ./python
handle SIGPIPE noprint nostop
handle SIGUSR1 noprint nostop
handle SIGUSR2 noprint nostop
run -m test -F -r -v
