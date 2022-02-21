#!/bin/bash

# 1. 若.proto修改了，要重新编译
cd ~/gopath/src/github.com/Elaine-Y/gateway_rp
go mod tidy
buf generate

# 2. 启动gateway
go run main.go

# 3. 编译服务
cd ~/elaineyang/demo
bazel build --cxxopt='-std=c++17' //...

# 4. 运行服务
./bazel-bin/src/server

# curl请求
curl -X GET localhost:8080/v3/pay/transactions/out-trade-no/1217752501201407033233368018?mchid=1230000109
