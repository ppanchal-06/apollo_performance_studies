# Usage

## Prerequisites

- `make` and a C/C++ toolchain installed.
- Network connectivity between server and client machines.

## Build

1. Build everything: `make`
2. Build a single target: `make tcp_server` or `make tcp_client` (or `g++ -Wall -O2 -std=c++11 tcp_server.cpp -o tcp_server -lboost_system -pthread`)
3. Or build with your preferred build system.

## Run the server

- On the host that will accept connections, start the server:
  `./tcp_server`
- Ensure firewalls allow the chosen port.

## Run the client

- From a client machine, connect to the server by hostname or IP:
  `./tcp_client <server-host> [-o output_file]`
- Example:
  `./tcp_client CMS-L1T-SERENITY-1 -o results.csv`

## Bash script

- `run_all.sh` runs the setup across all Z boards and the S1-021 node of Serenity-S1.
- Make it executable and run:
  `chmod +x run_all.sh && ./run_all.sh`

## Plotting/Comparing

- Update the notebook with your output CSV file names to view the latency and throughput results.

## Notes

- Check logs and stderr for errors.
- If connections fail, verify network/firewall settings and that the server is listening on the expected port.
- For automated runs, ensure SSH keys are in place and the script has the correct hosts list.
