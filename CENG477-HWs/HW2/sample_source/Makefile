CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2
LDFLAGS = -lm

TARGET = rasterizer

SRCS = Main.cpp Scene.cpp Camera.cpp Helpers.cpp Instance.cpp \
       Color.cpp Matrix4.cpp Vec4.cpp Vec3.cpp \
       Vec4WithColor.cpp Vec3WithColor.cpp Triangle.cpp \
       Translation.cpp Scaling.cpp Rotation.cpp tinyxml2.cpp Mesh.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean rasterizer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rasterizer: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
	rm -f *.ppm # Optionally remove generated output images