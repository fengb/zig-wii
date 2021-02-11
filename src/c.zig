pub usingnamespace @cImport({
    @cInclude("gccore.h");
    @cInclude("wiiuse/wpad.h");
});

// -- Manually translated macros
pub inline fn memVirtualToPhysical(x: anytype) u32 {
    return (@import("std").meta.cast(u32, x)) & ~SYS_BASE_UNCACHED;
}
pub inline fn memPhysicalToK0(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) + SYS_BASE_CACHED));
}
pub inline fn memPhysicalToK1(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) + SYS_BASE_UNCACHED));
}
pub inline fn memK0ToPHYSICAL(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) - SYS_BASE_CACHED));
}
pub inline fn memK1ToPHYSICAL(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) - SYS_BASE_UNCACHED));
}
pub inline fn memK0ToK1(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) + (SYS_BASE_UNCACHED - SYS_BASE_CACHED)));
}
pub inline fn memK1ToK0(x: anytype) ?*c_void {
    return (@import("std").meta.cast(?*c_void, (@import("std").meta.cast(u32, x)) - (SYS_BASE_UNCACHED - SYS_BASE_CACHED)));
}
