const std = @import("std");
const Builder = std.build.Builder;

pub fn build(b: *Builder) void {
    const mode = b.standardReleaseOptions();
    const obj = b.addObject("main", "src/main.zig");
    obj.setOutputDir("build");
    obj.linkLibC();
    obj.setLibCFile(std.build.FileSource{ .path = "libc.txt" });
    obj.addIncludeDir("vendor/devkitpro/libogc/include");
    obj.setBuildMode(mode);
    obj.setTarget(.{
        .cpu_arch = .powerpc,
        .os_tag = .freestanding,
        .abi = .eabi,
        .cpu_model = .{ .explicit = &std.Target.powerpc.cpu.@"750" },
        .cpu_features_add = std.Target.powerpc.featureSet(&.{.hard_float}),
    });

    const elf = b.addSystemCommand(&[_][]const u8{ "docker-compose", "run", "devkitpro", "/opt/devkitpro/devkitPPC/bin/powerpc-eabi-gcc", "build/main.o", "-g", "-DGEKKO", "-mrvl", "-mcpu=750", "-meabi", "-mhard-float", "-Wl,-Map,.map", "-L/opt/devkitpro/libogc/lib/wii", "-lwiiuse", "-lbte", "-logc", "-lm", "-o", "zig-wii.elf" });
    const dol = b.addSystemCommand(&[_][]const u8{ "docker-compose", "run", "devkitpro", "elf2dol", "zig-wii.elf", "zig-wii.dol" });
    b.default_step.dependOn(&dol.step);
    dol.step.dependOn(&elf.step);
    elf.step.dependOn(&obj.step);
}
