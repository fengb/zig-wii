const std = @import("std");
const Builder = std.build.Builder;

pub fn build(b: *Builder) void {
    const mode = b.standardReleaseOptions();
    const obj = b.addObject("main", "src/main.zig");
    obj.setOutputDir("build");
    obj.linkLibC();
    obj.setLibCFile("libc.txt");
    obj.addIncludeDir("devkitpro/libogc/include");
    obj.setBuildMode(mode);
    obj.setTarget(.{
        .cpu_arch = .powerpc,
        .os_tag = .freestanding,
        .abi = .eabi,
        .cpu_model = .{ .explicit = &std.Target.powerpc.cpu.ppc750 },
        .cpu_features_add = std.Target.powerpc.featureSet(&.{.hard_float}),
    });

    const make = b.addSystemCommand(&[_][]const u8{ "docker-compose", "run", "devkitpro", "make" });
    b.default_step.dependOn(&obj.step);
    b.default_step.dependOn(&make.step);

    var main_tests = b.addTest("src/main.zig");
    main_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run library tests");
    test_step.dependOn(&main_tests.step);
}
