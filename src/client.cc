//
// Created by Elaine on 2022/2/12.
// Here is Client
//

// internal stl
#include <vector>

// lib outside
// #include <fmt/core.h>
#include <grpcpp/grpcpp.h>

// user lib
#include "proto/hello.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using helloworld::Hello;
using helloworld::Request;
using helloworld::Response;

class HelloClient {
public:
    explicit HelloClient(const std::shared_ptr<Channel> &channel)
            : stub_(Hello::NewStub(channel)) {}

    void GetRep(const std::string &name) {
        ClientContext context;
        Request req;
        Response res;
        req.set_out_trade_no(name);
        req.set_mchid(name + "66mc");
        // fmt::print("client: start to send request to server: {}\n", name);
        Status status = stub_->GetKey(&context, req, &res);
        // fmt::print("client: receive from server: {}, {}\n", name, res.appid());

        if (!status.ok()) {
            // fmt::print("client: RPC failed, error code: {}, error message: {}.\n", status.error_code(),
            //            status.error_message());
        }
    }

private:
    std::unique_ptr<Hello::Stub> stub_;
};

int main(int argc, char *argv[]) {
    HelloClient client(grpc::CreateChannel("127.0.0.1:50001", grpc::InsecureChannelCredentials()));
    std::vector<std::string> names = {
            "test2", "empty", "", "ttt2"
    };
    for (const auto &name : names) {
        client.GetRep(name);
    }
    return 0;
}
