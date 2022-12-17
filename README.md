# Ribosome-RDMA-Server-Agent
This repository contains the code needed to run the RDMA server agent in the Ribosome system. 

This implementation is tested with **MLNX_OFED >= 5.4.0**.

## Requirements
The agent is designed to run on a server equipped with a [Mellanox ConnectX-5](https://www.nvidia.com/en-us/networking/ethernet/connectx-5/) NIC.

Before compiling the agent you need to install the following libraries: 
- [spdlog](https://github.com/gabime/spdlog)
- [libcrafter](https://github.com/pellegre/libcrafter)
- [Boost System](https://www.boost.org/doc/libs/1_68_0/libs/system/doc/index.html)

Before running the server agent, you need to disable the iCRC check on the specific ConnectX-5 port.
In this repository, such commands are not shown as they are under NDA.

## How To Compile
In order to compile the agent, run the following commands from the root project folder:
```bash
mkdir build && cd build
cmake -S .. -B . && make -j
```

## Usage

```
server <device> <idx> <numa> <qps> <min-timer> <max-timer>
```

- `<device>`: name of the interface to use for the connection with Ribosome switch (example: `mlx5_1`)
- `<idx>`: unique index associated to this server, starting from 0. Each RDMA Agent process should be identified with a different index, assigned sequentially
- `<numa>`: index of the NUMA node to use when allocating buffers
- `<qps>`: number of Queue-Pairs to create
- `<min-timer>`: minimum QP reset timer value, in number of packets (example: 200). This is used (in conjuction with `<max-timer>`) to compute a random value that is used in the Ribosome Tofino to idle sending packets towards a freshly restored QP
- `<max-timer>`: maximum QP reset timer value, in number of packets (example: 2000). This is used (in conjuction with `<min-timer>`) to compute a random value that is used in the Ribosome Tofino to idle sending packets towards a freshly restored QP

## How to Run
Before starting the process, you have to ensure that the Linux network interface corresponding to the `mlx5_N` that you want to use towards the Ribosome switch has the correct MTU and an IPv4 address assigned.

First of all, check which Linux interface corresponds to the `mlx5_N` that you want to use (this can be done using the `ibdev2netdev` command). For example the name is `cx5_0if0`.

The server agent opens Queue-Pairs with a path MTU of 4096 bytes. The corresponding network interface MTU must be tweaked in order to work with this value. 
Set the MTU of the interface to 4200 bytes:
```bash
sudo ip link set dev cx5_0if0 mtu 4200
```

Assign an IPv4 address to the interface. Do not worry, even if it an unreachable network, the server agent will install a fake ARP entry to correctly send packets on the `cx5_0if0` interface:
```bash
sudo ip addr add 192.168.40.13/24 dev cx5_0if0
```

At this point you can start the server agent with root privileges, for example:
```bash
sudo ./server mlx5_N 0 0 32 10 100
```

## Real Example of Usage
Consider the experimental setup depicted in the following figure.

![Testbed](https://user-images.githubusercontent.com/10586339/208256941-01d80c36-aba4-4006-a02a-883d4a903c36.png)

The ConnectX-5 port, the `idx`, and the corresponding Tofino port of each `RDMA Server` are shown. To swap things a little bit, `RDMA Server 4` is connected to port 8, while `RDMA Server 3` is connected to port 12. The `idx` is assigned with respect to the corresponding **QP-Restore Mirror Group** assigned in the [Ribosome-P4 `setup.py` script](https://github.com/Ribosome-Packet-Processor/Ribosome-P4/blob/50aa7d4992acab1319907c431f2e27afb67b1b6c/setup.py#L242). In particular, the group is associated to each one of the ports shown in the figure, and it is computed as `200 + idx`. Hence, in order to receive the QP Restore packets for the correct Queue-Pairs, the mirror group must correspond the correct server, denoted by the correct `idx`.

Moreover, each process should be run with the same `<qp>` value (which is the one chosen in the Ribosome-P4 program). In this example we use 32 Queue-Pairs.

The commands to correctly run the server agents are (`<numa>`, `<min-timer>`, and `<max-timer>` values are just examples):
```bash
user@rdma-server-1: sudo ./server mlx5_1 0 0 32 10 100
```
```bash
user@rdma-server-2: sudo ./server mlx5_3 1 0 32 10 100
```
```bash
user@rdma-server-3: sudo ./server mlx5_2 2 0 32 10 100
```
```bash
user@rdma-server-4: sudo ./server mlx5_0 3 0 32 10 100
```
