#!/bin/bash

echo "Monitor de Memória e Swap"
echo "Pressione Ctrl+C para parar"

while true; do
    clear
    echo "=== $(date) ==="
    free -h
    sleep 2
done

# chmod +x memmon.sh
# ./memmon.sh