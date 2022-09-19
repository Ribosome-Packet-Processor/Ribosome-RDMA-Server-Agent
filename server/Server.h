#ifndef RDMA_SERVER_SERVER_H
#define RDMA_SERVER_SERVER_H

#pragma once

#include <netdb.h>
#include <random>

#include "../common/common.h"
#include "../ib_networking/IBNetworking.h"
#include "../connection_manager/ConnectionManager.h"
#include "../serializer/Serializer.h"
#include "../packet_crafter/PacketCrafter.h"
#include "../socket_connection/SocketConnection.h"

class Server {
public:
    explicit Server(std::string, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

    ~Server();

    int init();

private:
    std::string device;
    uint16_t idx;
    uint16_t numaNode;
    uint16_t qps;
    uint16_t minDistrValue;
    uint16_t maxDistrValue;

    Connection *connection = nullptr;
    struct ibv_qp_attr *qpAttrs = nullptr;

    std::shared_ptr<spdlog::logger> logger = nullptr;
    std::shared_ptr<PacketCrafter> crafter = std::make_shared<PacketCrafter>();
};


#endif
