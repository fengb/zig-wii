FROM devkitpro/devkitppc:latest

ENV PATH="/opt/zig:${PATH}"
ENV ZIG_VERSION="951+9e2472706"
ENV ZLS_SHA="c541479"

# Install zig
RUN mkdir /opt/zig && mkdir /opt/zls && \
	curl -L https://ziglang.org/builds/zig-linux-x86_64-0.9.0-dev.$ZIG_VERSION.tar.xz | tar -xJ --strip-components=1 -C /opt/zig && \
	git clone --recurse-submodules https://github.com/zigtools/zls /opt/zls && \
	cd /opt/zls && \
	git reset --hard $ZLS_SHA && \
	zig build -Drelease-safe

# Entrypoint
ENTRYPOINT /bin/bash
