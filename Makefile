CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = app
LIB = libwave.a

all: $(LIB) $(TARGET)

# Создаём библиотеку
$(LIB): src/physics/Vector3.o src/physics/Wave.o src/render/WaveRenderer.o
	ar rcs $@ $^

# Собираем программу
$(TARGET): main.o $(LIB)
	$(CXX) $< -o $@ -L. $(LDFLAGS) -lwave

# Компиляция .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/physics/*.o src/render/*.o main.o $(LIB) $(TARGET)

.PHONY: all run clean
