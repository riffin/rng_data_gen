SRC := ./src

TARGET =test

TEST_SRC := $(wildcard $(SRC)/*.cpp)
TEST_OBJS := $(TEST_SRC:.cpp=.o)

CFLAGS := -std=c++17
LDFLAGS := -lstdc++fs

all: $(TARGET)

$(TARGET): $(TEST_OBJS)
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^
