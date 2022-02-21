workspace(name = "demo")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# load rules_foreign_cc
git_repository(
    name = "rules_foreign_cc",
    remote = "https://gitee.com/mirrors_bazelbuild/rules_foreign_cc",
    commit = "d02390f1363cdd2ba5a7f7907a481503d483d569",
    shallow_since = "1616082096 +0000",
#    branch = "0.2.0"
)
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# load gtest
git_repository(
    name = "gtest",
    remote = "https://gitee.com/hejuncheng1/googletest",
    commit = "ba96d0b1161f540656efdaed035b3c062b60e006",
#    branch = "release-1.10.x",
)

# load gflags
git_repository(
    name = "gflags",
    remote = "https://gitee.com/anynone/gflags-gflags",
    branch = "v2.2.2",
)

## load openssl
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""
http_archive(
    name = "openssl",
    build_file_content = all_content,
    strip_prefix = "openssl-OpenSSL_1_1_1i",
    urls = ["https://github.com/openssl/openssl/archive/OpenSSL_1_1_1i.tar.gz"],
)

# load protobuf
git_repository(
    name = "com_google_protobuf",
    remote = "https://gitee.com/anynone/protocolbuffers-protobuf",
    commit = "19fb89416f3fdc2d6668f3738f444885575285bc",
    shallow_since = "1610561587 -0800",
#    branch = "4.0.x",
#    branch = "v3.17.3",
)
load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

# load googleapis
git_repository(
    name = "com_google_googleapis",
    remote = "https://gitee.com/anynone/googleapis-googleapis",
    commit = "411e51eea221af5b77f809cef2c6592df45f73d5",
    shallow_since = "1614528000 -0800",
)

# Configure @com_google_googleapis to only compile C++ and gRPC libraries.
load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,  # C++ support is only "Partially implemented", roll our own.
    grpc = True,
)

# load rules proto
http_archive(
    name = "rules_proto",
    sha256 = "602e7161d9195e50246177e7c55b2f39950a9cf7366f74ed5f22fd45750cd208",
    strip_prefix = "rules_proto-97d8af4dc474595af3900dd85cb3a29ad28cc313",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
        "https://github.com/bazelbuild/rules_proto/archive/97d8af4dc474595af3900dd85cb3a29ad28cc313.tar.gz",
    ],
)

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

# load grpc
git_repository(
    name = "com_github_grpc_grpc",
    remote = "https://gitee.com/anynone/grpc-grpc",
    commit = "257d0045ab958eb767a3591c88e9d0c2bdf4b916",
    shallow_since = "1611079677 -0800",
#    branch = "v1.35.0",
)
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()

# load sqlite3
http_archive(
    name = "sqlite3",
    strip_prefix = "sqlite-bazel-6473349723a61ecf929a422795790bf657d86ed9",
    urls = ["https://github.com/rockwotj/sqlite-bazel/archive/6473349723a61ecf929a422795790bf657d86ed9.zip"],
    sha256 = "a04556cb10fe365329c673303b13c0e7a7ad4488767c8537d4e5e8ee0403031c",
)

## load openssl
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""
http_archive(
    name = "openssl",
    build_file_content = all_content,
    strip_prefix = "openssl-OpenSSL_1_1_1i",
    urls = ["https://github.com/openssl/openssl/archive/OpenSSL_1_1_1i.tar.gz"],
)