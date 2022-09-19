# Ribosome-RDMA-Server-Agent
This repository contains the code needed to run the RDMA server agent in the Ribosome system. 

## Requirements
The agent is designed to run on a server equipped with a [Mellanox ConnectX-5](https://www.nvidia.com/en-us/networking/ethernet/connectx-5/) NIC.

Before compiling the agent you need to install the following libraries: 
- [spdlog](https://github.com/gabime/spdlog)
- [libcrafter](https://github.com/pellegre/libcrafter)
- [Boost System](https://www.boost.org/doc/libs/1_68_0/libs/system/doc/index.html)

## How To Compile
In order to compile the agent you need to type the following command: 

```bash
mkdir build && cd build
cmake -S .. -B . && make -j
```