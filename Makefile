CXX = clang++
CC = clang

EXE = glsl_visualizer
IMGUI_DIR = external/imgui/
GLAD_DIR = external/glad/
SHADER_DIR = assets/shaders/
SRC_DIR = src/
SOURCES = main.cpp
SOURCES += $(shell find $(SRC_DIR) -name "*.cpp")
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(GLAD_DIR)/include/glad/glad.c
BUILD_DIR = build
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(OBJS))

define source-to-object
$(BUILD_DIR)/$(notdir $(basename $(1))).o: $(1)
endef

UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(GLAD_DIR)/include -Iheaders -I$(SRC_DIR)
CFLAGS = -I$(GLAD_DIR)/include
CXXFLAGS += -g -Wall -Wformat
LIBS =

ifeq ($(UNAME_S), Linux) #LINUX
    ECHO_MESSAGE = "Linux"
    LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

    CXXFLAGS += `pkg-config --cflags glfw3`
    CFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw -ldl

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(foreach src,$(SOURCES),$(eval $(call source-to-object,$(src))))

$(BUILD_DIR)/%.o: | $(BUILD_DIR)
	@echo "Building $@ from $<"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Fallback rules with vpath to find source files
vpath %.cpp $(SRC_DIR) $(SRC_DIR)/core $(SRC_DIR)/ui $(SRC_DIR)/utils $(IMGUI_DIR) $(IMGUI_DIR)/backends .
vpath %.c $(GLAD_DIR)/include/glad

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Specific rule for GLAD
$(BUILD_DIR)/glad.o: $(GLAD_DIR)/include/glad/glad.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	@echo Executable created: $(EXE)

clean:
	rm -f $(EXE)
	rm -rf $(BUILD_DIR)

.PHONY: all clean