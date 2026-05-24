# Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
TARGET   := minishell
SRCS     := main.cpp parser.cpp executor.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
