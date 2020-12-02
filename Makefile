SRC := ./src

TARGET = rng_gen

TEST_SRC := $(wildcard $(SRC)/*.cpp)
TEST_OBJS := $(TEST_SRC:.cpp=.o)

CFLAGS := -std=gnu++17
LDFLAGS := -Llib -pthread
LXXFLAGS := -lstdc++fs -lcurl -lcurlpp

all: $(TARGET)

$(TARGET): $(TEST_OBJS)
	g++ $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LXXFLAGS)
