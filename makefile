include config.mk

DEFINES=-DVERSION="$(VERSION)" \
	-DOSFAMILY="$(OS)" \

SOURCE_DIR=sources
BUILD_DIR=build
OBJECT_DIR=$(BUILD_DIR)/objects
CFLAGS+=-I./includes $(DEFINES) -Wno-int-to-void-pointer-cast -Wno-incompatible-pointer-types-discards-qualifiers -Wno-pointer-sign

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC=$(call rwildcard,$(SOURCE_DIR),*.c)
OBJS=$(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SRC))
DIRS=$(wildcard $(SOURCE_DIR)/*)

compiler: $(OBJS) link

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(OBJECT_DIR)

setup:
	@rm -rf $(BUILD_DIR) || true
	mkdir $(BUILD_DIR)
	mkdir $(OBJECT_DIR)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@ echo [ Compiling $^ ]
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

link:
	@ echo [ Linking ]
	$(LD) $(LFLAGS) -o $(BUILD_DIR)/clecxible $(OBJS)

