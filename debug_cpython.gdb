file ./python
handle SIGPIPE noprint nostop
handle SIGUSR1 noprint nostop
handle SIGUSR2 noprint nostop
handle SIGXFSZ noprint nostop
# test_tracemalloc may call tracemalloc.stop() before failmalloc.disable(),
# and so wrong allocators may be used. Exclude the test
run -m test -F -r -v -x test_tracemalloc
