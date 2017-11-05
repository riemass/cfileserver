OBJDIR := 'build'
OBJS := $(addprefix $(OBJDIR)/,transfer_functions.o)
SERV_OBJS := $(addprefix $(OBJDIR)/,sendfile.o)
CL_OBJS := $(addprefix $(OBJDIR)/,recvfile.o)

vpath %.c src
$(OBJDIR)/%.o : %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $< -Iinc

server: $(OBJS) $(SERV_OBJS)
	$(CC) $^ $(OUTPUT_OPTION)

client: $(OBJS) $(CL_OBJS)
	$(CC) $^ $(OUTPUT_OPTION)

all: server client

clean:
	rm -rf $(OBJDIR) server client
