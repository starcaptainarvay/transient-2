CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = -lglfw -lvulkan -lavformat -lavcodec -lavutil -lswscale

# SRCS = src/vulkan_window.c src/video_player.c src/video_manager.c src/obrien.c
SRCS = src/vulkan_window.c src/obrien.c
OBJS = $(SRCS:.c=.o)
EXEC = Transient.exe

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
