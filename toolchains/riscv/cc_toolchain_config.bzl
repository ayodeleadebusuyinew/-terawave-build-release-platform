load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

load(
    "@bazel_tools//tools/build_defs/cc:action_names.bzl",
    "ACTION_NAMES",
)

def _impl(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/usr/bin/riscv64-linux-gnu-gcc",
        ),
        tool_path(
            name = "ld",
            path = "/usr/bin/riscv64-linux-gnu-ld",
        ),
        tool_path(
            name = "ar",
            path = "/usr/bin/riscv64-linux-gnu-ar",
        ),
        tool_path(
            name = "cpp",
            path = "/usr/bin/riscv64-linux-gnu-cpp",
        ),
        tool_path(
            name = "gcov",
            path = "/usr/bin/riscv64-linux-gnu-gcov",
        ),
        tool_path(
            name = "nm",
            path = "/usr/bin/riscv64-linux-gnu-nm",
        ),
        tool_path(
            name = "objdump",
            path = "/usr/bin/riscv64-linux-gnu-objdump",
        ),
        tool_path(
            name = "strip",
            path = "/usr/bin/riscv64-linux-gnu-strip",
        ),
    ]

    default_compile_flags = feature(
        name = "default_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.c_compile,
                    ACTION_NAMES.cpp_compile,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-O2",
                            "-Wall",
                            "-Wextra",
                        ],
                    ),
                ],
            ),
        ],
    )

    default_link_flags = feature(
        name = "default_link_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = [
                    ACTION_NAMES.cpp_link_executable,
                    ACTION_NAMES.cpp_link_dynamic_library,
                    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
                ],
                flag_groups = [
                    flag_group(
                        flags = [
                            "-lstdc++",
                        ],
                    ),
                ],
            ),
        ],
    )

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "terawave-riscv64-toolchain",
        host_system_name = "local",
        target_system_name = "riscv64-linux-gnu",
        target_cpu = "riscv64",
        target_libc = "glibc",
        compiler = "gcc",
        abi_version = "riscv64",
        abi_libc_version = "glibc",

        tool_paths = tool_paths,

        cxx_builtin_include_directories = [
            "/usr/riscv64-linux-gnu/include",
            "/usr/lib/gcc-cross/riscv64-linux-gnu/13/include",
            "/usr/lib/gcc-cross/riscv64-linux-gnu/13/include-fixed",
            "/usr/riscv64-linux-gnu/include/c++/13",
            "/usr/riscv64-linux-gnu/include/c++/13/riscv64-linux-gnu",
        ],

        features = [
            default_compile_flags,
            default_link_flags,
        ],
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
