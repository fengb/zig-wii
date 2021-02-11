const std = @import("std");
const c = @import("c.zig");

var xfb: *c_void = undefined;
var rmode: *c.GXRModeObj = undefined;

export fn zig_main(argc: c_int, argv: [*]const [*:0]const u8) noreturn {
    // Initialise the video system
    c.VIDEO_Init();

    // This function initialises the attached controllers
    _ = c.WPAD_Init();

    // Obtain the preferred video mode from the system
    // This will correspond to the settings in the Wii menu
    rmode = c.VIDEO_GetPreferredMode(null);

    // Allocate memory for the display in the uncached region
    xfb = c.memK0ToK1(c.SYS_AllocateFramebuffer(rmode)) orelse unreachable;

    // Initialise the console, required for printf
    c.console_init(xfb, 20, 20, rmode.fbWidth, rmode.xfbHeight, rmode.fbWidth * c.VI_DISPLAY_PIX_SZ);

    // Set up the video registers with the chosen mode
    c.VIDEO_Configure(rmode);

    // Tell the video hardware where our display memory is
    c.VIDEO_SetNextFramebuffer(xfb);

    // Make the display visible
    c.VIDEO_SetBlack(false);

    // Flush the video register changes to the hardware
    c.VIDEO_Flush();

    // Wait for Video setup to complete
    c.VIDEO_WaitVSync();
    if (rmode.viTVMode & c.VI_NON_INTERLACE != 0) c.VIDEO_WaitVSync();

    // The console understands VT terminal escape codes
    // This positions the cursor on row 2, column 0
    // we can use variables for this with format codes too
    // e.g. printf ("\x1b[%d;%dH", row, column );
    _ = std.c.printf("\x1b[2;0H");

    _ = std.c.printf("Hello world from Zig");

    while (true) {
        // Call WPAD_ScanPads each loop, this reads the latest controller states
        _ = c.WPAD_ScanPads();

        // WPAD_ButtonsDown tells us which buttons were pressed in this loop
        // this is a "one shot" state which will not fire again until the button has been released
        const pressed = c.WPAD_ButtonsDown(0);

        // We return to the launcher application via exit
        if (pressed & c.WPAD_BUTTON_HOME != 0) std.c.exit(0);

        // Wait for the next frame
        c.VIDEO_WaitVSync();
    }
}
