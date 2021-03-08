#!/bin/bash

echo "PID: $$"
sleep 3
kill -SIGSTOP $$
sleep 3
echo "PID $$ restarted"
